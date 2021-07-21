#include "TaskCommon.h"
#include "taskcommoninc.h"

using namespace taskcommon;

TaskCommon::TaskCommon()
{
#if defined(MOBILE_ROBOT)
    m_pInterface = std::make_shared<MobileRobot>();
#elif defined(OTHER_ROBOT)

#else
    m_pInterface = std::make_shared<Interface>();
#endif
    
}

TaskCommon::~TaskCommon()
{

}

void TaskCommon::init()
{

}

void TaskCommon::start()
{
    SPDLOG_INFO("taskcommon start!!!");

}

void TaskCommon::createTask(const TaskBasicInfo& taskBasicInfo)
{
    SPDLOG_INFO("TaskCommon::createTask");
    m_pInterface->createTask(taskBasicInfo);
}

void TaskCommon::cancelTask(uint32_t reason)
{
    m_pInterface->cancelTask(reason);
}

void TaskCommon::pauseTask(uint32_t reason) 
{
    m_pInterface->pauseTask(reason);
}

void TaskCommon::resumeTask(uint32_t reason)
{
    m_pInterface->resumeTask(reason);
}

void TaskCommon::navicatResCallback(uint32_t taskId, uint32_t rangeId, uint8_t status) 
{
    m_pInterface->navicatResCallback(taskId,rangeId,status); 
}

void TaskCommon::registerRunEndCb(taskcommon::RunEndCallback runEndCb)
{
    m_pInterface->registerRunEndCb(runEndCb);
}


void TaskCommon::registerTaskEntryCb(taskcommon::TaskEntryCallback taskEntryCb)
{
    m_pInterface->registerTaskEntryCb(taskEntryCb);
}

void TaskCommon::registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb)
{
    m_pInterface->registerTaskEventCb(eventType, taskEventCb);
}

int TaskCommon::run(uint32_t plrId, uint32_t type)
{
    return m_pInterface->run(plrId,type);
}

int TaskCommon::runPath(uint32_t id, const std::vector<globalmap::CommonPoint>& pointList)
{
    return m_pInterface->runPath(id, pointList);
}

int TaskCommon::pause()
{
    return m_pInterface->pause();
}

int TaskCommon::resume()
{
    return m_pInterface->resume();
}

int TaskCommon::cancel()
{
    return m_pInterface->cancel();
}
