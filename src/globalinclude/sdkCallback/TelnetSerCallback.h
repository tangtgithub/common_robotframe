#ifndef __TELNETSERCALLBACK_H
#define __TELNETSERCALLBACK_H

#include "commondefine.h"
#include <memory>

class TelnetSerCallback
{
public:
    static TelnetSerCallback* getDeriveInstance();
    virtual void init(uint16_t port) {}
    virtual void start() {}
};

#endif
