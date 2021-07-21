#ifndef __ROBOTSTATEMANAGE_H
#define __ROBOTSTATEMANAGE_H


#include "RobotState.h"


class RobotStateManage : public StateManagerCallback
{
public:
    RobotStateManage();
    void init() override;
    void start() override;
    void changeState(emStateId stateId, void* pStateArg = nullptr) override;
    void eraseState(emStateId stateId, void* pStateArg = nullptr) override;
    void clearAllState() override;
    emStateId getCurStateId() override;
    RobotState* getCurState() override;
    uint32_t getStateCount() override;
    RobotState* getRobotStateById(emStateId stateId) override;
    bool isContainState(RobotState* pState) override;
    bool isContainState(emStateId stateId) override;
private:
    void stateBehave() ;//触发当前状态对应的行为
    void changeState(RobotState* state) ;
    void getCurrentStateList(std::string &stateList) ;
private:
    std::list<RobotState*> m_stateList;//状态集
    std::recursive_mutex m_recursiveStateMutex;
    std::map<emStateId, std::string> m_mapDebug;
};

#endif
