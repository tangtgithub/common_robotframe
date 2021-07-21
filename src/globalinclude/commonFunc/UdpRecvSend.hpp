#ifndef UDP_RECV_SEND_H
#define UDP_RECV_SEND_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/un.h>

#include <cstring>
#include <string>
#include <functional>

#include <sys/ioctl.h>
#include <net/if.h>

#include "Spdlogger.hpp"

#define READ_SIZE 1024
#define UDP_EVENTS 1

class UdpRecvSend
{
public:
    UdpRecvSend() {}
    ~UdpRecvSend()
    {
        m_bQuitEpollFlag = false;
        if (m_iEpollFd) {
            close(m_iEpollFd);
        }
        if (m_iServerFd) {
            close(m_iServerFd);
        }
        if (m_iClientFd) {
            close(m_iClientFd);
        }
    }

    // 初始化
    // @param iServerPort 服务端端口，如果不使用可以设置为0
    // @param iClientPort 客户端端口，如果不使用可以设置为0
    // @param seconds 服务端超时设置，默认3秒
    bool Init(int iServerPort = 0, int iClientPort = 0, int seconds = 3)
    {
        return Initialization(iServerPort, iClientPort, seconds);
    }

    // 注册服务端数据读取回调
    // @param pCb 待注册的回调函数
    //        buf   读取到的数据
    //        len   读取到的数据长度
    void RegisterReadCb(std::function<void(void* buf, size_t len)> pCb)
    {
        m_pCb = pCb;
    }

    // 注册服务端数据读取回调，包含客户端Ip
    // @param pCbNeedIp 待注册的回调函数
    //        buf   读取到的数据
    //        len   读取到的数据长度
    //        strIp 读取到的客户端Ip
    void RegisterReadCbNeedIp(std::function<void(void* buf, size_t len, std::string& strIp)> pCbNeedIp)
    {
        m_pCbNeedIp = pCbNeedIp;
    }

    // 写入数据
    // @param buf    写入数据
    // @param length 数据长度
    bool WData(const void* buf, int length)
    {
        return WriteDataToServer(buf, length);
    }

    // 指定ip和端口写入数据
    // @param buf    写入数据
    // @param length 数据长度
    // @param strIp  服务端地址
    // @param port   服务端端口
    bool WDataSpecifyIP(const void* buf, int length, const std::string& strIp, uint16_t port)
    {
        LOGINFO(emModuleId::Common,"WDataSpecifyIP:len={},ip={},port={}", length,strIp,port);
        return WriteDataToServer(buf, length, strIp, port);
    }

private:
    bool Initialization(int iServerPort, int iClientPort, int seconds)
    {
        if (iServerPort) {
            if (!StartServer(iServerPort, seconds)) {
                return false;
            }
        }

        if (iClientPort) {
            if (!StartClient(iClientPort)) {
                return false;
            }
        }
        return true;
    }

    bool StartServer(int iServerPort, int seconds)
    {
        struct sockaddr_in saddr{};
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(static_cast<unsigned short>(iServerPort));

        if ((m_iServerFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            LOGWARN(emModuleId::Common,"create udp server falied:{}", strerror(errno));
            return false;
        }

        struct timeval tv;
        tv.tv_sec = seconds;
        tv.tv_usec = 0;
        if (setsockopt(m_iServerFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
            LOGWARN(emModuleId::Common,"set udp server attribute falied:{}", strerror(errno));
        }

        if (bind(m_iServerFd, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr)) == -1) {
            LOGWARN(emModuleId::Common,"bind udp server falied:{}", strerror(errno));
            return false;
        }

        std::thread(&UdpRecvSend::EpollEvents, this).detach();
        return true;
    }

    void EpollEvents()
    {
        struct epoll_event events[UDP_EVENTS];
        m_iEpollFd = epoll_create(UDP_EVENTS);
        AddEvent(m_iEpollFd, m_iServerFd, EPOLLIN);

        while (true) {
            int iResFd = epoll_wait(m_iEpollFd, events, UDP_EVENTS, -1);
            HandleEvents(m_iEpollFd, events, iResFd, m_iServerFd);
            if (m_bQuitEpollFlag) {
                LOGINFO(emModuleId::Common,"leave epoll");
                break;
            }
        }
    }

    void AddEvent(int iEpollFd, int iFd, int iState)
    {
        struct epoll_event ev;
        ev.events = iState;
        ev.data.fd = iFd;
        epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iFd, &ev);
    }

    void HandleEvents(int iEpollFd, struct epoll_event* events, int iResFd, int iServerFd)
    {
        for (int i = 0; i < iResFd ; i++) {
            int iFd = events[i].data.fd;
            if ((iFd == iServerFd) && (events[i].events & EPOLLIN)) {
                ReadDataFromClient(iEpollFd, iServerFd);
            }
        }
    }

    std::string GetBroadcastAddr(std::string strIp)
    {
        int sockfd;
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            LOGWARN(emModuleId::Common,"socket error:{}", strerror(errno));
            return "";
        }

