/** *******************************************************
* @brief        : 系统检测相关的封装
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 
**********************************************************/
#ifndef SYSTEMCHECK_H
#define SYSTEMCHECK_H
#include "systemCmd.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <string>


struct MemInfo
{
    uint32_t m_total;//总计物理内存的大小
    uint32_t m_free;//显示还有多少物理内存和交换空间可用使用
    uint32_t m_buffers;//缓存的大小,
    uint32_t m_cached;//缓存的大小,
    uint32_t m_swapCached;//交换内存的大小
    uint32_t m_available;//显示还可以被应用程序使用的物理内存大小
};

struct CpuInfo
{
    char m_name[20];//cpu名字
    uint32_t m_user;//从系统启动开始累积到当前时刻，用户态的CPU时间(单位:jiffies)，不包含nice值为负进程。 1jiffies=0.01秒
    uint32_t m_nice;//从系统启动开始累计到当前时刻，nice值为负的进程所占用的cpu时间不包含
    uint32_t m_system;//从系统启动开始累计到当前时刻，系统调度(系统模式)所占用的cpu时间不包含
    uint32_t m_idle;//从系统启动开始累计到当前时刻，除硬盘IO等待时间外的其他等待时间。这个值是USER_HZ乘上/proc/uptime文件中的第二个条目
    uint32_t m_iowait;//【Linux 2.5.41引入】从系统启动开始累计到当前时刻，IO等待所耗费的cpu时间。这个值是不可靠的
    uint32_t m_irq;//【Linux 2.6.0引入】从系统启动开始累计到当前时刻，硬中断所耗费的时间
    uint32_t m_softirq;//【Linux 2.6.0引入】从系统启动开始累计到当前时刻，软中断所耗费的时间
    bool operator()(const CpuInfo& info)
    {
        if (std::string(this->m_name) == std::string(info.m_name))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class SystemCheck
{
public:
    /*
     * @brief: 获取内存的使用率
     * @return: [0.0,1.0]
     */
    static double getMemUsage()
    {
        MemInfo info{};
        FILE* fpMemInfo = fopen("/proc/meminfo", "r");

        if (fpMemInfo == nullptr)
        {
            std::cerr << "/proc/meminfo file open failed" << std::endl;
            return {};
        }

        uint32_t value;
        char line[1024];
        char name[1024];

        while (fgets(line, sizeof(line) - 1, fpMemInfo))
        {
            sscanf(line, "%s%u", name, &value);

            if (0 == strcmp(name, "MemTotal:"))
            {
                info.m_total = value;
            }
            else if (0 == strcmp(name, "MemFree:"))
            {
                info.m_free = value;
            }
            else if (0 == strcmp(name, "MemAvailable:"))
            {
                info.m_available = value;
            }
            else if (0 == strcmp(name, "Buffers:"))
            {
                info.m_buffers = value;
            }
            else if (0 == strcmp(name, "Cached:"))
            {
                info.m_cached = value;
            }
            else if (0 == strcmp(name, "SwapCached:"))
            {
                info.m_swapCached = value;
            }
        }

        fclose(fpMemInfo);
        double usage = (info.m_total - info.m_free - info.m_cached - info.m_buffers) / static_cast<double>(info.m_total);
        return usage;
    }

    /*
     * @brief: 获取每个内核的温度,真实核数，不是虚拟核
     * @return std::map<uint32_t, float>: 核序号，温度
     */
    static std::map<uint32_t, float> getCpuTemperature()
    {
        std::map<uint32_t, float> cpuTemperature;
        std::string cmdStr = "sensors  |grep Core|awk '{print $3}'";
        std::vector<std::string> lines;
        SystemCmd::executeSystemCmd(cmdStr, lines);

        for (size_t i = 0; i < lines.size(); ++i)
        {
            std::string value = lines[i].erase(0, 1);
            cpuTemperature[static_cast<uint32_t>(i)] = stof(value);
        }

        return cpuTemperature;
    }

    /*
     * @brief: 获取每个cpu的占用率
     *
     * @return std::map<uint32_t, float>: 核序号，利用率
     */
    static std::map<uint32_t, double> getCpuUsage()
    {
        std::map<uint32_t, double> cpus;
        std::vector<CpuInfo> oldCpuInfos = getCpuInfo();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::vector<CpuInfo> preCpuInfos = getCpuInfo();

        if (oldCpuInfos.size() != preCpuInfos.size())
        {
            return {};
        }

        for (size_t i = 0; i < oldCpuInfos.size(); ++i)
        {
            auto it = std::find_if(preCpuInfos.begin(), preCpuInfos.end(), oldCpuInfos[i]);

            if (it != preCpuInfos.end())
            {
                uint32_t cpuNext = 0;

                if (getCpuNameNum(it->m_name) == -1)
                {
                    continue;
                }
                else
                {
                    cpuNext = getCpuNameNum(it->m_name);
                }

                double cpuUsage = calCpuUsage(oldCpuInfos[i], *it);
                //std::cout << "cpuNext: " << cpuNext << " cpuUsage: " << cpuUsage << std::endl;
                cpus.insert(std::pair<uint32_t, double>(cpuNext, cpuUsage));
            }
        }

        return cpus;
    }

    /*
     * @brief: 获取cpu的频率
     *
     */
    static std::map<uint32_t, float> getCpuHz()
    {
        std::map<uint32_t, float> cpuMHzs;
        FILE* fpCpuInfo = fopen("/proc/cpuinfo", "r");

        if (fpCpuInfo == nullptr)
        {
            std::cerr << "/proc/cpuinfo file open failed" << std::endl;
            return {};
        }

        char line[1024];
        char name[512];
        char name1[512];
        uint32_t cpuNum = 0;
        float cpuMHz = 0.0;

        while (fgets(line, sizeof(line) - 1, fpCpuInfo))
        {
            //sscanf(line, "%s :%u", name, &cpuNum);
            //if (std::string(name) == "processor")
            //{
            //    continue;
            //}
            sscanf(line, "%s %s :%f", name, name1, &cpuMHz);
            std::string cpuMHzName = std::string(name) + std::string(name1);

            if (cpuMHzName == "cpuMHz")
            {
                cpuMHzs[cpuNum++] = cpuMHz;
            }
        }

        return cpuMHzs;
    }

    /*
     * @brief: 获取机器磁盘的利用率
     * @param rootDiskName: 磁盘的目录名，默认根目录
     * @return: [0,100]
     */
    static uint32_t getRootDiskUsage(const std::string& rootDiskName = "/dev/sda1")
    {
        std::vector<std::string> lines;
        std::string cmdStr = "df -h";

        SystemCmd::executeSystemCmd(cmdStr, lines);
        char fileSystem[1024];

        for (std::string line : lines)
        {
            sscanf(line.c_str(), "%s", fileSystem);

            if (strcmp(fileSystem, rootDiskName.c_str()) == 0)
            {
                char usage[10];
                char tmp[1024];
                sscanf(line.c_str(), "%s %s %s %s %s", tmp, tmp, tmp, tmp, usage);
                return atoi(usage);
            }
            else
            {
                continue;
            }

        }

        return {};
    }
private:
    /*
     * @brief: 获取cpu的当前信息
     */
    static std::vector<CpuInfo> getCpuInfo()
    {
        std::vector<CpuInfo> cpuInfos;
        FILE* fpCpuInfo = fopen("/proc/stat", "r");

        if (fpCpuInfo == nullptr)
        {
            std::cerr << "/proc/stat file open failed" << std::endl;
            return {};
        }

        char line[1024];

        while (fgets(line, sizeof(line) - 1, fpCpuInfo))
        {
            CpuInfo info{};
            sscanf(line, "%s %u %u %u %u %u %u %u", info.m_name, &info.m_user, &info.m_nice
                   , &info.m_system, &info.m_idle, &info.m_iowait, &info.m_irq, &info.m_softirq);
            std::string cpuName(info.m_name);

            if (cpuName.find("cpu") == cpuName.npos)
            {
                continue;
            }

            cpuInfos.push_back(info);

        }

        return cpuInfos;
    }

    /*
     * @brief: 计算cpu的使用率
     *
     */

    static double calCpuUsage(const CpuInfo& oldInfo, const CpuInfo& preInfo)
    {
        uint32_t oldTotal = oldInfo.m_user + oldInfo.m_nice + oldInfo.m_system + oldInfo.m_idle + oldInfo.m_iowait + oldInfo.m_irq + oldInfo.m_softirq;
        uint32_t oldWork = oldInfo.m_user + oldInfo.m_nice + oldInfo.m_system;

        uint32_t newTotal = preInfo.m_user + preInfo.m_nice + preInfo.m_system + preInfo.m_idle + preInfo.m_iowait + preInfo.m_irq + preInfo.m_softirq;
        uint32_t newWork = preInfo.m_user + preInfo.m_nice + preInfo.m_system;

        uint32_t totalUse = newTotal - oldTotal;
        uint32_t workUse = newWork - oldWork;
        double usage = workUse / static_cast<double>(totalUse);
        return usage;


    }

    /*
     * @breif: 将字符串中数组提取出来
     *
     */
    static int getCpuNameNum(const char* name)
    {
        std::string str(name);
        size_t i = 0;

        for (; i < str.length(); ++i)
        {
            if (isdigit(str[i]))
            {
                break;
            }
        }

        if (i == str.length())
        {
            return -1;
        }

        std::string tmp = str.substr(i, str.length() - i);
        return stoi(tmp);
    }
};

#endif //SYSTEMCHECK_H
