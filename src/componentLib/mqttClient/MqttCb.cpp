#include "MqttCb.h"
#include "mqttCommon.h"
#include "MsgToCloud.h"


MqttCb::MqttCb()
{
}

MqttCb::~MqttCb()
{

}

void MqttCb::init(const MqttConnectInfo& info)
{
    m_pMsg2cloud = std::make_unique<MsgToCloud>();
    m_info = info;
}

void MqttCb::start()
{
    SPDLOG_INFO("mqtt start!!!");

}

void MqttCb::mqttPublishMsg(const std::string& msg, int cmd, int qos)
{
}

void MqttCb::registerCmd(CmdObserver cmdObserver)
{
}

