#include "ServerImpl.h"
#include <sys/prctl.h>
#include "CentralPMCallback.h"
#include <cstdlib>
#include "telnetCommon.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "HandleString.hpp"
#include "Spdlogger.hpp"

#define PI acos(-1)



using namespace admincmd;

void ServerImpl::run(uint16_t port)
{
    m_pCpm = CentralPMCallback::getDeriveInstance();
    if (!m_adminCmdServer.init("0.0.0.0", port))
    {
        std::cout << "m_adminCmdServer init failed: " << m_adminCmdServer.getLastError() << std::endl;
        return;
    }
    m_strCommonCmdHelp = ">>>> test\n"
        ">>>> setLogLevel(moduleid,level)\n"
        ">>>> getModuleIdInfo()\n"
        ">>>> quit\n";

    m_strAdminCmdHelp = ">>>> common\n"
        ">>>> algorithm\n"
        ">>>> exit\n";

    m_strAlgorithmCmdHelp = ">>>> generateMap() \n"
        ">>>> testPath()\n"
        ">>>> quit\n";

    m_bRunning = true;
    m_adminCmdThread = std::thread([this]() {
        prctl(PR_SET_NAME, "telnetServer");
        while (m_bRunning.load(std::memory_order_relaxed))
        {
            m_adminCmdServer.poll(this);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    std::cout << "Server running..." << std::endl;
    m_adminCmdThread.detach();

}

void ServerImpl::stop()
{
    m_bRunning = false;
}


void ServerImpl::onAdminConnect(AdminConn& conn)
{
    struct sockaddr_in addr;
    conn.getPeername(addr);
    std::cout << "admin connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;

    conn.user_data.login = false;
    switch (m_unModule)
    {
    case emModule::Common:
    {
        conn.write(m_strCommonCmdHelp.data(), static_cast<uint32_t>(m_strCommonCmdHelp.size()));
        break;
    }
    case emModule::Algorithm:
    {
        conn.write(m_strAlgorithmCmdHelp.data(), static_cast<uint32_t>(m_strAlgorithmCmdHelp.size()));
        break;
    }
    default:
    {
        conn.write(m_strAdminCmdHelp.data(), static_cast<uint32_t>(m_strAdminCmdHelp.size()));
        break;
    }
    }
    conn.write("#", 1);

}


void ServerImpl::onAdminDisconnect(AdminConn& conn, const std::string& error)
{
    struct sockaddr_in addr;
    conn.getPeername(addr);
    std::cout << "admin disconnect, error: " << error << ", login state: " << conn.user_data.login << std::endl;

}

void ServerImpl::onAdminCMD(AdminConn& conn, int argc, const char** argv)
{
    std::string resp = onCMD(conn.user_data, argc, argv);
    if (resp.size() == 1)
    {
        if (resp.back() != '\n')
        {
            resp.push_back('\n');
            conn.write(resp.data(), static_cast<uint32_t>(resp.size()));
        }
    }
    else
    {
        if (resp.back() != '\n')
        {
            resp.push_back('\n');
        }
        conn.write(resp.data(), static_cast<uint32_t>(resp.size()));

    }

    switch (m_unModule)
    {
    case emModule::Common:
    {
        conn.write("CommonModule#", sizeof("CommonModule#"));
        break;
    }
    case emModule::Algorithm:
    {
        conn.write("Algorithm#", sizeof("Algorithm#"));
        break;
    }
    default:
    {
        conn.write("#", 1);
        break;
    }
    }

}

std::string ServerImpl::onCMD(CMDConnData& conn, int argc, const char** argv)
{
    std::cout << "onCMD:" << argv[0] << std::endl;

    std::string strCmd(argv[0]);

    switch (m_unModule)
    {
    case emModule::Common:
    {
        return commonCmdParse(strCmd);
    }
    case emModule::Algorithm:
    {
        return algorithmCmdParse(strCmd);
    }
    default:
    {
        return baseCmdParse(strCmd);
        break;
    }
    }

}

std::string ServerImpl::baseCmdParse(const std::string& cmd)
{
    std::cout << "commonCmdParse" << cmd.size() << std::endl;
    std::string tmpCmd;
    if (cmd.size() > 2)
    {
        tmpCmd = cmd.substr(0, cmd.find_last_of("\r"));
        std::cout << "tmpCmd:" << tmpCmd << "  " << tmpCmd.size() << std::endl;
    }
    else
    {
        std::cout << "huiche" << std::endl;
        return "\n";
    }

    if (tmpCmd == "help")
    {
        return m_strAdminCmdHelp;
    }
    else if (tmpCmd.find("algorithm") != std::string::npos)
    {
        m_unModule = emModule::Algorithm;
        return "\n";
    }
    else if (tmpCmd.find("common") != std::string::npos)
    {
        m_unModule = emModule::Common;
        return "\n";
    }
    else
    {
        return "invalid cmd, check help";
    }
}

std::string ServerImpl::commonCmdParse(const std::string& cmd)
{
    std::vector<std::string> vargs;
    std::cout << "commonCmdParse" << cmd.size() << std::endl;
    std::string tmpCmd;
    if (cmd.size() > 2)
    {
        tmpCmd = cmd.substr(0, cmd.find_last_of("\r"));
        std::cout << "tmpCmd:" << tmpCmd << "  " << tmpCmd.size() << std::endl;
    }
    else
    {
        std::cout << "huiche" << std::endl;
        return "\n";
    }
    if (tmpCmd == "help")
    {
        return m_strCommonCmdHelp;
    }
    if (tmpCmd == "test")
    {
        return "test";
    }
    else if ( tmpCmd.find("getModuleIdInfo") != std::string::npos  )
    {
        return Spdlogger::getInstance()->getDescribe();
    }
    else if ( tmpCmd.find("setLogLevel") != std::string::npos  )
    {
        if (!parseArgs(tmpCmd,vargs))
        {
            return "args parse error";
        }
        if (vargs.size() == 1) {
            Spdlogger::getInstance()->SetLogLevel(atoi(vargs.at(0).c_str()));
        }
        else if (vargs.size() == 2) {
            Spdlogger::getInstance()->setLogLevel(static_cast<emModuleId>(atoi(vargs.at(0).c_str())), atoi(vargs.at(1).c_str()));
        }
        return "\n";
    }
    else if (tmpCmd == "quit")
    {
        m_unModule = emModule::Default;
        return "\n";
    }
    else
    {
        return "invalid cmd, check help";
    }
    return "";
}
std::string ServerImpl::algorithmCmdParse(const std::string& cmd)
{
    std::cout << "::algorithmCmdParse" << cmd.size() << std::endl;
    std::string tmpCmd;
    if (cmd.size() > 2)
    {
        tmpCmd = cmd.substr(0, cmd.find_last_of("\r"));
        std::cout << "tmpCmd:" << tmpCmd << "  " << tmpCmd.size() << std::endl;
    }
    else
    {
        std::cout << "huiche" << std::endl;
        return "\n";
    }
    if (tmpCmd == "help")
    {
        return m_strAlgorithmCmdHelp;
    }
    else if (tmpCmd.find("generateMap") != std::string::npos)
    {
        std::cout << "generateMap yes" << std::endl;
        algorithm::Point minp;
        algorithm::Point maxp;
        minp.dx = 0;
        minp.dy = 0;
        maxp.dx = 5;
        maxp.dy = 5;

        m_pCpm->getAlgorithmHandler()->loadMap(minp,maxp);
        m_pCpm->getAlgorithmHandler()->switchToPng("/home/tangt/algorithm.png");
        return "push success";
    }
    else if (tmpCmd.find("testPath") != std::string::npos)
    {
        m_pCpm->getAlgorithmHandler()->test2();
        return "push success";
    }
    else if (tmpCmd == "quit")
    {
        m_unModule = emModule::Default;
        return "\n";
    }
    else
    {
        return "invalid cmd, check help";
    }
}


bool ServerImpl::parseArgs(std::string& strFunInfo,std::vector<std::string>& vecArgs)
{
        std::string::size_type beginPos = strFunInfo.find("(");
        std::string::size_type endPos = strFunInfo.find(")");

        if ( beginPos == std::string::npos || endPos == std::string::npos )
        {
            return false;
        }

        std::string args = strFunInfo.substr(beginPos+1,endPos-beginPos-1);
        HandleString::split(args,',',vecArgs);
        return true;

}
