#ifndef PUSHRTMP_H 
#define PUSHRTMP_H __attribute__((deprecated))
#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "commondefine.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
}
#endif

#include <fstream>
//#include <jsoncpp/json/json.h>
#include "json/json.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <functional>
#include <ifaddrs.h>
#include <unistd.h>
#include <chrono>
using namespace std;

#define CAMERA_DELAY_TIMES      200

VideoStream* VideoStream::m_video = new VideoStream();
std::mutex  VideoStream::s_mutex;


class VideoStream
{
public:


    int StartUp(const std::string& src_url, const std::string& dest_url)
    {
        Init();
        m_statusRun = 1;

        std::ifstream infile;
        string rtmp_ip("");
        string temp("");
        infile.open("../robotconfig/server.txt", ios::in);
        if (infile)
        {
            infile >> temp;
            infile >> temp;
            infile >> temp;
            infile >> rtmp_ip;

        }
        infile.close();
        string rtmp_url = dest_url;
        size_t pos = rtmp_url.find("//");
        size_t pos1 = rtmp_url.find("/live");
        rtmp_url.replace(pos + 2, pos1 - pos - 2, rtmp_ip);
        m_strDestYunTaiLocal = src_url;
        m_strRtmpDest = rtmp_url;
        pthread_create(&m_beginProcess, NULL, WorkStream, static_cast<void*>(getInstance()));

        return 0;
    }
    bool StopPush()
    {

        m_statusRun = 2;
        return true;
    }
    static VideoStream* getInstance()
    {
        return m_video;
    }
private:
    void Init()
    {

        m_pMatContextIn = nullptr;
        m_pMatContextOut = nullptr;
        m_pOutputMat = nullptr;
        m_pMatContextDelayIn = nullptr;
        m_pMatContextDelayOut = nullptr;
        m_pOutputDelayMat = nullptr;

        m_strRtmpDest = "";
        m_strDestYunTaiLocal = "";
        m_statusRun = 0;

    }
    // bool ReadConfig();
    int PushStream()
    {

        /*if (CameraPushCmd::CameraPushCmd_NULL == s_eCameraPushing)
        {
            return -1;
        }*/

        AVPacket            packet;
        size_t              i = 0;
        size_t              nIndexVideo = -1;
        int                 nIndexFrame = 0;
        const char* pStrSrc = nullptr;
        const char* pStrDest = m_strRtmpDest.c_str();

        av_register_all();
        avformat_network_init();

        pStrSrc = m_strDestYunTaiLocal.c_str();
        spdlog::info("Push Camera Stream : pStrSrc {} -> pStrDest {}", pStrSrc, pStrDest);
        cout << "Push Camera Stream: " << pStrSrc << " -> " << pStrDest << endl;
        AVDictionary* options = NULL;
        av_dict_set(&options, "rtsp_transport", "tcp", 0);
        if ((m_nRet = avformat_open_input(&m_pMatContextIn, pStrSrc, nullptr, &options)) < 0)
        {
            spdlog::error("Can't open source.");
            return -1;
        }
        if ((m_nRet = avformat_find_stream_info(m_pMatContextIn, nullptr)) < 0)
        {
            spdlog::error("Failed to retrieve input stream information.");
            return -1;
        }
        for (i = 0; i < m_pMatContextIn->nb_streams; i++)
        {
            if (m_pMatContextIn->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                nIndexVideo = i;
                break;
            }
        }

        av_dump_format(m_pMatContextIn, 0, pStrSrc, 0);
        avformat_alloc_output_context2(&m_pMatContextOut, nullptr, "flv", pStrDest); // rtmp

        if (!m_pMatContextOut)
        {
            spdlog::error("Could not create output context: {}", pStrDest);
            m_nRet = AVERROR_UNKNOWN;
            return -1;
        }

        m_pOutputMat = m_pMatContextOut->oformat;
        for (i = 0; i < m_pMatContextIn->nb_streams; i++)
        {
            // Create an output stream based on the input stream
            AVStream* in_stream = m_pMatContextIn->streams[i];
            AVStream* out_stream = avformat_new_stream(m_pMatContextOut, in_stream->codec->codec);
            if (!out_stream)
            {
                spdlog::error("Allocating output stream failed.");
                m_nRet = AVERROR_UNKNOWN;
                return -1;
            }
            // Copy AVCodecContext Setting
            m_nRet = avcodec_copy_context(out_stream->codec, in_stream->codec);
            if (m_nRet < 0)
            {
                spdlog::error("Copy context form input to output stream codec context failed.");
                return -1;
            }

            // Add global header information
            out_stream->codec->codec_tag = 0;
            if (m_pMatContextOut->oformat->flags & AVFMT_GLOBALHEADER)
            {
                out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            }
        }

        // Output format information of the output RTMP file
        av_dump_format(m_pMatContextOut, 0, pStrDest, 1);

        // Open the output URL
        if (!(m_pOutputMat->flags & AVFMT_NOFILE))
        {
            VideoStream::s_mutex.lock();
            m_nRet = avio_open2(&m_pMatContextOut->pb, pStrDest, AVIO_FLAG_WRITE, nullptr, nullptr);
            VideoStream::s_mutex.unlock();
            if (m_nRet < 0)
            {
                spdlog::error("Could't open output URL:{}", pStrDest);
                return -1;
            }
        }

        // Write file header
        m_nRet = avformat_write_header(m_pMatContextOut, nullptr);
        if (m_nRet < 0)
        {
            spdlog::error("Error occurred when opening output URL.");
            return -1;
        }
        // Write video and audio frames
        while (m_statusRun == 1)
        {
            AVStream* in_stream, * out_stream;

            // get a AVPacket
            m_nRet = av_read_frame(m_pMatContextIn, &packet);

            if (m_nRet < 0)
            {
                break;
            }
            // set PTS
            if (packet.pts == AV_NOPTS_VALUE)
            {
                // Time between the original two frames
                int64_t calc_duration = static_cast<int64_t>((AV_TIME_BASE) / av_q2d(m_pMatContextIn->streams[nIndexVideo]->r_frame_rate));

                // Set the time parameter of one frame
                AVRational time_base1 = m_pMatContextIn->streams[nIndexVideo]->time_base;

                packet.pts = static_cast<int64_t>(nIndexFrame * calc_duration / static_cast<int64_t>((av_q2d(time_base1) * AV_TIME_BASE)));
                packet.dts = packet.pts;
                //packet.duration = static_cast<int64_t>( calc_duration / static_cast<int64_t>((av_q2d(time_base1)*AV_TIME_BASE)));
                packet.duration = static_cast<int>(calc_duration / static_cast<int>((av_q2d(time_base1) * AV_TIME_BASE)));

            }

            in_stream = m_pMatContextIn->streams[packet.stream_index];
            out_stream = m_pMatContextOut->streams[packet.stream_index];

            // Conversion PTS/DTS
            packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base, static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base, static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            //packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
            packet.duration = static_cast<int>(av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base));
            packet.pos = -1;

            //Print to Screen
            if (packet.stream_index == static_cast<int>(nIndexVideo))
            {
                nIndexFrame++;
            }

            m_nRet = av_interleaved_write_frame(m_pMatContextOut, &packet);
            /*
            if (m_nRet < 0)
            {
                cout << "Error muxing packet.";
                break;
            }
            */
            av_free_packet(&packet);
            //av_packet_unref(&packet);
        }
        av_write_trailer(m_pMatContextOut);
        return 0;
    }
    void CleanUp()
    {
        if (nullptr != m_pThreadPush)
        {
            m_pThreadPush->join();
            m_pThreadPush.reset();
        }
        if (nullptr != m_pThreadCheckingCMD)
        {
            m_pThreadCheckingCMD->join();
            m_pThreadCheckingCMD.reset();
        }
    }
    static void* WorkStream(void* ptr)
    {
        VideoStream* video_obj = static_cast<VideoStream*>(ptr);
        while (video_obj->m_statusRun == 1)
        {
            usleep(500000);
            spdlog::info("WorkStream:begin push!");
            video_obj->PushStream();
            video_obj->CloseVideo();
        }
        return static_cast<void*>(NULL);
    }
    void WorkCheckPushingCamStreamType();
    void CloseVideo()
    {
        avformat_close_input(&m_pMatContextIn);
        if (m_pMatContextOut && !(m_pOutputMat->flags & AVFMT_NOFILE))
        {
            avio_close(m_pMatContextOut->pb);
        }
        avformat_free_context(m_pMatContextOut);
        avformat_free_context(m_pMatContextIn);

        m_pMatContextIn = nullptr;
        m_pMatContextOut = nullptr;
        if (m_nRet < 0 && AVERROR_EOF != m_nRet)
        {
            spdlog::error("Error occurred.");
        }
        return;
    }
    VideoStream()
    {

        //StartUp();
    }
    ~VideoStream()
    {
        if (m_video != NULL)
        {
            delete m_video;
        }

    }

private:
    static VideoStream* m_video;
    std::string                     m_strRtmpDest;
    std::string                     m_strDestYunTaiLocal;
    pthread_t						m_beginProcess;
    static std::mutex               s_mutex;
    AVFormatContext* m_pMatContextIn;
    AVFormatContext* m_pMatContextOut;
    AVOutputFormat* m_pOutputMat;
    AVFormatContext* m_pMatContextDelayIn;
    AVFormatContext* m_pMatContextDelayOut;
    AVOutputFormat* m_pOutputDelayMat;
    int                             m_nRet;
    int                             m_nDelayRet;
    int								m_statusRun; //1:开始推流 2.停止推流 3.网络异常
    bool                            m_bPush;
    std::queue<AVPacket>            m_queAVPacket;
    std::unique_ptr<std::thread>    m_pThreadCheckingCMD;
    std::unique_ptr<std::thread>    m_pThreadPush;
    std::unique_ptr<std::thread>    m_pThreadPushLocalDelay;

};
#endif //PUSHRTMP_H

