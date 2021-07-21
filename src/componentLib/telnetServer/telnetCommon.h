#ifndef __TELNETCOMMON_H
#define __TELNETCOMMON_H

#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Telnet, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Telnet, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Telnet, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Telnet, __VA_ARGS__)

#endif
