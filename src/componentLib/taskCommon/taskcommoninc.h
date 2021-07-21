#ifndef __TASKCOMMON_COMMON_H
#define __TASKCOMMON_COMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::TaskCommon, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::TaskCommon, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::TaskCommon, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::TaskCommon, __VA_ARGS__)


#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif

#endif
