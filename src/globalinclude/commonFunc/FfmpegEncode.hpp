#ifndef FFMPEG_ENCODE_H
#define FFMPEG_ENCODE_H

#include "Spdlogger.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

#ifdef __cplusplus
}
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

class FfmpegEncoder
{
public:
    FfmpegEncoder()
    {
    }

    ~FfmpegEncoder()
    {
        if (m_pFrame) {
            av_frame_free(&m_pFrame);
        }

        if (m_pSws_ctx) {
            sws_freeContext(m_pSws_ctx);
        }
    }

    bool Initizational(int iWidth, int iHeight)
    {
        m_pFrame = av_frame_alloc();
        if (!m_pFrame) {
            LOGWARN(emModuleId::Common,"could not allocate video frame");
            return false;
        }
        m_pFrame->format = AV_PIX_FMT_YUV420P;
        m_pFrame->width = iWidth;
        m_pFrame->height = iHeight;
        m_pFrame->pts = 0;
        m_iWidth = iWidth;
        m_iHeight = iHeight;
        
        int ret = av_frame_get_buffer(m_pFrame, 32);
        if (ret < 0) {
            LOGWARN(emModuleId::Common, "could not allocate the video frame data");
            return false;
        }

        // 初始化格式转换器
        m_pSws_ctx = sws_getCachedContext(m_pSws_ctx, 
            m_iWidth, m_iHeight, AV_PIX_FMT_BGR24, // AV_PIX_FMT_BGR32(邵银)
            m_iWidth, m_iHeight, AV_PIX_FMT_YUV420P, 
            SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!m_pSws_ctx) {
            LOGWARN(emModuleId::Common, "sws_ctx init failed");
            return false;
        }
        return true;
    }

    AVFrame* Encode(const stream_data& stream, uint8_t* data)
    {
        if (av_frame_make_writable(m_pFrame) < 0) { // 检测帧对象是否可读
            LOGWARN(emModuleId::Common, "data frame discontinuity");
            return nullptr;
        }

        if (stream.width != static_cast<uint32_t>(m_iWidth) || 
            stream.height != static_cast<uint32_t>(m_pFrame->height)) {
            LOGWARN(emModuleId::Common, "data error, init width: {}, init height:{}, data width: {}, data height:{}", m_pFrame->width, m_pFrame->height, stream.width, stream.height);
            return nullptr;
        }

        // 输入的数据结构
        uint8_t* in_data[AV_NUM_DATA_POINTERS] = {0};
        in_data[0] = data;

        // 一行（宽）数据的字节数
        m_iWidthlen = stream.step;

        // 像素格式转换
        sws_scale(m_pSws_ctx, in_data, &m_iWidthlen, 0, m_iHeight, m_pFrame->data, m_pFrame->linesize);
        if(m_pFrame->data == nullptr) {
            LOGWARN(emModuleId::Common, "ffmpeg encode failed");
            return nullptr;
        }
        
        m_pFrame->pts = m_iPts++;
        return m_pFrame;
    }

private:
    AVFrame* m_pFrame = nullptr;
    struct SwsContext* m_pSws_ctx = nullptr; // 格式转换器
    int m_iPts = 0;                          // 时间戳对象
    int m_iWidth = 0;                        // 数据宽度
    int m_iHeight = 0;                       // 数据高度
    int m_iWidthlen = 0;
};

#endif  /* FFMPEG_ENCODE_HPP */
