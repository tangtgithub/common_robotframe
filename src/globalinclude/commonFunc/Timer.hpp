/** *******************************************************
* @brief        : 定时器相关的封装
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 
**********************************************************/
#ifndef __COMMONTIMER_H
#define __COMMONTIMER_H 

#include <chrono>
#include <mutex>

using namespace std;
using namespace std::chrono;

namespace MyTimer{

class Timer {
public:
	//  Timer的时间单位：秒  毫秒  纳秒
    static constexpr double SECONDS = 1e-3;    
    static constexpr double MILLISECONDS = 1.0;
    
    Timer(double scale = SECONDS) :  m_bStarted(false),m_bPaused(false),pause_ms(0), m_dScale(scale) { };   // 默认构造，单位为秒
    virtual ~Timer() {}
    
    /*
     * @brief: 获取当前系统时间的时间戳，单位秒
     */
    static time_t getSystemCurrentTimeStamp()
    {
        time_point<system_clock, seconds> tp = time_point_cast<seconds>(system_clock::now());
        auto tmp = duration_cast<seconds>(tp.time_since_epoch());
        return  tmp.count();
    }


    void start()
    {
        std::lock_guard<std::mutex> lck(m_mutex);
        if ( m_bStarted == true)
        {
            return;
        }
        m_bStarted = true;
        start_t = std::chrono::high_resolution_clock::now();
        pause_ms = std::chrono::duration<double, std::milli>(0);
    
    }

    void pause()
    {
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            if ( m_bStarted == false)
            {
                return;
            }
        }

        std::lock_guard<std::mutex> lck(m_mutePause);
        m_bPaused = true;
        pause_t = std::chrono::high_resolution_clock::now();
    
    }
    
    void continues()
    {
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            if (!m_bStarted)
            {
                return ;
            }
        }

        {
            std::lock_guard<std::mutex> lck(m_mutePause);
            if ( m_bPaused != true )
            {
                return;
            }
            m_bPaused = false;
            std::chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();

            pause_ms += (end_t - pause_t);
        }
        

    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_bStarted = false;
        }
        
        {
            std::lock_guard<std::mutex> lck(m_mutePause);
            if ( m_bPaused == true )
            {
                std::chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();
                pause_ms += (end_t - pause_t);
                m_bPaused = false;
            }
        }
    }

    double stopWithSecondsFilterPause()
    {
        double seconds = 0;
       
        {
            std::lock_guard<std::mutex> lck(m_mutePause);
            if ( m_bPaused == true )
            {
                std::chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();
                pause_ms += (end_t - pause_t);
            }
            seconds = getSecondsFilterPause();
            m_bPaused = false;
        }
        
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_bStarted = false;
        }
        return seconds;
    }

    double getTime()
    {
        std::chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed_ms;
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            if (!m_bStarted)
            {
                return 0;
            }
            elapsed_ms = end_t - start_t;
        }
        return elapsed_ms.count()*m_dScale;
    }

    double getSecondsFilterPause()
    {
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            if (!m_bStarted)
            {
                return 0;
            }
        }
        return (getTime()-pause_ms.count()*m_dScale); 
    }

    bool getRunStatus()
    {
        std::lock_guard<std::mutex> lck(m_mutex);
        return m_bStarted;
    }
private:
	// 采用高分辨率时钟，定义了一个时间点类型 
    bool                                           m_bStarted;
    bool                                           m_bPaused;
    std::chrono::duration<double, std::milli>      pause_ms;
    double                                         m_dScale;
    std::chrono::high_resolution_clock::time_point start_t;
    std::chrono::high_resolution_clock::time_point pause_t;
    std::mutex                                     m_mutex;
    std::mutex                                     m_mutePause;

};

};

#endif
