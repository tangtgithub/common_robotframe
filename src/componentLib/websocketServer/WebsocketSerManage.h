#ifndef __WEBSOCKET_SERVERMANAGE_H__
#define __WEBSOCKET_SERVERMANAGE_H__

#include <memory>
#include "WebsocketServer.h"
#include "RobotControl.h"
#include "WebsocketSerCallback.h"



class WebsocketSerManage : public WebsocketSerCallback
{
public:
    static WebsocketSerManage* getInstance()
    {
        static WebsocketSerManage wm;
        return &wm;
    }
    void init() override;
    void start() override;
    void registerCmd(CmdObserver cmdObserver) override;

private:
    WebsocketSerManage() {};
    ~WebsocketSerManage() {};
    WebsocketSerManage(const WebsocketSerManage&) = delete;
    WebsocketSerManage& operator=(const WebsocketSerManage&) = delete;

private:
    WebsocketServerPtr m_pWebsocketSer;
    RobotControlPtr    m_pRobotControl;
};

#endif
