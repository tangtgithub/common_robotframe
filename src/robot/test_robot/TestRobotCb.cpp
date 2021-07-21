#include "TestRobotCb.h"
#include "TestRobotImpl.h"


TestRobotCb::TestRobotCb()
{
    m_pTestRobotImpl = new TestRobotImpl();
    m_pTestRobotBusiness = new TestRobotBusiness();
}
TestRobotCb::~TestRobotCb()
{

}

void TestRobotCb::initTestRobot(RobotBusiness*& pRobotBusiness)
{
    m_pTestRobotImpl->initTestRobot();
}

void TestRobotCb::startTestRobot()
{
    m_pTestRobotImpl->startTestRobot();
}
