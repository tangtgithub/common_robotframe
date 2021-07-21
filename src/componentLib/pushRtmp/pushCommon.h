#ifndef __PUSHCOMMON_H
#define __PUSHCOMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::PushRtmp, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::PushRtmp, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::PushRtmp, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::PushRtmp, __VA_ARGS__)

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif

#endif
