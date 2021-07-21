#include "TaskCommonCallback.h"
#include "TaskCommon.h"

TaskCommonCallback* TaskCommonCallback::getDeriveInstance()
{
    static  taskcommon::TaskCommon taskCommon;
    return &taskCommon;
}
