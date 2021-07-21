/** *******************************************************
* @brief        :任务通用-桥接模式中的实现类 基类
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 该类作为任务通用模块中的实现类的基类，具体的实现类需要继承该类
**********************************************************/
#ifndef __COMMON_INTERFACE_H
#define __COMMON_INTERFACE_H

#include <cstdint>
#include <memory>
#include "TaskCommonCallback.h"
#include "taskcommoninc.h"

using namespace task;

namespace taskcommon
{

class Interface
{
public:
    /***********************************通用**********************************************/
    virtual int run(uint32_t id, uint32_t type) {return 0;}
    virtual int pause() {return 0;}
    virtual int resume() {return 0;}
    virtual int cancel() {return 0;}
    virtual void createTask(const TaskBasicInfo& taskBasicInfo){}
    virtual void cancelTask(uint32_t reason=0) {}
    virtual void pauseTask(uint32_t reason=0) {}
    virtual void resumeTask(uint32_t reason=0) {}
    virtual void registerTaskEntryCb(taskcommon::TaskEntryCallback taskEntryCb) {}
    virtual void registerTaskEventCb(uint32_t eventType, taskcommon::TaskEventCallback taskEventCb) {}
    /***********************************************************************************/

    /*********************************移动类机器人**************************************/
    virtual int runPath(uint32_t id, const vector<globalmap::CommonPoint>& pointList) {return 0;}
    virtual void navicatResCallback(uint32_t taskId, uint32_t rangeId, uint8_t status) {}
    virtual void registerRunEndCb(taskcommon::RunEndCallback runEndCb) {}
    /***********************************************************************************/

};

typedef std::shared_ptr<Interface> InterfacePtr;

};
#endif
