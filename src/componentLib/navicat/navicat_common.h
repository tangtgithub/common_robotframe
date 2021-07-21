#ifndef __NAVICATCOMMON_H
#define __NAVICATCOMMON_H


#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Navicat, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Navicat, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Navicat, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Navicat, __VA_ARGS__)




#endif
