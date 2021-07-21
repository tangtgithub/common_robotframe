#ifndef __GUARD_H
#define __GUARD_H


#include <functional>

 
class Guard
{
    typedef std::function<void(void)> CallbackFun;
public:
    Guard(CallbackFun fun) : m_callbackFun(fun)
    {
    
    }
    ~Guard()
    {
        if ( m_callbackFun )
        {
            m_callbackFun();
        }
    }

private:
    CallbackFun m_callbackFun;
};


#endif

