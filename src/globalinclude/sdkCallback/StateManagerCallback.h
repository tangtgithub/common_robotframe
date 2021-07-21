/** *******************************************************
* @brief        :状态的对外接口
* @author       :tangt
* @date         :2020-06-16
* @copyright    :Copyright (C) 2020 - All Rights Reserved
**********************************************************/
#ifndef __STATEMANAGER_CALLBACK_H_
#define __STATEMANAGER_CALLBACK_H_

#include <functional>
#include <vector>


enum class emStateId : int
{
    UnknowStateId,
    AwaitOrderStateId = 3, //待命
    ExcutTaskStateId,      //4,执行任务
    ScanMapStateId,        //5,扫图
    RecordPathStateId,     //6,录制路径
    RelocationStateId,     //7重定位
    AutoChargingStateId,   //8自动充电
    ManualChargingStateId, //9手动充电
    BreakdownStateId,      //10故障状态
    EmergencyButtonStateId,//11急停状态
    FreeNaviStateId = 20,  //20 自由导航状态
};


class RobotState
{
public:
    virtual ~RobotState() {}
    virtual void behavior() = 0;
    virtual emStateId stateId() = 0;  //方便识别这个状态
    virtual void dieBehavior() {}; //状态消除时对应的行为
    virtual void setArgument(void* pArg) { return; }
    virtual void* getArgument() { return nullptr; }
    virtual bool isAwaitOrderState() { return true; }
};


class StateManagerCallback
{
public:
    static StateManagerCallback* getDeriveInstance();
    virtual void init() {}
    virtual void start() {}
    virtual void changeState(emStateId stateId, void* pStateArg = nullptr){}
    virtual void eraseState(emStateId stateId, void* pStateArg = nullptr){}
    virtual void clearAllState() {}
    virtual emStateId getCurStateId() {return emStateId::UnknowStateId;}
    virtual RobotState* getCurState() {return nullptr;}
    virtual uint32_t getStateCount() {return 1;}
    virtual RobotState* getRobotStateById(emStateId stateId) {return nullptr;}
    virtual bool isContainState(RobotState* pState) {return true;}
    virtual bool isContainState(emStateId stateId) {return true;}
};


#endif
