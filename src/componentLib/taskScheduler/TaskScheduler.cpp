#include "TaskScheduler.h"
#include "TaskSchedulerCommon.h"
#include <iostream>

TaskScheduler::TaskScheduler()
    : m_isRunning(false)
    , m_strategy(1)
{

}

TaskScheduler::~TaskScheduler()
{

}

void TaskScheduler::addTask(const Task& task)
{
    switch (task.executationType)
    {
    case kImmediatelyTask:
        addImmediatelyTask(task);
        break;
    case kTimingTask:
        addTimedTask(task);
        break;
    case kNoExecTask:
        addNoexecTask(task);
        break;
    default:
        break;
    }
}

void TaskScheduler::deleteTask(uint32_t taskId)
{
    {
        std::lock_guard<std::mutex> lock(m_taskMutex);
        if (taskId == m_runningTask.taskId)
        {
            m_isRunning = false;
            return;
        }

        for (auto it = m_taskList.begin(); it != m_taskList.end(); it++)
        {
            if (it->taskId == taskId)
            {
                m_taskList.erase(it);
                return;
            }
        }
    }

}

void TaskScheduler::init(int strategy) 
{
    m_strategy = strategy;
}

void TaskScheduler::start()
{
    SPDLOG_INFO("taskScheduler start!!!");
}

void TaskScheduler::test()
{
    std::cout << "*****test task scheduler*****\n";
    Task task1(1,1,nullptr);
    Task task2(2,1, nullptr);
    Task task3(3,1, nullptr);
    Task task4(4,3, nullptr);

    addTask(task1);
    addTask(task2);
    addTask(task3);
    addTask(task4);
}

void TaskScheduler::addImmediatelyTask(const Task& task)
{
    {
        std::lock_guard<std::mutex> lock(m_taskMutex);

        if (!m_isRunning)
        {
            m_isRunning = true;
            m_runningTask = task;
            if (task.runFunc)
                task.runFunc(task.taskId);
            else if (m_taskRunFunc)
                m_taskRunFunc(task.taskId);

            SPDLOG_INFO("task(taskID:{}) will be running", task.taskId);

            return;
        }

        return conflictStrategy(task);
    }
    
}

void TaskScheduler::addTimedTask(const Task& task)
{
    //暂不处理定时任务
    return;
}

void TaskScheduler::addNoexecTask(const Task& task)
{
    SPDLOG_INFO("task(taskID:{}) will not be running", task.taskId);
    if (task.endFunc)
    {
        task.endFunc(task.taskId, kUnexecTask); 
    }     

    return;
}

void TaskScheduler::conflictStrategy(const Task& task)
{
    switch (m_strategy)
    {
    case kNewTaskPreempt:
        interruptStrategy(task);
        break;
    case kGiveupNewTask:
        giveUpStrategy(task);
        break;
    case kHighPriorityPreempt:
        highPriorityStrategy(task);
        break;
    case kHighOrSamePriorityPreempt:
        samePriorityStrategy(task);
        break;
    default:
        break;
    }

    return;
}

void TaskScheduler::interruptStrategy(const Task& task)
{
    SPDLOG_INFO("task(taskID:{}) will be interrupt", m_runningTask.taskId);
    if (m_runningTask.endFunc)
    {
        m_runningTask.endFunc(m_runningTask.taskId, kLowPriority);
    }
        

    m_runningTask = task;
    if (task.runFunc)
        task.runFunc(task.taskId);
    else if (m_taskRunFunc)
        m_taskRunFunc(task.taskId);

    SPDLOG_INFO("task(taskID:{}) will be running", task.taskId);
}

void TaskScheduler::giveUpStrategy(const Task& task)
{
    SPDLOG_INFO("task(taskID:{}) will not be running", task.taskId);
    if (task.endFunc)
    {
        task.endFunc(task.taskId, kLowPriority);     
    }    

    return;
}

void TaskScheduler::highPriorityStrategy(const Task& task)
{
    if (task.taskPriority > m_runningTask.taskPriority)
        interruptStrategy(task);
    else
        giveUpStrategy(task);
}

void TaskScheduler::samePriorityStrategy(const Task& task)
{
    if (task.taskPriority >= m_runningTask.taskPriority)
        interruptStrategy(task);
    else
        giveUpStrategy(task);
}
