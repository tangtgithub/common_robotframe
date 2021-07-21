#ifndef __WEBSOCKETSER_CALLBACK_H__
#define __WEBSOCKETSER_CALLBACK_H__
#include "commondefine.h"
#include "MsgCallback.h"
#include "json/json.h"

class WebsocketSerCallback
{
public:
    static WebsocketSerCallback* getDeriveInstance();
    
    virtual void init() {}
    virtual void start() {}
    /*
    * 功能:注册消息回调,其实是观察者模式的应用，这个接口其实就是注册观察者
    * 参数:cmds:需要单独处理的cmd的集合
    * 参数:p_msgHandler:cmds的处理类。一个cmd可以对应对个处理类.
    */
    virtual void registerCmd(CmdObserver cmdObserver) { return; }

};

#endif
