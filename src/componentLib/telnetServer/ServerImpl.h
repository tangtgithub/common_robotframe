#ifndef __ADMINCMD_SERVERIMPL_H
#define __ADMINCMD_SERVERIMPL_H

#include "AdminCmdServer.hpp"
#include <iostream>
#include <thread>
#include <atomic>
#include <signal.h>

enum class emModule : uint32_t
{
    Default,
    Common,
    Algorithm,
};

class CentralPMCallback;

namespace admincmd {
    class ServerImpl
    {
    public:
        struct CMDConnData
        {
            bool login;
        };
        using AdminServer = admincmd::AdminCMDServer<ServerImpl, CMDConnData>;
        using AdminConn = AdminServer::Connection;

        void run(uint16_t port = 1234);

        void stop();

        void onAdminConnect(AdminConn& conn);

        void onAdminDisconnect(AdminConn& conn, const std::string& error);

        void onAdminCMD(AdminConn& conn, int argc, const char** argv);

    private:
        std::string onCMD(CMDConnData& conn, int argc, const char** argv);
        std::string baseCmdParse(const std::string& cmd);
        std::string taskCmdParse(const std::string& cmd);
        std::string commonCmdParse(const std::string& cmd);
        std::string cleaningCmdParse(const std::string& cmd);
        std::string navicatCmdParse(const std::string& cmd);
        std::string websocketCmdParse(const std::string& cmd);
        std::string cloudCmdParse(const std::string& cmd);

        std::string mqttCmdParse(const std::string& cmd);
        std::string commonRobotCmdParse(const std::string& cmd);

        std::string algorithmCmdParse(const std::string& cmd);

        bool parseArgs(std::string& strFunInfo,std::vector<std::string>& vecArgs);

    private:
        CentralPMCallback* m_pCpm = nullptr;
        AdminServer              m_adminCmdServer;
        std::thread              m_adminCmdThread;
        std::string              m_strAdminCmdHelp;
        std::string              m_strCommonCmdHelp;
        std::string              m_strAlgorithmCmdHelp;
        std::atomic<bool>        m_bRunning;
        emModule                 m_unModule = emModule::Default;

    };
}

#endif
