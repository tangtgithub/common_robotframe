/** *******************************************************
* @brief        :线程池类
* @author       :tangt
* @date         :2021-02-02
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :对于返回值为void的function，只执行一次
*                对于返回值为int的function，一直执行直至返回值为0
*                对于返回值为bool的function，一直执行直至返回值为true
*                一直执行只是代表该function会被反复执行，其他function仍可正常得到调度
*                todo:目前暂不支持指定执行失败后重新执行的时间
**********************************************************/
#ifndef __THREAD_POOL_NEW_H__
#define __THREAD_POOL_NEW_H__

#include <functional>
#include <deque> 
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <assert.h>
#include <sys/prctl.h>

class ThreadPoolNew
{
public:
    typedef std::function<void()> Task;
    typedef std::function<int()> TaskI;
    typedef std::function<bool()> TaskB;
public:
    //实际会比threadNum多创建一个线程
    explicit ThreadPoolNew(size_t threadNum = 10, std::string threadName = "ThreadPool")
        : m_isRunning(true)
        , m_threadsNum(threadNum)
        , m_threadName(threadName)
        , m_earlyTime(0)
    {
        createThreads();
    }
    ~ThreadPoolNew()
    {
        stop();
    }

public:
    size_t addTask(const Task& task)
    {
        std::unique_lock<std::mutex> lock(m_mutex_msg);
        m_taskQueue.push_back(task);
        m_cvMsg.notify_one();
        return sizeLockless();
    }
    size_t addTaskI(const TaskI& task)
    {
        std::unique_lock<std::mutex> lock(m_mutex_msg);
        m_taskQueueI.push_back(task);
        m_cvMsg.notify_one();
        return sizeLockless();
    }
    size_t addTaskB(const TaskB& task)
    {
        std::unique_lock<std::mutex> lock(m_mutex_msg);
        m_taskQueueB.push_back(task);
        m_cvMsg.notify_one();
        return sizeLockless();
    }

private:
    void createThreads()
    {
        for (size_t i = 0; i < m_threadsNum; ++i)
        {
            std::thread(&ThreadPoolNew::threadFunc, this).detach();
        }
        std::thread(&ThreadPoolNew::cacheHandler, this).detach();
    }

    void stop()
    {
        if (!m_isRunning) {
            return;
        }

        m_isRunning = false;
        m_cvMsg.notify_all();
    }
    void threadFunc()
    {
        prctl(PR_SET_NAME, m_threadName.c_str());
        while (m_isRunning)
        {
            Task task;
            TaskI taskI;
            TaskB taskB;
            take(task, taskI, taskB);
            if (task)
                task();
            else if (taskI)
            {
                if (taskI() == 0)
                    continue;

                std::unique_lock<std::mutex> lock(m_mutex_msg);
                m_taskQueueICache.push_back(taskI);
                if(m_earlyTime == 0)
                    m_earlyTime = time(nullptr);
            }
            else if (taskB)
            {
                if (taskB())
                    continue;
                
                std::unique_lock<std::mutex> lock(m_mutex_msg);
                m_taskQueueBCache.push_back(taskB);
                if (m_earlyTime == 0)
                    m_earlyTime = time(nullptr);
            }
        }
    }
    void take(Task &task, TaskI &taskI, TaskB &taskB)
    {
        std::unique_lock<std::mutex> lock(m_mutex_msg);
        while (sizeLockless()==0 && m_isRunning)
        {
            m_cvMsg.wait(lock);
        }

        if (!m_isRunning) {
            return;
        }

        assert(sizeLockless() != 0);
        if (m_taskQueue.size() != 0)
        {
            task = m_taskQueue.front();
            m_taskQueue.pop_front();
        }
        else if (m_taskQueueI.size() != 0)
        {
            taskI = m_taskQueueI.front();
            m_taskQueueI.pop_front();
        }
        else if (m_taskQueueB.size() != 0)
        {
            taskB = m_taskQueueB.front();
            m_taskQueueB.pop_front();
        }
        
    }
    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex_msg);
        return m_taskQueue.size()+ m_taskQueueI.size()+ m_taskQueueB.size();
    }
    size_t sizeLockless()
    {
        return m_taskQueue.size() + m_taskQueueI.size() + m_taskQueueB.size();
    }
    void cacheHandler()
    {
        prctl(PR_SET_NAME, m_threadName.c_str());
        
        while (1)
        {
            time_t now = time(nullptr);
            if (now - m_earlyTime >= 5 && m_earlyTime != 0)
            {
                std::unique_lock<std::mutex> lock(m_mutex_msg);
                m_taskQueue.insert(m_taskQueue.end(), m_taskQueueCache.begin(), m_taskQueueCache.end());
                m_taskQueueI.insert(m_taskQueueI.end(), m_taskQueueICache.begin(), m_taskQueueICache.end());
                m_taskQueueB.insert(m_taskQueueB.end(), m_taskQueueBCache.begin(), m_taskQueueBCache.end());
                m_taskQueueCache.clear();
                m_taskQueueICache.clear();
                m_taskQueueBCache.clear();
                m_earlyTime = 0;
                m_cvMsg.notify_all();
            }
            else
                std::this_thread::sleep_for(3s);
        }  
    }

private:
    ThreadPoolNew& operator=(const ThreadPoolNew&) = delete;
    ThreadPoolNew(const ThreadPoolNew&) = delete;

private:
    volatile  bool              m_isRunning;
    size_t                      m_threadsNum;
    std::string                 m_threadName;
    //立即执行队列
    std::deque<Task>            m_taskQueue;
    std::deque<TaskI>           m_taskQueueI;
    std::deque<TaskB>           m_taskQueueB;
    //缓存队列，执行失败的先放入缓存队列
    std::deque<Task>            m_taskQueueCache;
    std::deque<TaskI>           m_taskQueueICache;
    std::deque<TaskB>           m_taskQueueBCache;
    time_t                      m_earlyTime;//第一次执行失败的时间

    std::mutex                  m_mutex_msg;
    std::condition_variable     m_cvMsg;
};


#endif
