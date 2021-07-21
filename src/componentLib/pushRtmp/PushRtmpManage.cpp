#include "PushRtmpManage.h"
#include "pushCommon.h"
#include "PushRtmp.h"

int PushRtmpManage::initPushRtmp(int width, int height, int fps, int bitRate, uint8_t videoNUmber)
{
    if (m_mapPushRtmp.find(videoNUmber) == m_mapPushRtmp.end())
    {
        SPDLOG_INFO("initPushRtmpParam::videoNUmber={}", videoNUmber);
        m_mapPushRtmp[videoNUmber] = std::make_shared<PushRtmp>();
    }
    return m_mapPushRtmp[videoNUmber]->initPushRtmpParam(width, height, fps, bitRate);
}


int PushRtmpManage::startPushRtmp(const std::string& url, uint8_t videoNUmber)
{
    SPDLOG_INFO("startPushRtmp::videoNUmber={}", videoNUmber);
    try
    {
        m_mapPushRtmp.at(videoNUmber)->startPushRtmp(url);
    }
    catch (const std::out_of_range& err)
    {
        SPDLOG_INFO("startPushRtmp::Out of range error:{},videoNUmber={}", err.what(), videoNUmber);
        return -1;
    }
    return 0;
}


int PushRtmpManage::stopPushRtmp(uint8_t videoNUmber)
{
    SPDLOG_INFO("stopPushRtmp::videoNUmber={}", videoNUmber);
    try
    {
        m_mapPushRtmp.at(videoNUmber)->stopPushRtmp();
    }
    catch (const std::out_of_range& err)
    {
        SPDLOG_INFO("stopPushRtmp::Out of range error:{},videoNUmber={}", err.what(), videoNUmber);
        return -1;
    }
    return 0;
}


int PushRtmpManage::pushRtmp(AVFrame* frame, uint8_t videoNUmber)
{
    try
    {
        m_mapPushRtmp.at(videoNUmber)->pushRtmp(frame);
    }
    catch (const std::out_of_range& err)
    {
        SPDLOG_INFO("pushRtmp::Out of range error:{},videoNUmber={}", err.what(), videoNUmber);
        return -1;
    }
    return 0;
}
