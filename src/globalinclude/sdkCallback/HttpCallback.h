#ifndef __HTTP_CALLBACK_H__
#define __HTTP_CALLBACK_H__

#include <string>
#include <functional>
#include <vector>
#include "json/json.h"

class HttpCallback
{
public:
    static HttpCallback* getDeriveInstance();

    virtual void init() {}
    /*-------------------------------------  启动http模块  -------------------------------------*/
    virtual void start() {}

    /*------------------------------------- http通信模块 -------------------------------------*/

    /**
    * @brief HTTP POST 请求，带token
    *
    * @param strUrl          输入参数, 请求的Url地址, 格式如: http://www.baidu.com
    *
    * @param strPost         输入参数, 请求参数, 格式如: para1=val1&para2=val2 或者是 json 字符串
    *
    * @param tokenInfo       输入参数, 验证信息 token, timeStamp
    *
    * @param strResponse     输出参数, 请求返回的结果
    *
    * @param conn_timeout_ms 输入参数, 连接服务器超时时间
    *
    * @param resp_timeout_ms 输入参数, 服务器响应超时时间
    *
    * @return 返回是否Post成功, 成功 0, 失败其他数字
    */
    virtual int PostData(const std::string& strUrl,
        const std::string& strPost,
        const std::pair<std::string, std::string>& tokenInfo,
        std::string& strResponse,
        int conn_timeout_ms = 5000,
        int resp_timeout_ms = 5000) {
        return 0;
    }

    /**
    * @brief HTTP POST 请求，不带 token
    *
    * @param strUrl          输入参数, 请求的Url地址, 格式如: http://www.baidu.com
    *
    * @param strPost         输入参数, 请求参数, 格式如: para1=val1&para2=val2 或者是 json 字符串
    *
    * @param strResponse     输出参数, 请求返回的结果
    *
    * @param conn_timeout_ms 输入参数, 连接服务器超时时间
    *
    * @param resp_timeout_ms 输入参数, 服务器响应超时时间
    *
    * @return 返回是否Post成功, 成功 0, 失败其他数字
    */
    virtual int PostData(const std::string& strUrl,
        const std::string& strPost,
        std::string& strResponse,
        const std::vector<std::string>& vecHeaderData = std::vector<std::string>(),
        int conn_timeout_ms = 5000,
        int resp_timeout_ms = 5000) {
        return 0;
    }

    /**
    * @brief HTTP GET请求
    *
    * @param strUrl          输入参数,请求的Url地址, 格式如: http://www.baidu.com
    *
    * @param strResponse     输出参数, 请求返回的结果
    *
    * @param tokenInfo       输入参数, 验证信息 token, timeStamp
    *
    * @param conn_timeout_ms 输入参数, 连接服务器超时时间
    *
    * @param resp_timeout_ms 输入参数, 服务器响应超时时间
    *
    * @return 返回是否Get成功, 成功 0, 失败其他数字
    */
    virtual int GetData(const std::string& strUrl,
        std::string& strResponse,
        const std::pair<std::string, std::string>& tokenInfo,
        int conn_timeout_ms = 8000,
        int resp_timeout_ms = 10000) {
        return 0;
    }

    /**
    * @brief HTTP GET请求
    *
    * @param strUrl          输入参数,请求的Url地址, 格式如: http://www.baidu.com
    *
    * @param strResponse     输出参数, 请求返回的结果
    *
    * @param conn_timeout_ms 输入参数, 连接服务器超时时间
    *
    * @param resp_timeout_ms 输入参数, 服务器响应超时时间
    *
    * @return 返回是否Get成功, 成功 0, 失败其他数字
    */
    virtual int GetData(const std::string& strUrl,
        std::string& strResponse,
        const std::vector<std::string>& vecHeaderData = std::vector<std::string>(),
        int conn_timeout_ms = 8000,
        int resp_timeout_ms = 10000) {
        return 0;
    }

};


#endif
