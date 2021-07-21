#include "GeneralAbility.h"
#include <sys/prctl.h>
#include "GeneralAbilityCommon.h"
#include "GlCommonFunc.h"
#include "CentralPMCallback.h"



GeneralAbility::GeneralAbility()
{
    m_glCommonFunc = std::make_shared<GlCommonFunc>();
}

GeneralAbility::~GeneralAbility()
{
}

void GeneralAbility::init()
{

}

void GeneralAbility::start() 
{
    SPDLOG_INFO("general ability start!!!");

}

void GeneralAbility::deviceStatusReport(uint8_t deviceStatus)
{

}
