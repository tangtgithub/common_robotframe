#ifndef __ROBOTSTATE_COMMON_H
#define __ROBOTSTATE_COMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::RobotState, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::RobotState, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::RobotState, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::RobotState, __VA_ARGS__)


#endif
