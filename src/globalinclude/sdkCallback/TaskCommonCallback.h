#ifndef __TASKCOMMON_CALLBACK_H
#define __TASKCOMMON_CALLBACK_H

#include <string>
#include <vector>
#include <unistd.h>
#include "commondefine.h"
#include "taskdefine.h"
#include "mapdefine.h"


namespace taskcommon{

/**************点、线、区域******************/
const uint32_t kArea = 1;
const uint32_t kLine = 2;
const uint32_t kPoint = 3;

/**************任务事件******************/
const uint32_t kTaskPauseEvent = 1;
const uint32_t kTaskResumeEvent = 2;
const uint32_t kTaskCancelEvent = 3;


/**************运行结果*****************/
const uint32_t kRunSuccess = 1;
const uint32_t kRunFail = 2;
//plrId:点、线、区域id，status:运行结果 1：成功 2：失败
typedef std::function<void (uint32_t plrId, uint8_t status)> RunEndCallback;

//任务入口回调
typedef std::function<void (uint32_t taskId)> TaskEntryCallback;

//任务事件回调:取消、暂停、继续
typedef std::function<void (uint32_t)> TaskEventCallback;


};

class TaskCommonCallback
{
public:
    static TaskCommonCallback* getDeriveInstance();
    

    virtual void init() {  }
    virtual void start() {  }

    /* 创建任务
    * parameter1  TASK_BASIC_INFO 任务基础信息
    */
    virtual void createTask(const task::TaskBasicInfo& taskBasicInfo) {}
    
    /* 取消任务
    * parameter1  TASK_BASIC_INFO 任务基础信息
    */
    virtual void cancelTask(uint32_t reason=0) {}
  
    /* 暂停任务
    * parameter1  TASK_BASIC_INFO 任务基础信息
    */
    virtual void pauseTask(uint32_t reason=0) {}

    /* 继续任务
    * parameter1  TASK_BASIC_INFO 任务基础信息
    */
    virtual void resumeTask(uint32_t reason=0) {}

    /* 点、线、区域导航结果通知回调
    * parameter1  uint32_t 任务id
    * parameter2  uint32_t 点、线、区域id
    * parameter3  uint32_t 导航结果 1：成功 2：失败
    */
    virtual void navicatResCallback(uint32_t taskId, uint32_t rangeId, uint8_t status) {}

    /* 注册点、路径、区域运行结束时的回调函数
    * parameter1  RunEndCallback 回调函数原型
    */
    virtual void registerRunEndCb(taskcommon::RunEndCallback rangeEndCb) {}



    /* 注册任务入口回调函数
    * parameter1  TaskEntryCallback 回调函数原型
    */
    virtual void registerTaskEntryCb(taskcommon::TaskEntryCallback taskEntryCb) {}


    /* 注册任务事件回调，如任务暂停、继续和取消等
    * parameter1  TaskEventCallback 回调函数原型
    */
    virtual void registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb) {}

    /* 或跑一个点，或跑一条路径，或跑一个区域（先在区域内规划路径，再跑这条规划路径）
     * returnValue int 成功返回0 失败返回-1
     * parameter1  uint32_t  或点id，或路径id，或区域id
     * parameter2  uint32_t  1:区域 2：路径 3：点
     */
    virtual int run(uint32_t id, uint32_t type) {return 0;}
    virtual int runPath(uint32_t id, const std::vector<globalmap::CommonPoint>& pointList) {return 0;}
    
    /* 暂停
     * returnValue int 成功返回0 失败返回-1
     */
    virtual int pause() {return 0;}
    
    /* 继续
     * returnValue int 成功返回0 失败返回-1
     */
    virtual int resume() {return 0;}
     
    /* 取消
     * returnValue int 成功返回0 失败返回-1
     */
    virtual int cancel() {return 0;}

};

#endif


