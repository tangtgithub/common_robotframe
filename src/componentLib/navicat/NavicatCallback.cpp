#include "NavicatCallback.h"
#include "NavicatCb.h"
#include "navicat_common.h"

NavicatCallback* NavicatCallback::getDeriveInstance()
{
    static NavicatCb ncb;
    return &ncb;
}

