
#include "RobotState.h"
#include "CentralPMCallback.h"
#include "robotstate_common.h"


/***********************************************************
 * 待命状态
 **********************************************************/
void AwaitOrderState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::AwaitOrderStateId));
}
emStateId AwaitOrderState::stateId()
{
    return emStateId::AwaitOrderStateId;
}


/***********************************************************
 * 执行任务状态
 **********************************************************/
void ExcutTaskState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::ExcutTaskStateId));

}

emStateId ExcutTaskState::stateId()
{
    return emStateId::ExcutTaskStateId;
}
void ExcutTaskState::dieBehavior()
{
}

/***********************************************************
 * 扫图状态
 **********************************************************/
void ScanMapState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::ScanMapStateId));
}
emStateId ScanMapState::stateId()
{
    return emStateId::ScanMapStateId;
}

void ScanMapState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}


/***********************************************************
 * 重定位状态
 **********************************************************/
void RelocationState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::RelocationStateId));

}
emStateId RelocationState::stateId()
{
    return emStateId::RelocationStateId;
}

void RelocationState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}


/***********************************************************
 * 手动充电状态
 **********************************************************/
void ManualChargingState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::ManualChargingStateId));
}
emStateId ManualChargingState::stateId()
{
    return emStateId::ManualChargingStateId;
}
void ManualChargingState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}

/***********************************************************
 * 故障状态
 **********************************************************/
void BreakdownState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::BreakdownStateId));
}
emStateId BreakdownState::stateId()
{
    return emStateId::BreakdownStateId;
}
void BreakdownState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}


/***********************************************************
 * 自由导航状态
 **********************************************************/
void FreeNaviState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::FreeNaviStateId));
}
emStateId FreeNaviState::stateId()
{
    return emStateId::FreeNaviStateId;
}
void FreeNaviState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}

/***********************************************************
 * 急停状态
 **********************************************************/
void EmergencyButtonState::behavior()
{
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(emStateId::EmergencyButtonStateId));
}

emStateId EmergencyButtonState::stateId()
{
    return emStateId::EmergencyButtonStateId;
}

void EmergencyButtonState::dieBehavior()
{
    emStateId curStateId =  CentralPMCallback::getRobotStateManageHandler()->getCurStateId();
    CentralPMCallback::getGeneralAbilityHandler()->deviceStatusReport(static_cast<uint8_t>(curStateId));
}

/***********************************************************
 * 状态工厂，所有状态均从这里获取
 **********************************************************/
RobotState* StateFactory::createState(emStateId stateId)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    std::map<emStateId, RobotState*>::iterator iter = m_mapStatePool.find(stateId);
    if (iter != m_mapStatePool.end())
    {
        return iter->second;
    }
    else
    {
        switch (stateId)
        {
        case emStateId::AwaitOrderStateId:
        {
            RobotState* pState = new AwaitOrderState();
            m_mapStatePool[emStateId::AwaitOrderStateId] = pState;
            return pState;
        }
        case emStateId::ExcutTaskStateId:
        {
            RobotState* pState = new ExcutTaskState();
            m_mapStatePool[emStateId::ExcutTaskStateId] = pState;
            return pState;
        }
        case emStateId::ScanMapStateId:
        {
            RobotState* pState = new ScanMapState();
            m_mapStatePool[emStateId::ScanMapStateId] = pState;
            return pState;
        }
        case emStateId::RelocationStateId:
        {
            RobotState* pState = new RelocationState();
            m_mapStatePool[emStateId::RelocationStateId] = pState;
            return pState;
        }
        case emStateId::ManualChargingStateId:
        {
            RobotState* pState = new ManualChargingState();
            m_mapStatePool[emStateId::ManualChargingStateId] = pState;
            return pState;
        }
        case emStateId::BreakdownStateId:
        {
            RobotState* pState = new BreakdownState();
            m_mapStatePool[emStateId::BreakdownStateId] = pState;
            return pState;
        }
        case emStateId::EmergencyButtonStateId:
        {
            RobotState* pState = new EmergencyButtonState();
            m_mapStatePool[emStateId::EmergencyButtonStateId] = pState;
            return pState;
        }
        case emStateId::FreeNaviStateId:
        {
            RobotState* pState = new FreeNaviState();
            m_mapStatePool[emStateId::FreeNaviStateId] = pState;
            return pState;
        }
        default:
        {
            SPDLOG_ERROR("unknow state = {}", static_cast<int>(stateId));
            return nullptr;
        }
        }
    }
}
