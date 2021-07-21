#include "TestRobotCallback.h"
#include "TestRobotCb.h"


TestRobotCallback* TestRobotCallback::getDeriveInstance()
{
    static TestRobotCb testRobotCb;
    return &testRobotCb;
}
