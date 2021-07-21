#include "PushRtmp.h"
#include "pushCommon.h"
#include <errno.h>

PushRtmp::PushRtmp()
    :m_hasRtmpThread(false)
    , m_curFrame(nullptr)
    , m_hasFrame(false)
    , m_headerSuccess(false)
    , m_rtmpFailedTimes(0)
    , m_hasInitPushRtmpParam(false)
    , m_startPushRtmp(false)
{
    //注册所有的编解码器
    avcodec_register_all();

    //注册所有的封装器
    av_register_all();

    //注册所有网络协议
    avformat_network_init();
}

PushRtmp::~PushRtmp()
{
    
}

void PushRtmp::startPushRtmp(string url)
{
    if (m_hasRtmpThread)
    {
        SPDLOG_WARN("already has rtmp thread");
        return;
    }
    if (!m_hasInitPushRtmpParam)
    {
        SPDLOG_WARN("please init pushRtmpParam first");
        return;
    }
    usleep(1000 * 200);

    m_startPushRtmp = true;
    m_hasRtmpThread = true;
    m_hasFrame = false;
    m_outUrl = url;
    std::thread rtmpThread(&PushRtmp::pushRtmpThread, this);
    rtmpThread.detach();
}

void PushRtmp::stopPushRtmp()
{
    m_startPushRtmp = false;
}

void PushRtmp::pushRtmpThread()
{
    int ret = 0;
    AVPacket pack;
    timeval startTime;
    timeval endTime;

    while (m_startPushRtmp)
    {
        if (!m_hasFrame)
        {
            sleep(1);
            continue;
        }
        if (!m_headerSuccess)
        {
            SPDLOG_INFO("write header to cloud srs");
            m_framecnt = 0;
            if (writeHeader2cloudRtmp())
            {
                sleep(5);
                continue;
            }
        }
        gettimeofday(&startTime, nullptr);
        memset(&pack, 0, sizeof(pack));

        {
            std::unique_lock<std::mutex> m_uniqueLock_curFrame(m_mutex_curFrame);
            ret = avcodec_send_frame(m_vc, m_curFrame);
            if (ret != 0)
            {
                SPDLOG_ERROR("avcodec_send_frame error");
                continue;
            }

            ret = avcodec_receive_packet(m_vc, &pack);
            if (ret != 0 || pack.size > 0)
            {
                //cout << "*" << pack.size << flush;
            }
            else
            {
                SPDLOG_ERROR("avcodec_receive_packet error");
                continue;
            }
        }

        pack.stream_index = m_out_stream->index;
        AVRational time_base = m_out_stream->time_base;//{ 1, 1000 };
        AVRational r_framerate1 = m_vc->framerate;
        AVRational time_base_q;
        time_base_q.num = 1;
        time_base_q.den = AV_TIME_BASE;

        m_framecnt++;

        int64_t calc_duration = static_cast<int64_t>((AV_TIME_BASE) * (1 / av_q2d(r_framerate1)));  //内部时间戳
        pack.pts = av_rescale_q(m_framecnt * calc_duration, time_base_q, time_base);
        pack.dts = pack.pts;
        pack.duration = av_rescale_q(calc_duration, time_base_q, time_base);
        pack.pos = -1;


        if (av_interleaved_write_frame(m_octx, &pack) < 0) //写入图像到视频
        {
            SPDLOG_WARN("av_interleaved_write_frame failed");
            if (m_rtmpFailedTimes++ < 5 * m_fps)
                continue;
            else
            {
                m_rtmpFailedTimes = 0;
                avio_close(m_octx->pb);
                if (m_vc)
                    avcodec_free_context(&m_vc);
                m_headerSuccess = false;
            }
        }
        //else
            //spdlog::debug("av_interleaved_write_frame success");
        gettimeofday(&endTime, nullptr);

        long long temp = (endTime.tv_sec - startTime.tv_sec) * 1000 * 1000 + endTime.tv_usec - startTime.tv_usec;
        long long sleepTime = 1000 * 1000 / m_fps - temp;
        if (sleepTime > 0 && sleepTime < 1000 * 1000 / m_fps)
        {
            usleep(static_cast<unsigned int>(sleepTime));
        }

    }
    m_hasRtmpThread = false;
}

int PushRtmp::pushRtmp(AVFrame* frame)
{
    if (frame == nullptr)
    {
        SPDLOG_WARN("frame is null");
        return FAILURE;
    }

    std::unique_lock<std::mutex> m_uniqueLock_curFrame(m_mutex_curFrame);
    m_curFrame = frame;
    m_hasFrame = true;

    return SUCCESS;
}

