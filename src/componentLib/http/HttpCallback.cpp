#include "HttpCallback.h"
#include "HttpCb.h"

HttpCallback* HttpCallback::getDeriveInstance()
{
    static HttpCb httpcb;
    return &httpcb;
}
