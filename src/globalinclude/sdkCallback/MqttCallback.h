#ifndef __MQTTCALLBACK_H__
#define __MQTTCALLBACK_H__

#include "commondefine.h"
#include "MsgCallback.h"
#include <functional>


struct MqttConnectInfo
{
    std::string deviceCode;
    std::string pubtopic;
    std::string subTopic;
    std::string serverIp;
    std::string username;
    std::string passwd;
    int port;
};

class MqttCallback
{
public:
    static MqttCallback* getDeriveInstance();

    virtual void init(const MqttConnectInfo& info) { return; }
    virtual void start() {}
    virtual void mqttPublishMsg(const std::string& msg, int cmd, int qos = 2) { return; }
    /*
    * 功能:注册消息回调
    * 参数:CmdObserver.cmds:需要单独处理的cmd的集合
    * 参数:CmdObserver.p_msgHandler:cmds的处理类。一个cmd可以对应一个处理类.
    */
    virtual void registerCmd(CmdObserver cmdObserver) { return; }
};

#endif
