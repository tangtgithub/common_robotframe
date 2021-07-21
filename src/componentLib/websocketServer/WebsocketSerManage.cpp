#include "WebsocketServer.h"
#include "WebsocketSerManage.h"
#include "websocketServerCommon.h"


void  WebsocketSerManage::init()
{
    m_pWebsocketSer = std::make_shared<WebsocketServer>();

    m_pRobotControl = std::make_shared<RobotControl>(m_pWebsocketSer);
    m_pRobotControl->init();

    m_pWebsocketSer->setOnOpenCb(std::bind(&RobotControl::onOpen, m_pRobotControl, std::placeholders::_1));
    m_pWebsocketSer->setOnCloseCb(std::bind(&RobotControl::onClose, m_pRobotControl, std::placeholders::_1));
    m_pWebsocketSer->setOnMsgCb(std::bind(&RobotControl::onMessage, m_pRobotControl, std::placeholders::_1, std::placeholders::_2));

}


void WebsocketSerManage::registerCmd(CmdObserver cmdObserver)
{
    m_pRobotControl->registerCmd(cmdObserver);
}


void WebsocketSerManage::start()
{
    SPDLOG_INFO("websocket server starting!!!");
    m_pWebsocketSer->startServer();
}
