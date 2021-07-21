#include "MqttCallback.h"
#include "MqttCb.h"

MqttCallback* MqttCallback::getDeriveInstance()
{
    static MqttCb mqttcb;
    return &mqttcb;
}
