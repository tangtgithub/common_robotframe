#ifndef __UDP_SERVER_WS_H_
#define __UDP_SERVER_WS_H_

#include <string>

class UdpServer
{
public:
    UdpServer();
    ~UdpServer();

    void initServer(uint16_t port);
    int readData(void* buf, int length, std::string& strIp);
    int readData(void* buf, int length);
    void setTimeout(int seconds);

private:
    int m_sockfd;
};


#endif
