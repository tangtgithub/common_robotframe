#include "MsgFromCloud.h"
#include "MqttCb.h"
#include "mqttCommon.h"
#include "json/json.h"

void MsgFromCloud::onMsg(int cmd, std::string msg)
{
    switch (cmd)
    {
    default:
        break;
    }
}
