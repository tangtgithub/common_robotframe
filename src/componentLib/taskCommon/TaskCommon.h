/** *******************************************************
* @brief        :任务通用-桥接模式中的抽象类
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 该类作为任务通用模块中的抽象类，具体的实现通过 m_pInterface 转调
**********************************************************/

#ifndef __TASK_COMMON_H_
#define __TASK_COMMON_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "TaskCommonCallback.h"
#include "MobileRobot.h"
#include "taskdefine.h"
#include "mapdefine.h"

using namespace task;


namespace taskcommon
{

class TaskCommon : public TaskCommonCallback
{
public:

    TaskCommon();
    ~TaskCommon();

    void init() override;
    void start() override;

    /* 创建任务
    * parameter1  TaskBasicInfo 任务基础信息
    */
    void createTask(const TaskBasicInfo& taskBasicInfo) override;

    /* 取消任务
    */
    void cancelTask(uint32_t reason=0) override;
  
    /* 暂停任务
    */
    void pauseTask(uint32_t reason=0) override;

    /* 继续任务
    */
    void resumeTask(uint32_t reason=0) override;

    /* 点、线、区域导航结果通知回调
    * parameter1  uint32_t 任务id
    * parameter2  uint32_t 点、线、区域id
    * parameter3  uint32_t 导航结果 1：成功 2：失败
    */
    void navicatResCallback(uint32_t taskId, uint32_t rangeId, uint8_t status) override;
    
    
    /* 注册点、路径、区域运行结束时的回调函数
    * parameter1  RunEndCallback 回调函数原型
    */
     void registerRunEndCb(taskcommon::RunEndCallback runEndCb) override;


    /* 注册任务入口回调函数
    * parameter1  TaskEntryCallback 回调函数原型
    */
     void registerTaskEntryCb(taskcommon::TaskEntryCallback taskEntryCb) override;

    /* 注册任务事件回调，如任务暂停、继续和取消等
    * parameter1  TaskEventCallback 回调函数原型
    */
    void registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb) override;

    /* 或跑一个点，或跑一条路径，或跑一个区域（先在区域内规划路径，再跑这条规划路径）
     * returnValue int 成功返回0 失败返回-1
     * parameter1  uint32_t  或点id，或路径id，或区域id
     * parameter2  uint32_t  1:区域 2：路径 3：点
     */
     int run(uint32_t id, uint32_t type) override;
     int runPath(uint32_t id, const std::vector<globalmap::CommonPoint>& pointList) override;
    
    /* 暂停
     * returnValue int 成功返回0 失败返回-1
     */
     int pause() override;
    
    /* 继续
     * returnValue int 成功返回0 失败返回-1
     */
     int resume() override;

    /* 取消
     * returnValue int 成功返回0 失败返回-1
     */
     int cancel() override;

protected:

     taskcommon::InterfacePtr     m_pInterface;

};

};


#endif
