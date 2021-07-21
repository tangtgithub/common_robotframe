#include "MobileRobot.h"
#include "taskcommoninc.h"
#include "CentralPMCallback.h"
#include "AlgorithmCallback.h"


using namespace taskcommon;


MobileRobot::MobileRobot() : m_taskEntryCb(nullptr),
                             m_runStatus(taskcommon::kDefault)
{

}

void MobileRobot::createTask(const TaskBasicInfo& taskBasicInfo)
{
    Task task(taskBasicInfo.id, taskBasicInfo.executationType, nullptr);

    auto entryFun = [this](uint32_t taskId) -> void
    {
        m_curRunInfo.taskId = taskId;
        if ( m_taskEntryCb )
        {
            m_taskEntryCb(taskId);
        }
        else
        {
            SPDLOG_WARN("createTask::none task entry");
        }
    };
    task.runFunc = entryFun;
    CentralPMCallback::getTaskScheHandler()->addTask(task);
}

void MobileRobot::cancelTask(uint32_t reason)
{
    if ( m_taskEventCbs.find(kTaskCancelEvent) == m_taskEventCbs.end() )
    {
        return;
    }
    for ( auto eventcb : m_taskEventCbs[kTaskCancelEvent] )
    {
        eventcb(reason);
    }
}

void MobileRobot::pauseTask(uint32_t reason) 
{
    if ( m_taskEventCbs.find(kTaskPauseEvent) == m_taskEventCbs.end() )
    {
        return;
    }
    for ( auto eventcb : m_taskEventCbs[kTaskPauseEvent] )
    {
        eventcb(reason);
    }

}

void MobileRobot::resumeTask(uint32_t reason)
{
    if ( m_taskEventCbs.find(kTaskResumeEvent) == m_taskEventCbs.end() )
    {
        return;
    }
    for ( auto eventcb : m_taskEventCbs[kTaskResumeEvent] )
    {
        eventcb(reason);
    }

}

void MobileRobot::navicatResCallback(uint32_t taskId, uint32_t rangeId, uint8_t status)
{
    setRunStatus(kDefault);
    std::vector<RunEndCallback> runEndCbs;

    {
        std::lock_guard<std::mutex> lck(m_callMutex);
        runEndCbs = m_runEndCbs;
    }
    for ( const auto& funcb : runEndCbs )
    {
        funcb(rangeId, status);
    }
}

void MobileRobot::registerRunEndCb(taskcommon::RunEndCallback runEndCb)
{
    std::lock_guard<std::mutex> lck(m_callMutex);
    m_runEndCbs.push_back(runEndCb);
}


void MobileRobot::registerTaskEntryCb(taskcommon::TaskEntryCallback taskEntryCb)
{
    std::lock_guard<std::mutex> lck(m_callMutex);
    m_taskEntryCb = taskEntryCb;
}

void MobileRobot::registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb) 
{
    std::lock_guard<std::mutex> lck(m_callMutex);
    if ( m_taskEventCbs.find(eventType) == m_taskEventCbs.end())
    {
        std::vector<taskcommon::TaskEventCallback> cbs;
        cbs.push_back(taskEventCb);
        m_taskEventCbs[eventType] = cbs;
    }
    else
    {
        m_taskEventCbs[eventType].push_back(taskEventCb);
    }
}

int MobileRobot::runArea(uint32_t areaId)
{
    return 0;
}

int MobileRobot::runLine(uint32_t lineId)
{
    return 0;
}

int MobileRobot::runPoint(uint32_t pointId)
{
    return 0;
}

int MobileRobot::run(uint32_t plrId, uint32_t type)
{
    setRunStatus(kRunning);
    return 0;
}

int MobileRobot::runPath(uint32_t id, const vector<globalmap::CommonPoint>& pointList)
{
    m_curRunInfo.plrId = id;
    m_curRunInfo.type = kLine;

    //navicat run path
    
    setRunStatus(kRunning);

    return 0;
}

int MobileRobot::pause()
{
    if ( taskcommon::kRunning != getRunStatus() )
    {
        SPDLOG_WARN("pause::invalid pause,it is not currently running");
        return -1;
    }

    // navicat pause

    setRunStatus(kPause);

    return 0;
}

int MobileRobot::resume()
{
    if ( taskcommon::kPause != getRunStatus() )
    {
        return -1;
    }

    //navicat resume

    setRunStatus(kRunning);
    return 0;
}

int MobileRobot::cancel()
{
    if ( taskcommon::kDefault == getRunStatus() )
    {
        SPDLOG_WARN("cancel::invalid cancel,it is not currently running");
        return -1;
    }

    //navicat cancel

    setRunStatus(kDefault);
    return 0;

}

uint32_t MobileRobot::getRunStatus()
{
    std::lock_guard<std::mutex> lck(m_runStatusMutex);
    return m_runStatus;
}

void  MobileRobot::setRunStatus(uint32_t runStatus)
{
    std::lock_guard<std::mutex> lck(m_runStatusMutex);
    m_runStatus = runStatus;
}
