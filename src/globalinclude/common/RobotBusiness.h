/** *******************************************************
* @brief        : 机器人业务相关接口基类
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 任何一个机器人只需要继承该类，就可以完成相关业务的实现 
**********************************************************/
#ifndef __ROBOTBUSSINESS_H
#define __ROBOTBUSSINESS_H

#include "commondefine.h"

class RobotBusiness
{
public:
    virtual void finishBussiss() {}

};


#endif

