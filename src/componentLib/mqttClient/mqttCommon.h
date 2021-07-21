#ifndef __MQTTCOMMON_H
#define __MQTTCOMMON_H

#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::Mqtt, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::Mqtt, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::Mqtt, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::Mqtt, __VA_ARGS__)



#endif
