/** *******************************************************
* @brief        :任务调度的对外接口
* @author       :tangt
* @date         :2021-06-16
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :目前只实现了立即执行任务和不执行任务的调度，定时任务待补充
**********************************************************/
#ifndef __TASK_SCHEDULER_H_
#define __TASK_SCHEDULER_H_

#include "TaskSchedulerCallback.h"
#include <set>
#include <mutex>

class TaskScheduler :public TaskSchedulerCallback
{
public:
    TaskScheduler();
    ~TaskScheduler();

    /**
    * @param strategy     输入参数, 冲突策略。1-新任务打断当前任务；2-有任务正在执行时，放弃新任务；3-高优先级抢占，否则放弃;4-高/同优先级抢占，否则放弃
    */
    void init(int strategy) override;

    /**
    * @brief 启动任务调度程序
    * @return 返回none
    */
    void start() override;

    /**
    * @brief 添加任务至任务调度队列
    * @param task     输入参数, 待调度任务
    * @return 返回none
    */
    void addTask(const Task& task)override;
    /**
    * @brief 将任务从任务调度队列移除
    * @param task     输入参数, 待移除任务
    * @return 返回none
    */
    void deleteTask(uint32_t taskId) override;
    /**
    * @brief 设置通用的任务处理函数，单个任务可以在任务里自定义自己的特殊处理函数
    * @param func     输入参数, 任务处理函数
    * @return 返回none
    */
    void setHandler(TaskRunFunc func) override { m_taskRunFunc = func; };

    void test()override;


private:
    //添加立即执行任务
    void addImmediatelyTask(const Task &task);
    //添加定时任务
    void addTimedTask(const Task& task);
    //添加不执行任务
    void addNoexecTask(const Task& task);
    //任务冲突策略
    void conflictStrategy(const Task& task);
    //新任务打断当前任务
    void interruptStrategy(const Task& task);
    //有任务正在执行时，放弃新任务
    void giveUpStrategy(const Task& task);
    //高优先级抢占，否则放弃
    void highPriorityStrategy(const Task& task);
    //高/同优先级抢占，否则放弃
    void samePriorityStrategy(const Task& task);


private:
    TaskRunFunc m_taskRunFunc;
    std::set<Task> m_taskList;
    bool m_isRunning;
    Task m_runningTask;
    int m_strategy;
    std::mutex m_taskMutex;
};


#endif
