#ifndef __TASK_SCHE_COMMON_H
#define __TASK_SCHE_COMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::TaskSchedu, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::TaskSchedu, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::TaskSchedu, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::TaskSchedu, __VA_ARGS__)

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif


#endif
