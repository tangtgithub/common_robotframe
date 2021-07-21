#ifndef __WEBSOCKETSERVERCOMMON_H
#define __WEBSOCKETSERVERCOMMON_H


#include "Spdlogger.hpp"

#define SPDLOG_INFO(...)  LOGINFO(emModuleId::WebsocketS, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::WebsocketS, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::WebsocketS, __VA_ARGS__)
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::WebsocketS, __VA_ARGS__)


const uint32_t kMultiplyNum = 1000;

#define TEST_FLAG


#endif
