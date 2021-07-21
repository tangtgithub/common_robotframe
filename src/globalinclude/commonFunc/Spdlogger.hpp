/** *******************************************************
* @brief        : spdlog的封装
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 实现功能： 1、 日志中带模块名
*                            2、可以在线设置日志等级
**********************************************************/

#ifndef __SPDLOGGER_H
#define __SPDLOGGER_H
#include <stdio.h>
#include <iostream>
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "commondefine.h"

#ifdef FILELOG
#include "spdlog/sinks/rotating_file_sink.h"
#else
#include "spdlog/sinks/stdout_color_sinks.h"
#endif

#include <map>
#include <string>
#include <memory>
#include <stdarg.h>
#include <stdio.h>



static const int MODULE_NUM = 29;

enum class emModuleId
{
    Main,
    Common,
    Http,
    WebsocketS,
    Database,
    Mqtt,
    Navicat,
    PushRtmp,
    centre,
    Algorithm,
    GeneralAbi,
    TaskSchedu,
    Telnet,
    TaskCommon,
    RobotState,
    TestRobot,
    Test
};

static const std::string MODULE_NAME[MODULE_NUM] = {
    "Main",
    "Common",
    "Http",
    "WebsocketS",
    "Database",
    "Mqtt",
    "Navicat",
    "PushRtmp",
    "centre",
    "Algorithm",
    "GeneralAbi",
    "TaskSchedu",
    "Telnet",
    "TaskCommon",
    "RobotState",
    "TestRobot",
    "Test"
};


using namespace std;
class Spdlogger
{
public:
    static Spdlogger* getInstance()
    {
        static Spdlogger logger;
        return &logger;
    }

    std::shared_ptr<spdlog::logger> getModuleLogger(emModuleId id)
    {
        string moduleName = MODULE_NAME[static_cast<int>(id)];
        if (m_mapLogger.find(id) == m_mapLogger.end())
        {
            printf("**********please register moudle %s first**********\n", moduleName.c_str());
            SPDLOG_LOGGER_CRITICAL(m_mapLogger[emModuleId::Main], "**********please register moudle {} first**********", moduleName);

            //exit(1);
        }
        return m_mapLogger[id];
    }

    void SetLogLevel(int logLevel)
    {
        for (int i = 0; i < MODULE_NUM; i++)
            setLogLevel(static_cast<emModuleId>(i), logLevel);
    }
    void setLogLevel(emModuleId id, int level)
    {
        m_moduleLogSwitch[id] = level;
    }

    int getLogLevel(emModuleId id)
    {
        if (m_moduleLogSwitch.find(id) == m_moduleLogSwitch.end())
        {
            return SPDLOG_LEVEL_OFF;
        }
        else
        {
            return m_moduleLogSwitch[id];
        }
    }
    //仅供telnet调试时查看各模块的ID
    std::string getDescribe()
    {
        std::string temp;
        for (int i = 0; i < MODULE_NUM; i++)
        {
            temp += MODULE_NAME[i] + "-" + std::to_string(i) + "\n";
        }
        temp += "0-TRACE;1-DEBUG;2-INFO;3-WARNING;4-ERROR;5-CRITICAL;6-OFF\n";
        return temp;
    }

private:
    void registerModule(emModuleId moduleId)
    {
        std::string moduleName = MODULE_NAME[static_cast<int>(moduleId)];
        m_mapLogger[moduleId] = std::make_shared<spdlog::logger>(moduleName, m_pPublicLogger);
        m_mapLogger[moduleId]->flush_on(spdlog::level::trace);
        m_mapLogger[moduleId]->set_level(spdlog::level::trace);
        spdlog::set_default_logger(m_mapLogger[moduleId]);
    }


private:
    Spdlogger()
    {
#ifdef FILELOG
        m_pPublicLogger = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("./robot_log.txt", 1024 * 1024 * 40, 100);
#else
        m_pPublicLogger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#endif
        for (int i = 0; i < MODULE_NUM; i++)
        {
            registerModule(static_cast<emModuleId>(i));
            m_moduleLogSwitch[static_cast<emModuleId>(i)] = SPDLOG_LEVEL_INFO;
        }
    }
#ifdef FILELOG
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> m_pPublicLogger;
#else
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_pPublicLogger;
#endif
    std::map<emModuleId, std::shared_ptr<spdlog::logger>> m_mapLogger;

    std::map<emModuleId, int> m_moduleLogSwitch;
};

#define SETLEVEL(level) \
do \
{ \
    Spdlogger::getInstance()->SetLogLevel(level) \
} while(0)

#define LOGTRACE(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_TRACE) { \
        SPDLOG_LOGGER_TRACE(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)

#define LOGDEBUG(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_DEBUG) { \
        SPDLOG_LOGGER_DEBUG(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)

#define LOGINFO(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_INFO) { \
        SPDLOG_LOGGER_INFO(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)


#define LOGWARN(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_WARN) { \
        SPDLOG_LOGGER_WARN(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)

#define LOGERROR(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_ERROR) { \
        SPDLOG_LOGGER_ERROR(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)

#define LOGCRITICAL(module,...)  \
do \
{ \
    if (Spdlogger::getInstance()->getLogLevel(module) <= SPDLOG_LEVEL_CRITICAL) { \
        SPDLOG_LOGGER_CRITICAL(Spdlogger::getInstance()->getModuleLogger(module), __VA_ARGS__); \
    } \
} while(0)

#endif
