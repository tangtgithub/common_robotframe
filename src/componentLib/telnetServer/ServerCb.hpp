#ifndef __TELNET_SERVERCB_H
#define __TELNET_SERVERCB_H


#include "TelnetSerCallback.h"
#include <memory>
#include "ServerImpl.h"
#include "telnetCommon.h"

using namespace admincmd;

class ServerCb : public TelnetSerCallback
{
public:
    void init(uint16_t port = 7890) override
    {
        m_pServerImpl = std::make_shared<ServerImpl>();
        m_port = port;
    }
    void start() override
    {
        SPDLOG_INFO("telnet start!!!");
        m_pServerImpl->run(m_port);
    }
private:
    std::shared_ptr<ServerImpl>       m_pServerImpl;
    uint16_t  m_port;
};

#endif
