/** *******************************************************
* @brief        :获取/转换各种格式化时间
* @author       :tangt
* @date         :2020-09-03
* @copyright    :Copyright (C) 2020 - All Rights Reserved
* @note         :
**********************************************************/

#ifndef __GET_FORMAT_TIME_H__
#define __GET_FORMAT_TIME_H__

#include <time.h>
#include <sys/time.h>
#include <string>
#include <chrono>
#include "SystemCmd.hpp"

namespace COM_FUNC {

    struct Time
    {
        uint32_t year;
        uint32_t mon;
        uint32_t day;
        uint32_t hour;
        uint32_t min;
        uint32_t sec;
    };

    inline bool get_format_time(char* p_time, int len, const char* p_format)
    {
        if (p_time == nullptr || p_format == nullptr)
            return false;

        struct tm timeinfo;
        timeval ts;
        char time_ms[5] = { 0 };

        gettimeofday(&ts, NULL);

        localtime_r(&ts.tv_sec, &timeinfo);

        if (!strncmp(p_format, "yyyyMMdd", strlen(p_format)))
            strftime(p_time, len, "%Y%m%d", &timeinfo);
        else if (!strncmp(p_format, "yyyyMMdd HHmmss", strlen(p_format)))
            strftime(p_time, len, "%Y%m%d %H%M%S", &timeinfo);
        else if (!strncmp(p_format, "yyyy-MM-dd HH:mm:ss", strlen(p_format)))
            strftime(p_time, len, "%Y-%m-%d %H:%M:%S", &timeinfo);
        else if (!strncmp(p_format, "yyyy-MM-dd HH:mm:ss.mmm", strlen(p_format)))
        {
            strftime(p_time, len, "%Y-%m-%d %H:%M:%S", &timeinfo);
            sprintf(time_ms, ".%03lu", ts.tv_usec / 1000);
            strcat(p_time, time_ms);
        }
        else
            return false;

        return true;
    }
    inline bool get_format_time(std::string& time, const char* p_format)
    {
        int len = static_cast<int>(strlen(p_format));
        char* temp = new char[len + 1];
        bool ret = get_format_time(temp, len, p_format);
        time = temp;
        delete[]temp;

        return ret;
    }

    inline bool switch_format_time(std::string& formatTime, time_t time, const char* p_format)
    {
        struct tm timeinfo;

        char timeTemp[50] = { 0 };

        localtime_r(&time, &timeinfo);

        if (!strncmp(p_format, "yyyyMMdd", strlen(p_format)))
            strftime(timeTemp, sizeof(timeTemp), "%Y%m%d", &timeinfo);
        else if (!strncmp(p_format, "HH:mm:ss", strlen(p_format)))
            strftime(timeTemp, sizeof(timeTemp), "%H:%M:%S", &timeinfo);
        else if (!strncmp(p_format, "yyyy-MM-dd", strlen(p_format)))
            strftime(timeTemp, sizeof(timeTemp), "%Y-%m-%d", &timeinfo);
        else if (!strncmp(p_format, "yyyyMMdd HHmmss", strlen(p_format)))
            strftime(timeTemp, sizeof(timeTemp), "%Y%m%d %H%M%S", &timeinfo);
        else if (!strncmp(p_format, "yyyy-MM-dd HH:mm:ss", strlen(p_format)))
            strftime(timeTemp, sizeof(timeTemp), "%Y-%m-%d %H:%M:%S", &timeinfo);
        else
        {
            return false;
        }

        formatTime = timeTemp;
        return true;
    }
    inline std::string switch_format_time(time_t time, const char* p_format)
    {
        string temp;
        switch_format_time(temp, time, p_format);
        return temp;
    }

    inline std::string getCurrentTime()
    {
        auto tp = std::chrono::system_clock::now();
        std::time_t curTime = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        return switch_format_time(curTime,"yyyy-MM-dd HH:mm:ss"); 
    }
    
    inline std::string getCurrentDate()
    {
#if 0
        std::string time = getCurrentTime();
        std::string::size_type pos = time.find(" ");
        if ( pos != std::string::npos )
        {
            time = time.substr(0,pos);
        }
        return time;
#endif
        auto tp = std::chrono::system_clock::now();
        std::time_t curTime = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        return switch_format_time(curTime,"yyyy-MM-dd"); 
    }
    inline std::string getCurrentTime_()
    {
#if 0
        std::string time = getCurrentTime();
        std::string::size_type pos = time.find(" ");
        if ( pos != std::string::npos )
        {
            time = time.substr(pos+1,time.size()-pos-1);
        }
        return time;
#endif
        auto tp = std::chrono::system_clock::now();
        std::time_t curTime = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        return switch_format_time(curTime,"HH:mm:ss"); 
    }

    inline void getCurrentTime(Time &time)
    {
        struct tm *info;
        auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        info = localtime( &tt );

        time.year = info->tm_year+1900;
        time.mon = info->tm_mon+1;
        time.day = info->tm_mday;
        time.hour = info->tm_hour;
        time.min = info->tm_min;
        time.sec = info->tm_sec;
    }

    inline void setCurrentTime(const Time &time, const std::string strPasswd)
    {

        std::string strSetNptCmd = R"(echo ")" + strPasswd + R"(" | sudo -S timedatectl set-ntp false )"; 
        SystemCmd::executeSystemCmd(strSetNptCmd);
        
        std::string strDisableTimesync = R"(echo ")" + strPasswd + R"(" | sudo -S systemctl disable systemd-timesyncd.service )"; 
        SystemCmd::executeSystemCmd(strDisableTimesync);

        std::string strDeleteNtpdate = R"(echo ")" + strPasswd + R"(" | sudo -S /etc/network/if-up.d/ntpdate stop )"; 
        SystemCmd::executeSystemCmd(strDeleteNtpdate);

        //std::string strDeleteNtp = R"(echo ")" + strPasswd + R"(" | sudo -S rm -f /etc/init.d/ntp )"; 
        //SystemCmd::executeSystemCmd(strDeleteNtp);

        std::string strTime;
        std::string strYear = std::to_string(time.year);
        std::string strMon = std::to_string(time.mon);
        std::string strday = std::to_string(time.day);
        std::string strHour = std::to_string(time.hour);
        std::string strMin = std::to_string(time.min);
        std::string strSec = std::to_string(time.sec);

        strTime = strYear + "-" + strMon + "-" + strday + " " + strHour + ":" + strMin + ":" + strSec;

        std::string cmd = R"(echo ")" + strPasswd + R"(" | sudo -S  date -s ")" + strTime + R"(")";

        SystemCmd::executeSystemCmd(cmd);
    
        std::string strHw = R"(echo ")" + strPasswd + R"(" | sudo -S hwclock --systohc )"; 
        SystemCmd::executeSystemCmd(strHw);
    }

}



#endif
