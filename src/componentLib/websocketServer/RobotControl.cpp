#include "RobotControl.h"
#include <thread>
#include "websocketServerCommon.h"
#include <regex>
#include <future>
#include <functional>

inline void recordOtherException()
{
    try {
        std::rethrow_exception(current_exception());
    }
    catch (exception& e) {
        SPDLOG_ERROR(e.what());
    }
}


RobotControl::RobotControl(std::shared_ptr<WebsocketServer> p_websocketSer) : m_pWebsocketSer(p_websocketSer)
{

}

void RobotControl::init()
{
}

RobotControl::~RobotControl()
{

}

void RobotControl::handleMsg(int sessionId, const std::string& msgJson)
{


}

void RobotControl::onMessage(int sessionId, const std::string& msgJson)
{
    if ( SUCCESS == transmitToRegistrant(sessionId, msgJson) )
    {
        return;
    }
    else
    {
        handleMsg(sessionId, msgJson);
    }
}

bool RobotControl::checkCmdsOwner(int cmd, std::vector<MsgCallback*>& handlers)
{
    bool ret = false;
    for (const auto& observer : m_cmdsAndHandler)
    {
        auto iter = std::find(observer.cmds.begin(), observer.cmds.end(), cmd);
        if (iter != observer.cmds.end())
        {
            ret = true;
            handlers.push_back(observer.p_handler);
        }
    }
    return ret;
}

int RobotControl::transmitToRegistrant(int sessionIdWithNum, const std::string& jsonMsg)
{
    int cmd = 0; // parse jsonMsg
    std::vector<MsgCallback*> handlers;
    if (checkCmdsOwner(cmd, handlers))
    {
        SPDLOG_INFO("transmit websocket msg");
        for (const auto x : handlers)
        {
            x->onMsg(cmd, sessionIdWithNum, jsonMsg);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

void RobotControl::registerCmd(const CmdObserver& cmdObserver)
{
    m_cmdsAndHandler.push_back(cmdObserver);
}

