
#include "NavicatCb.h"
#include "NavicatImpl.h"
#include "navicat_common.h"

void NavicatCb::init()
{
    m_pNavicatImpl = new NavicatImpl();
    m_pNavicatImpl->init();
}


void NavicatCb::start()
{
    SPDLOG_INFO("navicat start!!!");
    m_pNavicatImpl->start();
}
