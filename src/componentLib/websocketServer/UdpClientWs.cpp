#include "UdpClientWs.h"
#include "websocketServerCommon.h"
#include "basecommon.h"

UdpClient::UdpClient():m_fd(0)
{
    memset(m_serverIp, 0, sizeof(m_serverIp));
}

UdpClient::~UdpClient()
{
    if (m_fd)
        close(m_fd);
}

int UdpClient::initClient(uint16_t port)
{
    int iOptval = 1;

    if ((m_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        spdlog::error("socket fail");
        return FAILURE;
    }

    memset(&m_client_addr, 0, sizeof(struct sockaddr_in));

    m_client_addr.sin_family = AF_INET;

    m_client_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    m_client_addr.sin_port = htons(port);

    if (setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        spdlog::error("setsockopt failed!");
        return FAILURE;
    }
    return SUCCESS;
}


int UdpClient::writeData(const void* buf, int length)
{
    //return static_cast<int>(write(m_connectFd, buf, length));
    if ((sendto(m_fd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&m_client_addr), sizeof(struct sockaddr))) == -1)
    {
        spdlog::error("sendto failed, errno={}\n", errno);
        return FAILURE;
    }

    return SUCCESS;
}

int UdpClient::writeData(const void* buf, int length, const std::string& strIp, uint16_t port)
{
    int iOptval = 1;
    int ret = FAILURE;
    int fd;
    do
    {
        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            spdlog::error("socket fail");
            break;
        }

        memset(&m_client_addr, 0, sizeof(struct sockaddr_in));
        m_client_addr.sin_family = AF_INET;
        //m_client_addr.sin_addr.s_addr = inet_addr("192.168.135.255");
        m_client_addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        m_client_addr.sin_port = htons(port);

        if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
        {
            spdlog::error("setsockopt failed!");
            break;
        }

        if ((sendto(fd, buf, length, 0, reinterpret_cast<struct sockaddr*>(&m_client_addr), sizeof(struct sockaddr))) == -1)
        {
            spdlog::error("sendto failed, errno={}\n", errno);
            break;
        }
        ret = SUCCESS;
    } while (0);

    close(fd);

    return ret;
}
