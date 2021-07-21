/** *******************************************************
* @brief        :任务通用-桥接模式中的实现类
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 该类作为任务通用模块中的具体实现类,主要实现移动类型的机器人的任务通用接口
**********************************************************/
#ifndef __MOBILE_ROBOT_H_
#define __MOBILE_ROBOT_H_

#include <sys/time.h>
#include <vector>
#include <mutex>
#include <memory>
#include "TaskCommonCallback.h"
#include "Interface.h"

using std::vector;
using taskcommon::TaskEventCallback;

namespace taskcommon
{

const uint32_t kDefault = 1;
const uint32_t kPause = 2;
const uint32_t kRunning = 3;

class MobileRobot : public taskcommon::Interface
{
public:
    struct RunInfo
    {
        uint32_t taskId;
        uint32_t plrId; //点、线、区域id
        uint32_t type;
    };

    MobileRobot();
    ~MobileRobot()=default;

    /* 创建任务
    * parameter1  TaskBasicInfo 任务基础信息
    */
    void createTask(const TaskBasicInfo& taskBasicInfo) override;

    void cancelTask(uint32_t reason=0) override;
  
    void pauseTask(uint32_t reason=0) override;

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

     void registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb) override;

    /* 或跑一个点，或跑一条路径，或跑一个区域（先在区域内规划路径，再跑这条规划路径）
     * returnValue int 成功返回0 失败返回-1
     * parameter1  uint32_t  或点id，或路径id，或区域id
     * parameter2  uint32_t  1:区域 2：路径 3：点
     */
     int run(uint32_t plrId, uint32_t type) override;
     int runPath(uint32_t id, const std::vector<globalmap::CommonPoint>& pointList) override;
    
    /* 暂停导航
     * returnValue int 成功返回0 失败返回-1
     */
     int pause() override;
    
    /* 继续导航
     * returnValue int 成功返回0 失败返回-1
     */
     int resume() override;

    /* 取消导航
     * returnValue int 成功返回0 失败返回-1
     */
     int cancel() override;

private:
     int runArea(uint32_t areaId);
     int runLine(uint32_t lineId);
     int runPoint(uint32_t pointId);

     uint32_t getRunStatus();
     void  setRunStatus(uint32_t runStatus);
private:
    std::mutex                                      m_callMutex;
    std::vector<taskcommon::RunEndCallback>         m_runEndCbs;
    taskcommon::TaskEntryCallback                   m_taskEntryCb;
    std::map<uint32_t,vector<TaskEventCallback>>    m_taskEventCbs;


    RunInfo                                         m_curRunInfo;

    std::mutex                                      m_runStatusMutex;
    uint32_t                                        m_runStatus;
};

};

#endif
