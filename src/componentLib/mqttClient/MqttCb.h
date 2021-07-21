#ifndef _MQTT_CB_H_
#define _MQTT_CB_H_

#include "MqttCallback.h"
#include <memory>

class MsgToCloud;
class MqttCb : public MqttCallback
{
public:
    MqttCb();
    ~MqttCb();

    void init(const MqttConnectInfo& info) override;
    void start() override;
    void mqttPublishMsg(const std::string& msg, int cmd, int qos = 2) override;
    void registerCmd(CmdObserver cmdObserver) override;

private:
    std::unique_ptr<MsgToCloud>         m_pMsg2cloud;
    MqttConnectInfo                     m_info;
};

#endif
