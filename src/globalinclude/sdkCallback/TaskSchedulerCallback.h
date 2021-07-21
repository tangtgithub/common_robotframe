/** *******************************************************
* @brief        :任务调度的对外接口
* @author       :tangt
* @date         :2021-06-16
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :所有机器人统一的任务调度接口，由任务调度模块选择出应该执行的任务
**********************************************************/
#ifndef __TASK_SCHEDULER_CALLBACK_H_
#define __TASK_SCHEDULER_CALLBACK_H_

#include <functional>
#include <vector>

//调度失败的原因
enum LoseScheduleReason
{
    kLowPriority,           //优先级不够
    kUnexecTask,            //任务不需要执行
    kOutOfRuntime          //超出任务运行时间
};

//调度策略
enum ScheduleStrategy
{
    kNewTaskPreempt = 1, //新任务打断当前任务
    kGiveupNewTask, //有任务正在执行时放弃
    kHighPriorityPreempt, // 高优先级抢占，否则放弃
    kHighOrSamePriorityPreempt, //高/同优先级抢占，否则放弃
};

//任务执行类型
enum TaskExecutType
{
    kImmediatelyTask = 1, //立即执行的任务
    kTimingTask, //定时任务
    kNoExecTask, // 暂不执行的任务
};

class Task;
typedef std::function<void(uint32_t taskId)> TaskRunFunc;
typedef std::function<void(uint32_t taskId,LoseScheduleReason reason)> TaskEndFunc;
class Task
{
public:
    uint32_t taskId;                 //任务ID
    uint8_t executationType;         //执行类型 1：立即执行 2：定时执行 3:不执行
    uint8_t taskPriority;            //优先级 1-极低，2-普通，3-正常，4-紧急
    time_t beginTime;                //任务开始时间
    time_t endTime;                  //任务结束时间
    uint8_t executEndType;           //任务结束方式 0:一直执行，1:其他时间，2:完成执行次数
    uint8_t repeatType;              //重复类型 周或月或年 1：每天 2：每周 3：每个月
    std::vector<int> repeatDate;     //重复日期
    TaskRunFunc runFunc;             //任务执行函数
    TaskEndFunc endFunc;             //如果任务被移除调度队列或者任务到达执行时间需通知订阅者

    Task()
    {
        taskId = 0;
        executationType = 0;
        taskPriority = 2;
        beginTime = 0;
        endTime = 0;
        executEndType = 2;
        repeatType = 0;
    }
    Task(uint32_t id, uint8_t execType, TaskEndFunc func)//一般的立即执行任务或者不执行任务
    {
        taskId = id;
        executationType = execType;
        taskPriority = 2;
        beginTime = 0;
        endTime = 0;
        executEndType = 2;
        repeatType = 0;
        endFunc = func;
    }
    bool operator<(const Task &right)const
    {
        if (this->taskId == right.taskId)
            return false;
        //待完成,临时这样写，因为目前不考虑定时任务
        return this->taskId < right.taskId;
    }
};

class TaskSchedulerCallback
{
public:
    static TaskSchedulerCallback* getDeriveInstance();
    
    /**
    * @param strategy     输入参数, 冲突策略。1-新任务打断当前任务；2-有任务正在执行时，放弃新任务；3-高优先级抢占，否则放弃;4-高/同优先级抢占，否则放弃
    */
    virtual void init(int strategy) {}

    /**
    * @brief 启动任务调度程序
    * @return 返回none
    */
    virtual void start() {}
    
    /**
    * @brief 添加任务至任务调度队列
    * @param task     输入参数, 待调度任务
    * @return 返回none
    */
    virtual void addTask(const Task& task) { return; }
    /**
    * @brief 将任务从任务调度队列移除
    * @param task     输入参数, 待移除任务
    * @return 返回none
    */
    virtual void deleteTask(uint32_t taskId) { return; }
    /**
    * @brief 设置通用的任务处理函数，单个任务可以在任务里自定义自己的特殊处理函数
    * @param func     输入参数, 任务处理函数
    * @return 返回none
    */
    virtual void setHandler(TaskRunFunc func) { return; }
    /**
    * @brief 启动任务调度程序
    * @param strategy     输入参数, 冲突策略。1-新任务打断当前任务；2-有任务正在执行时，放弃新任务；3-高优先级抢占，否则放弃;4-高/同优先级抢占，否则放弃
    * @return 返回none
    */
    virtual void run(int strategy) { return; }

    //测试接口，仅供内部测试
    virtual void test() { return; }
};


#endif
