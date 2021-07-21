/** *******************************************************
* @brief        : 定时触发函数执行
* @author       :tangt
* @date         :2020-09-03
* @copyright    :Copyright (C) 2020 - All Rights Reserved
* @note         :
**********************************************************/
#ifndef __TIMING_RUN_H
#define __TIMING_RUN_H


#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <boost/timer/timer.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/steady_timer.hpp>
#include <functional>
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <sys/prctl.h>

const uint32_t MAX_ASIOTIMER_NUM = 30;

using namespace boost::asio;


class TimingRun : public noncopyable
{
    typedef std::shared_ptr<boost::asio::steady_timer> timer_ptr;
    typedef std::function<void(void)> Fun;
    
public:
    static TimingRun* getInstance()
    {
        static TimingRun asioTimer;
        return &asioTimer;
    }

    void asyncRun()
    {
        m_pThread = std::make_shared<std::thread>([this]{
            prctl(PR_SET_NAME,"asioTimerRun");
            m_pIoService->run();
        });  
    }


    /*common once timer*/
    template <typename Rep, typename Period, typename Func, typename ...Args>
    int onceTiming(std::chrono::duration<Rep, Period> &&timeout, Func &&func, Args &&...args)
    {
        auto pTimer = std::make_shared<boost::asio::steady_timer>(*m_pIoService, timeout);
        
        auto call_obj = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);

        pTimer->async_wait([call_obj, pTimer](const boost::system::error_code &){
                    call_obj();
                });
        return 0;
    }
   
    /*A once timer in seconds*/
    template <typename Func, typename ...Args>
    int onceTiming(uint32_t secs, Func &&func, Args &&...args)
    {
        auto pTimer = std::make_shared<boost::asio::steady_timer>(*m_pIoService, std::chrono::seconds(secs));
        
        auto call_obj = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);

        pTimer->async_wait([call_obj, pTimer](const boost::system::error_code &){
                    call_obj();
                });
        return 0;
    }
    
    /*A once timer in milliseconds*/
    template <typename Func, typename ...Args>
    int onceTimingWithMsec(uint32_t msecs, Func &&func, Args &&...args)
    {
        auto pTimer = std::make_shared<boost::asio::steady_timer>(*m_pIoService, std::chrono::milliseconds(msecs));
        
        auto call_obj = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);

        pTimer->async_wait([call_obj, pTimer](const boost::system::error_code &){
                    call_obj();
                });
        return 0;
    }

    /*A permanent timer in seconds*/
    int permanentTiming(int secs, const Fun& fun)
    {
        uint32_t unCurTimerId = ++m_unCurTimerId;
        
        if ( unCurTimerId > MAX_ASIOTIMER_NUM )
        {
            SPDLOG_ERROR("The number of timers has reached the upper limit");
            return -1;
        }
        
        m_mapTimers[unCurTimerId] = std::make_shared<boost::asio::steady_timer>(*m_pIoService, std::chrono::seconds(secs));
        m_mapTimers[unCurTimerId]->async_wait([=](const boost::system::error_code &){
                    cycleTiming(1000*secs,fun,unCurTimerId);
                    });
        return 0;
    }

    /*A permanent timer in milliseconds*/
    int  permanentTimingWithMsec(int msecs, const Fun& fun)
    {
        uint32_t unCurTimerId = ++m_unCurTimerId;
        if ( unCurTimerId > MAX_ASIOTIMER_NUM )
        {
            SPDLOG_ERROR("The number of timers has reached the upper limit");
            return -1;
        }
        m_mapTimers[unCurTimerId] = std::make_shared<boost::asio::steady_timer>(*m_pIoService, std::chrono::milliseconds(msecs));
        m_mapTimers[unCurTimerId]->async_wait([=](const boost::system::error_code &){
                    cycleTiming(msecs,fun,unCurTimerId);
                    });
        return 0;
    }
  
private:
    TimingRun() : m_pIoService(new boost::asio::io_service), m_pWork(new boost::asio::io_service::work(*m_pIoService))
    {
        asyncRun(); 
    }
    ~TimingRun()
    {
        m_pIoService->stop();
        if ( m_pThread->joinable() )
        {
            m_pThread->join();
        }
    }

    void cycleTiming(int msecs, const Fun& fun, uint32_t timerId)
    {
        //std::cout << timerId << " " << msecs << std::endl;
        fun();
        m_mapTimers[timerId]->expires_from_now(std::chrono::milliseconds(msecs)); 
        m_mapTimers[timerId]->async_wait([=](const boost::system::error_code &){
                    cycleTiming(msecs,fun,timerId);
                });
    }

private:
    typedef std::shared_ptr<boost::asio::io_service> io_service_ptr;
    typedef std::shared_ptr<boost::asio::io_service::work> work_ptr;
    typedef std::shared_ptr<std::thread> thread_ptr;

    io_service_ptr          m_pIoService;
    thread_ptr              m_pThread;
    work_ptr                m_pWork;
    std::map<uint32_t,timer_ptr> m_mapTimers;
    uint32_t                m_unCurTimerId = 0;                     

};


#endif
