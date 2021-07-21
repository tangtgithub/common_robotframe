#ifndef __ALGORITHMCOMMON_H
#define __ALGORITHMCOMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Algorithm, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Algorithm, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Algorithm, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Algorithm, __VA_ARGS__)


#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif


#endif
