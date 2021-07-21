#ifndef __ROBOTSTATE_H
#define __ROBOTSTATE_H

#include "StateManagerCallback.h"
#include <atomic>
#include <map>
#include <list>
#include <vector>
#include <mutex>


class AwaitOrderState : public RobotState  //待命状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
};

class ExcutTaskState : public RobotState  //执行任务状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
};


class ScanMapState : public RobotState  //扫图状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
};


class RelocationState : public RobotState  //重定位状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
};

class ManualChargingState : public RobotState  //手动充电状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual bool isAwaitOrderState() override { return false; }
    virtual void dieBehavior() override;
};

class BreakdownState : public RobotState  //故障状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
    
};


class FreeNaviState : public RobotState  //自由导航状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
};


class EmergencyButtonState : public RobotState  //急停状态
{
public:
    virtual void behavior() override;
    virtual emStateId stateId() override;
    virtual void dieBehavior() override;
    virtual bool isAwaitOrderState() override { return false; }
};


class StateFactory  //状态工厂
{
public:
    static StateFactory* getInstance()
    {
        static StateFactory stateFactory;
        return &stateFactory;
    }
    RobotState* createState(emStateId stateId);
private:
    StateFactory() = default;
    std::map<emStateId, RobotState*> m_mapStatePool;
    std::mutex m_mutex;
};

#endif
