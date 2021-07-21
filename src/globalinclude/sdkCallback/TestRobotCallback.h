#ifndef __TESTROBOTCALLBACK_H
#define __TESTROBOTCALLBACK_H

#include "commondefine.h"
#include "RobotBusiness.h"


class TestRobotCallback
{
public:
    static TestRobotCallback* getDeriveInstance();
    virtual void initTestRobot(RobotBusiness*& pRobotBusiness) {}
    virtual void startTestRobot() {}
};

#endif


