#include "AlgorithmCallback.h"
#include "AlgorithmManage.h"

using namespace algorithm;

AlgorithmCallback* AlgorithmCallback::getDeriveInstance()
{
    static AlgorithmManage algorithmManage;
    return &algorithmManage;
}
