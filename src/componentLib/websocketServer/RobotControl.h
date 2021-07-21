#ifndef __ROBOT_CONTROL_H__
#define __ROBOT_CONTROL_H__

#include <boost/filesystem.hpp>
#include <vector>
#include "json/json.h"
#include "MsgCallback.h"
#include "WebsocketServer.h"
#include "commondefine.h"

class RobotControl;
typedef std::shared_ptr<RobotControl> RobotControlPtr;
class RobotControl
{
public:
    RobotControl(std::shared_ptr<WebsocketServer> p_websocketSer);
    ~RobotControl();

    void init();
    void onMessage(int sessionId, const std::string& msgJson);
    void onClose(int sessionId) {}
    void onOpen(int sessionId) {}
    void registerCmd(const CmdObserver& cmdObserver);

private:
    void handleMsg(int sessionId, const std::string& msgJson);
    bool checkCmdsOwner(int cmd, std::vector<MsgCallback*>& handlers);
    int transmitToRegistrant(int sessionIdWithNum, const std::string& jsonMsg);

private:
    std::shared_ptr<WebsocketServer> m_pWebsocketSer;
    std::vector<CmdObserver> m_cmdsAndHandler;
};

#endif
