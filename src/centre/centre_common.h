#ifndef _CENTRE_COMMON_H_
#define _CENTRE_COMMON_H_

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h> 
#include <netinet/tcp.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Spdlogger.hpp"
#include "commondefine.h"

#ifndef SPDLOG_INFO
#define SPDLOG_DEBUG(...) LOGDEBUG(emModuleId::centre, __VA_ARGS__)
#define SPDLOG_INFO(...)  LOGINFO(emModuleId::centre, __VA_ARGS__)
#define SPDLOG_WARN(...)  LOGWARN(emModuleId::centre, __VA_ARGS__)
#define SPDLOG_ERROR(...) LOGERROR(emModuleId::centre, __VA_ARGS__)
#endif

#endif
