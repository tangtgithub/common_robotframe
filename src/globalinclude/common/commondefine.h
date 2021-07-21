/** *******************************************************
* @brief        : 供所有模块使用的公共头文件定义
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 
**********************************************************/
#ifndef __COMMONDEFINE_H__
#define __COMMONDEFINE_H__

/* ------------------------------------ 头文件模块 ------------------------------------ */
#include "robotConfig.h"
#include <string>
#include <stdint.h>
#include <pthread.h>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <iostream>
#include <stdint.h>
#include <chrono>
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h> 
#include <netinet/tcp.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>


const std::string kSoftwareVersion = "v1.0.0";
const std::string kRobotConfigDir = "../robotconfig/";



#endif
