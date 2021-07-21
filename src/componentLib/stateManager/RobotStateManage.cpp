#include "RobotStateManage.h"
#include <thread>
#include "robotstate_common.h"

using namespace std;



RobotStateManage::RobotStateManage()
{
}
void RobotStateManage::getCurrentStateList(std::string &stateList)
{
    std::string strState = "";
    {
        std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
        for (std::list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
        {
            strState += m_mapDebug[(*iter)->stateId()];
            strState += "  ";
        }
    }
    stateList = strState;
}

void RobotStateManage::init()
{
    m_mapDebug[emStateId::UnknowStateId] = "UnknowState";
    m_mapDebug[emStateId::AwaitOrderStateId] = "AwaitOrderState";
    m_mapDebug[emStateId::ExcutTaskStateId] = "ExcutTaskState";
    m_mapDebug[emStateId::ScanMapStateId] = "ScanMapState";
    m_mapDebug[emStateId::RelocationStateId] = "RelocationState";
    m_mapDebug[emStateId::ManualChargingStateId] = "ManualChargingState";
    m_mapDebug[emStateId::BreakdownStateId] = "BreakdownState";
    m_mapDebug[emStateId::EmergencyButtonStateId] = "EmergencyButtonState";
    m_mapDebug[emStateId::FreeNaviStateId] = "FreeNaviState";

}

void RobotStateManage::start()
{
    SPDLOG_INFO("robot state manager start!!!");
    std::thread execThread([this] {
        while (1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(20));
            std::string strState = "";
            getCurrentStateList(strState);
            SPDLOG_INFO("[Timer printf] current state list::{}", strState);
        }
    });
    execThread.detach();

}

void RobotStateManage::changeState(emStateId stateId, void* pStateArg)
{
    if (isContainState(stateId))
    {
        return;
    }
    RobotState* pState = StateFactory::getInstance()->createState(stateId);
    pState->setArgument(pStateArg);
    changeState(pState);
}

void RobotStateManage::changeState(RobotState* pState)
{
    if (isContainState(pState))
    {
        return;
    }
    else
    {
        if ( !pState->isAwaitOrderState() && pState->stateId() != emStateId::AwaitOrderStateId)
        {
            std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
            for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
            {
                if ((*iter)->stateId() == emStateId::AwaitOrderStateId)
                {
                    m_stateList.erase(iter);
                    break;
                }
            }
        }
        else if ( pState->stateId() == emStateId::AwaitOrderStateId )
        {
            std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
            for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
            {
                if ( !(*iter)->isAwaitOrderState() )
                {
                    return;
                }
            }
        }
        m_stateList.push_back(pState);
    }
    
    std::string strState = "";
    getCurrentStateList(strState);
    SPDLOG_INFO("[State switch] current state list::{}", strState);

    stateBehave();
}


void RobotStateManage::stateBehave()
{
    RobotState* pCurState = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
        if ( m_stateList.empty() )
        {
            return;
        }
        pCurState = m_stateList.back();
    }
    pCurState->behavior();
}


void RobotStateManage::eraseState(emStateId stateId, void* pStateArg )
{
    RobotState* pState = getRobotStateById(stateId);
    if ( nullptr != pState  && nullptr != pStateArg )
    {
        pState->setArgument(pStateArg);
    }
    else if ( nullptr == pState ) //没有这个状态
    {
        return;
    }

    {
        std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
        for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
        {
            if ((*iter)->stateId() == stateId)
            {
                m_stateList.erase(iter);
                break;
            }
        }
    }

    //删除某个状态后，如果机器人当前状态列表是空的，则需要让机器人进入待命状态
    {
        std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
        if ( m_stateList.empty() )
        {
            RobotState* pAwaitOrderState = StateFactory::getInstance()->createState(emStateId::AwaitOrderStateId);
            changeState(pAwaitOrderState);
        }
        else
        {
            std::string strState = "";
            getCurrentStateList(strState);
            SPDLOG_INFO("[State switch] current state list::{}", strState);
        }
    }

    if ( pState != nullptr )//某状态删除时，执行它的遗言
    {
        pState->dieBehavior();
    }
}

void RobotStateManage::clearAllState()
{

    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    m_stateList.clear();
}

emStateId RobotStateManage::getCurStateId()
{
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    if ( m_stateList.empty() )
    {
        return StateFactory::getInstance()->createState(emStateId::AwaitOrderStateId)->stateId(); 
    }
    else
    {
        return m_stateList.back()->stateId();
    }
}

RobotState* RobotStateManage::getCurState()
{
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    if ( m_stateList.empty() )
    {
        return StateFactory::getInstance()->createState(emStateId::AwaitOrderStateId);
    }
    else
    {
        return m_stateList.back();
    }
}

uint32_t RobotStateManage::getStateCount()
{
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    return static_cast<uint32_t>(m_stateList.size());
}

RobotState* RobotStateManage::getRobotStateById(emStateId stateId)
{
    RobotState* pRobotState = nullptr;
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
    {
        if ((*iter)->stateId() == stateId)
        {
            pRobotState = *iter;
            break;
        }
    }
    return pRobotState;
}

bool RobotStateManage::isContainState(RobotState* pState)
{
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
    {
        if ((*iter)->stateId() == pState->stateId())
        {
            return true;
        }
    }
    return false;
}

bool RobotStateManage::isContainState(emStateId stateId)
{
    std::lock_guard<std::recursive_mutex> lck(m_recursiveStateMutex);
    for (list<RobotState*>::iterator iter = m_stateList.begin(); iter != m_stateList.end(); ++iter)
    {
        if ((*iter)->stateId() == stateId)
        {
            return true;
        }
    }
    return false;
}
