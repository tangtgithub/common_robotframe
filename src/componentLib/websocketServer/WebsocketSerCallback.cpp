#include "WebsocketSerCallback.h"
#include "WebsocketSerManage.h"

WebsocketSerCallback* WebsocketSerCallback::getDeriveInstance()
{
    return WebsocketSerManage::getInstance();
}
