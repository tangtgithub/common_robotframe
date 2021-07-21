#ifndef __GENERAL_ABILITY_COMMON_H
#define __GENERAL_ABILITY_COMMON_H

#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::GeneralAbi, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::GeneralAbi, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::GeneralAbi, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::GeneralAbi, __VA_ARGS__)

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif



#endif
