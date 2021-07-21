#include "PushRtmpCallback.h"
#include "PushRtmpManage.h"


PushRtmpCallback* PushRtmpCallback::getDeriveInstance()
{
    static PushRtmpManage pushRtmpManage;
    return &pushRtmpManage;
}
