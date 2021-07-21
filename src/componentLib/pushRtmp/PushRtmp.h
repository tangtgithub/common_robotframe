#ifndef __PUSH_RTMP_H_
#define __PUSH_RTMP_H_

#include <sys/time.h>
#include "commondefine.h"
#include "pushCommon.h"

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
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>

#ifdef __cplusplus
}
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

class PushRtmp
{
public:
    PushRtmp();
    ~PushRtmp();

    void startPushRtmp(string url);
    void stopPushRtmp();
    int initPushRtmpParam(int width, int height, int fps, int bitRate);
    int pushRtmp(AVFrame* frame);

private:
    void pushRtmpThread();
    int writeHeader2cloudRtmp();
    std::string ErrorMessage(int errNum);

private:
    bool m_hasRtmpThread;
    string m_outUrl;
    AVFrame* m_curFrame;
    std::mutex m_mutex_curFrame;
    volatile bool m_hasFrame;
    bool m_headerSuccess;
    int m_rtmpFailedTimes;
    int m_width;
    int m_height;
    int m_fps;
    int m_bitRate;
    AVStream* m_out_stream;
    AVFormatContext* m_octx;
    AVCodecContext* m_vc;
    int m_framecnt;
    int m_hasInitPushRtmpParam;
    volatile bool m_startPushRtmp;
};


#endif