int PushRtmp::writeHeader2cloudRtmp()
{
    int ret = 0;
    m_headerSuccess = false;

    ///4 初始化编码上下文
    //a 找到编码器
    
    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec)
    {
        SPDLOG_ERROR("canot find h264 encoder!");
        SPDLOG_ERROR("errno message:{}", strerror(errno));
        return FAILURE;
    }
    //b 创建编码器上下文
    m_vc = avcodec_alloc_context3(codec);
    if (!m_vc)
    {
        SPDLOG_ERROR("avcodec_alloc_context3 failed!");
        return FAILURE;
    }
    //c 配置编码器参数
    m_vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //全局参数
    m_vc->codec_id = codec->id;
    m_vc->thread_count = 8;

    //vc->bit_rate = 50 * 1024*8; //压缩后每秒视频的bit位大小 50kB
    //m_vc->bit_rate = 800000;
    m_vc->bit_rate = m_bitRate;
    m_vc->width = m_width;
    m_vc->height = m_height;
    m_vc->time_base = { 1, m_fps };
    m_vc->framerate = { m_fps, 1 };

    AVDictionary* param;
    param = 0;
    av_dict_set(&param, "preset", "superfast", 0);
    av_dict_set(&param, "profile", "high", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);

    //画面组的大小，多少帧一个关键帧
    //m_vc->gop_size = 250;
    m_vc->gop_size = m_fps * 2;
    m_vc->has_b_frames = 0;
    m_vc->max_b_frames = 0;
    m_vc->qmin = 10;
    m_vc->qmax = 30;
    m_vc->pix_fmt = AV_PIX_FMT_YUV420P;

    //d 打开编码器上下文
    ret = avcodec_open2(m_vc, codec, &param);
    if (ret != 0)
    {
        std::string strErrorMassage = ErrorMessage(ret);
        SPDLOG_ERROR("avcodec_open2 failed ret:{}, message:{}!", ret, strErrorMassage);
        return FAILURE;
    }

    //如果是输入文件 flv可以不传，可以从文件中判断。如果是流则必须传
    //创建输出上下文
    ret = avformat_alloc_output_context2(&m_octx, NULL, "flv", m_outUrl.c_str());
    if (ret < 0)
    {
        std::string strErrorMassage = ErrorMessage(ret);
        SPDLOG_WARN("avformat_alloc_output_context2 ret:{}, message:{}!", ret, strErrorMassage);
        return FAILURE;
    }
    SPDLOG_INFO("avformat_alloc_output_context2 success!");

    //为输出上下文添加音视频流（初始化一个音视频流容器）
    m_out_stream = avformat_new_stream(m_octx, codec);
    if (!m_out_stream)
    {
        SPDLOG_INFO("未能成功添加音视频流");
        //ret = AVERROR_UNKNOWN;
        return FAILURE;
    }

    m_out_stream->time_base.num = 1;
    m_out_stream->time_base.den = m_fps;
    m_out_stream->codec = m_vc;


    av_dump_format(m_octx, 0, m_outUrl.c_str(), 1);

    //打开IO
    ret = avio_open(&m_octx->pb, m_outUrl.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        std::string strErrorMassage = ErrorMessage(ret);
        SPDLOG_WARN("avio_open failed ret:{}, message:{}!", ret, strErrorMassage);
        return FAILURE;
    }

    SPDLOG_INFO("avio_open success");

    //写入头部信息
    ret = avformat_write_header(m_octx, 0);
    if (ret < 0)
    {
        std::string strErrorMassage = ErrorMessage(ret);
        SPDLOG_WARN("avformat_write_header ret:{}, message:{}!", ret, strErrorMassage);
        return FAILURE;
    }
    m_headerSuccess = true;
    SPDLOG_INFO("local avformat_write_header success");
    return SUCCESS;
}

int PushRtmp::initPushRtmpParam(int width, int height, int fps, int bitRate)
{
    m_width = width;
    m_height = height;
    m_fps = fps;
    m_bitRate = bitRate;
    m_hasInitPushRtmpParam = true;
    SPDLOG_INFO("width:{},height:{},fps:{},bitRate:{}", width, height, fps, bitRate);

    return SUCCESS;
}

std::string PushRtmp::ErrorMessage(int errNum)
{
    char errorStr[64] = { 0 };
    return av_make_error_string(errorStr, 64, errNum);
}
