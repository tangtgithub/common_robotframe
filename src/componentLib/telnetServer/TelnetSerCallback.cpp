#include "TelnetSerCallback.h"
#include "ServerCb.hpp"



TelnetSerCallback* TelnetSerCallback::getDeriveInstance()
{
    static ServerCb scb;
    return &scb;
}
