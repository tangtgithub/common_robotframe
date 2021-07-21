#ifndef __NAVICATCALLBACK_H
#define __NAVICATCALLBACK_H

#include "commondefine.h"

class NavicatCallback
{
public:
    static NavicatCallback* getDeriveInstance();
    virtual void init() {return ;}
    virtual void start() {return ;}
};


#endif
