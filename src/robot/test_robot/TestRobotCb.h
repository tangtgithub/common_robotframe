#ifndef __TESTROBOTCB_H
#define __TESTROBOTCB_H

#include "commondefine.h"
#include "TestRobotCallback.h"
#include "TestRobotBusiness.h"

class TestRobotImpl;

class TestRobotCb : public TestRobotCallback
{
public:
    TestRobotCb();
    ~TestRobotCb();

    void initTestRobot(RobotBusiness*& pRobotBusiness) override;

    void startTestRobot() override;
private:
    TestRobotImpl* m_pTestRobotImpl = nullptr;
    TestRobotBusiness* m_pTestRobotBusiness = nullptr;
};

#endif
