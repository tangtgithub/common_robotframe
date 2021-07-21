#ifndef __FUNCTIONHANDLERBASE_H__
#define __FUNCTIONHANDLERBASE_H__

#include <string>
#include "commondefine.h"
#include "DBCallback.h"
#include "MqttCallback.h"
#include "WebsocketSerCallback.h"
#include "NavicatCallback.h"
#include "PushRtmpCallback.h"
#include "HttpCallback.h"
#include "AlgorithmCallback.h"
#include "TaskSchedulerCallback.h"
#include "TaskCommonCallback.h"
#include "StateManagerCallback.h"


class GeneralAbilityCallback
{
public:

    static GeneralAbilityCallback* getDeriveInstance();
    virtual void init() {}
    virtual void start() {}
    virtual void deviceStatusReport(uint8_t deviceStatus) {}
};

#endif
