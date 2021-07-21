#ifndef __PUSHRTMPCALLBACK_H
#define __PUSHRTMPCALLBACK_H

#include <string>
#include <unistd.h>

class AVFrame;
class PushRtmpCallback
{
public:
    static PushRtmpCallback* getDeriveInstance();

    /* 初始化推流参数,程序启动时调用
    * returnValue  int         0 表示成功， 其他数字表示失败
    * parameter1  width:视频宽
    * parameter2  height：视频高
    * parameter3  fps：帧率
    * parameter4  bitRate：码率
    * parameter5  videoNUmber  摄像头（视频源）编号,该编号在初始化的时候指定,默认为1
    * eg：100kb则为100*1024*8
    */
    virtual int initPushRtmp(int width, int height, int fps, int bitRate,uint8_t videoNUmber=1) {return 0;}


    /* 开始推流
    * returnValue  int         0 表示成功， 其他数字表示失败
    * parameter1  url  推流地址
    * parameter2  videoNUmber  摄像头（视频源）编号,该编号在初始化的时候指定,默认为1
    */
    virtual int startPushRtmp(const std::string& url,uint8_t videoNUmber=1) {return 0;}

	
    /* 停止推流
    * returnValue  int         0 表示成功， 其他数字表示失败
    * parameter1  videoNUmber  摄像头（视频源）编号,该编号在初始化的时候指定,默认为1
    */
    virtual int stopPushRtmp(uint8_t videoNUmber=1) {return 0;}


    /* 推流
    * returnValue int         0 表示成功， 其他数字表示失败
    * parameter1  videoNUmber  摄像头（视频源）编号,该编号在初始化的时候指定,默认为1
    * parameter2  frame        帧数据
    */
	virtual int pushRtmp(AVFrame *frame,uint8_t videoNUmber=1) {return 0;}
};

#endif
