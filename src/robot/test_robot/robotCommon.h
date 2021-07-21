#ifndef __TESTROBOTTEST_H_
#define __TESTROBOTTEST_H_

#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::commonRobo, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::commonRobo, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::commonRobo, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::commonRobo, __VA_ARGS__)


#endif
