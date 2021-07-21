#ifndef __NAVICATCB_H
#define __NAVICATCB_H

#include "NavicatCallback.h"

class NavicatImpl;

class NavicatCb : public NavicatCallback
{
public:
    void init() override;
    void start()  override;
private:
    NavicatImpl* m_pNavicatImpl = nullptr;
};



#endif
