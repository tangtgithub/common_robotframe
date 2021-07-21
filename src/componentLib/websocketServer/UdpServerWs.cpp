#include "UdpServerWs.h"
#include "websocketServerCommon.h"
#include "basecommon.h"

UdpServer::UdpServer():m_sockfd(0)
{

}

UdpServer::~UdpServer()
{
    if (m_sockfd)
        close(m_sockfd);
}

void UdpServer::initServer(uint16_t port)
{
    struct sockaddr_in saddr;

    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    bind(m_sockfd, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr));
}

int UdpServer::readData(void* buf, int length, std::string& strIp)
{
    ssize_t ret = 0;
    struct sockaddr_in presaddr;
    socklen_t len = sizeof(struct sockaddr);

    ret = recvfrom(m_sockfd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&presaddr), &len);
    if (ret <= 0)
    {
        //if (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN)
            //m_connected = false;
        return FAILURE;
    }
    int  port = ntohs(presaddr.sin_port);
    struct in_addr in = presaddr.sin_addr;
    char str[INET_ADDRSTRLEN];   //INET_ADDRSTRLEN这个宏系统默认定义 16

    inet_ntop(AF_INET, &in, str, sizeof(str));

    printf("ip:port  %s : %d\n", str, port);

    std::string strIpAddr = str;

    strIpAddr = strIpAddr.substr(0, strIpAddr.find_last_of(".")) + ".255";
    std::cout << "ip:" << strIpAddr << std::endl;

    strIp = strIpAddr;

    reinterpret_cast<char*>(buf)[ret] = 0;

    return SUCCESS;
}

void UdpServer::setTimeout(int seconds)
{
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("error");
    }
}

int UdpServer::readData(void* buf, int length)
{
    ssize_t ret = 0;
    struct sockaddr_in presaddr;
    socklen_t len;

    ret = recvfrom(m_sockfd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&presaddr), &len);
    if (ret <= 0)
    {
        //if (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN)
            //m_connected = false;
        return FAILURE;
    }

    reinterpret_cast<char*>(buf)[ret] = 0;

    return SUCCESS;
}
