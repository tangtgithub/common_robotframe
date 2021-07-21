/** *******************************************************
* @brief        :通用能力
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :本模块主要负责复杂业务的中介以及通用的功能
*                本类：复杂业务的中介
*                GlCommonFunc:非业务类的通用功能
**********************************************************/
#ifndef __GENERAL_ABILITY_H_
#define __GENERAL_ABILITY_H_

#include <string>
#include <set>
#include <memory>
#include "GeneralAbilityCallback.h"

class GlCommonFunc;
class GeneralAbility : public GeneralAbilityCallback
{
public:
    GeneralAbility();
    ~GeneralAbility();
    void init() override;
    void start() override;
    void deviceStatusReport(uint8_t deviceStatus) override;
private:
    std::shared_ptr<GlCommonFunc>  m_glCommonFunc;
};



#endif
