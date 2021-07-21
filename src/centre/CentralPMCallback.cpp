#include "CentralPMCallback.h"
#include "Centremanage.h"

CentralPMCallback* CentralPMCallback::getDeriveInstance()
{
    return CentreManage::getInstance();
}


