#include "TaskSchedulerCallback.h"
#include "TaskScheduler.h"

TaskSchedulerCallback* TaskSchedulerCallback::getDeriveInstance()
{
    static TaskScheduler taskScheduler;
    return &taskScheduler;
}
