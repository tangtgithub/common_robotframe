#include "StateManagerCallback.h"
#include "RobotStateManage.h"

StateManagerCallback* StateManagerCallback::getDeriveInstance()
{
    static RobotStateManage stateManager;
    return &stateManager;
}
