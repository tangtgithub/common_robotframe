#ifndef __HTTPCOMMON_H
#define __HTTPCOMMON_H

#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Http, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Http, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Http, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Http, __VA_ARGS__)


#define STRNCPY(strDest, strSrc) strncpy(strDest, strSrc.asString().c_str(), sizeof(strDest) - 1);

#endif
