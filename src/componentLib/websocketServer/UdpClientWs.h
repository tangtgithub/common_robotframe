#ifndef __UDP_CLIENT_WS_H_
#define __UDP_CLIENT_WS_H_


#include <sys/types.h> 
#include <sys/socket.h> 
#include <string> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#define MAXLINE 80 
#define SERV_PORT 8888

class UdpClient
{
public:
    UdpClient();
    ~UdpClient();

    int initClient(uint16_t port);
    int writeData(const void* buf, int length);
    int writeData(const void* buf, int length, const std::string& strIp, uint16_t port);

private:
    int m_fd;
    char m_serverIp[16];
    struct sockaddr_in m_client_addr;
};


#endif