        struct ifconf ifconf;
        std::unique_ptr<char[]> buf(new char[1024]());
        ifconf.ifc_len = 1024;
        ifconf.ifc_buf = buf.get();
        if (ioctl(sockfd, SIOCGIFCONF, &ifconf) < 0) {
             LOGWARN(emModuleId::Common,"ioctl error:{}", strerror(errno));
             return "";
        }

        struct ifreq* ifreq1 = reinterpret_cast<struct ifreq*>(buf.get());
        size_t len = (ifconf.ifc_len / sizeof(struct ifreq));
        for(size_t i = 0; i < len; ++i) {
            std::string strTmpIp = inet_ntoa(reinterpret_cast<struct sockaddr_in*>(&ifreq1->ifr_addr)->sin_addr);
            if (strTmpIp == strIp) {
                struct ifreq ifreq2;
                bzero(&ifreq2, sizeof(ifreq2));
                strcpy(ifreq2.ifr_name, ifreq1->ifr_name);
                if (ioctl(sockfd, SIOCGIFBRDADDR, &ifreq2) < 0) {
                    LOGWARN(emModuleId::Common,"ioctl error:{}", strerror(errno));
                    return "";
                }
                return inet_ntoa(reinterpret_cast<struct sockaddr_in*>(&ifreq2.ifr_addr)->sin_addr);
            }
            ifreq1++;
        }
        return "";
    }

    void ReadDataFromClient(int iEpollFd, int iFd)
    {
        uint8_t buf[READ_SIZE] = {};
        struct sockaddr_in presaddr;
        socklen_t len = sizeof(struct sockaddr);
        ssize_t ret = recvfrom(iFd, buf, READ_SIZE, 0, reinterpret_cast<struct sockaddr*>(&presaddr), &len);
        if (ret <= 0) {
            LOGWARN(emModuleId::Common,"read data falied:{}", strerror(errno));
        }
        if (m_pCb) {
            m_pCb(buf, ret);
        }
        if (m_pCbNeedIp) {
            int  port = ntohs(presaddr.sin_port);
            struct in_addr in = presaddr.sin_addr;
            char str[INET_ADDRSTRLEN]; // INET_ADDRSTRLEN这个宏系统默认定义 16
            inet_ntop(AF_INET, &in, str, sizeof(str));
            LOGINFO(emModuleId::Common,"ip:port {}:{}", str, port);

            std::string strIpAddr = GetBroadcastAddr(str);
            if (!strIpAddr.size()) {
                strIpAddr = str;
                strIpAddr = strIpAddr.substr(0, strIpAddr.find_last_of(".")) + ".255";
            }
            LOGINFO(emModuleId::Common,"ip:{}", strIpAddr);

            m_pCbNeedIp(buf, ret, strIpAddr);
        }
    }

    bool StartClient(int iClientPort)
    {
        if ((m_iClientFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            LOGWARN(emModuleId::Common,"create udp client falied:{}", strerror(errno));
            return false;
        }

        m_client_addr.sin_family = AF_INET;
        m_client_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        m_client_addr.sin_port = htons(static_cast<unsigned short>(iClientPort));

        int iOptval = 1;
        if (setsockopt(m_iClientFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int))  == -1) {
            LOGWARN(emModuleId::Common,"set udp client attribute falied:{}", strerror(errno));
            return false;
        }
        return true;
    }

    bool WriteDataToServer(const void* buf, int length)
    {
        if ((sendto(m_iClientFd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&m_client_addr), sizeof(struct sockaddr))) == -1) {
            LOGWARN(emModuleId::Common,"send data to server falied:{}", strerror(errno));
            return false;
        }
        return true;
    }

    bool WriteDataToServer(const void* buf, int length, const std::string& strIp, uint16_t port)
    {
        int iClientFd;
        if ((iClientFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            LOGWARN(emModuleId::Common,"create udp client falied:{}", strerror(errno));
            return false;
        }

        int iOptval = 1;
        if (setsockopt(iClientFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) == -1) {
            LOGWARN(emModuleId::Common,"set udp client attribute:{}", strerror(errno));
            return false;
        }

        struct sockaddr_in client_addr {};
        memset(&client_addr, 0, sizeof(struct sockaddr_in));
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        client_addr.sin_port = htons(static_cast<unsigned short>(port));

        if ((sendto(iClientFd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&client_addr), sizeof(struct sockaddr))) == -1) {
            LOGWARN(emModuleId::Common,"send data to server falied:{}", strerror(errno));
            return false;
        }
        return true;
    }

private:
    int m_iEpollFd = 0;
    int m_iServerFd = 0;
    int m_iClientFd = 0;
    bool m_bQuitEpollFlag = false;
    struct sockaddr_in m_client_addr {};
    std::function<void(void* buf, size_t len)> m_pCb = nullptr;
    std::function<void(void* buf, size_t len, std::string& strIp)> m_pCbNeedIp = nullptr;
};

#endif /* UDP_RECV_SEND_H */
