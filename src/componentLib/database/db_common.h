#ifndef __DB_COMMON_H
#define __DB_COMMON_H

#include <iostream>
#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Database, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Database, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Database, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Database, __VA_ARGS__)

#endif

