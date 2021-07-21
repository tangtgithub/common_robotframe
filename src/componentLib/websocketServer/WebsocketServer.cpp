#include "WebsocketServer.h"
#include <sys/prctl.h>
#include <chrono>
#include "websocketServerCommon.h"
#include "UdpServerWs.h"
#include "UdpClientWs.h"

WebsocketServer::WebsocketServer()
    : m_sessionid(0)
{
    m_server.set_access_channels(websocketpp::log::alevel::none);
    m_server.init_asio();

    m_server.set_open_handler(bind(&WebsocketServer::onOpen, this, ::_1));
    m_server.set_close_handler(bind(&WebsocketServer::onClose, this, ::_1));
    m_server.set_message_handler(bind(&WebsocketServer::onMessage, this, ::_1, ::_2));
}

WebsocketServer::~WebsocketServer()
{

}

void WebsocketServer::onOpen(connection_hdl hdl)
{
    ConnectionData data;
    {
        std::lock_guard<std::mutex> guard(m_connectMutex);
        if (m_connections.size() > 10)
        {
            m_server.close(hdl, 0, "timeout");
            SPDLOG_ERROR("too much connection,forbid new connect.close this connection");
            return;
        }

        data.sessionid = ++m_sessionid;
        data.timeLastHeart = time(NULL);
        if (m_sessionid >= 60)
            m_sessionid = 0;

        m_connections[hdl] = data;
        SPDLOG_INFO("connect success,sessionid:{}", m_sessionid);
    }


    if(m_onOpenCb)
    {
        m_onOpenCb(data.sessionid);
    }
}

void WebsocketServer::onClose(connection_hdl hdl)
{
    try {
        std::lock_guard<std::mutex> guard(m_connectMutex);

        ConnectionData data = getDataFromConnHandle(hdl);
        SPDLOG_INFO("close connection success,sessionid:{}", data.sessionid);

        m_connections.erase(hdl);
        if (m_onCloseCb)
            m_onCloseCb(data.sessionid);
    }
    catch (std::invalid_argument &info) {
        SPDLOG_INFO("close connection success");

    }
    catch (...)
    {
        SPDLOG_ERROR("other error");
    }

}

ConnectionData WebsocketServer::getDataFromConnHandle(connection_hdl hdl)
{
    auto it = m_connections.find(hdl);

    if (it == m_connections.end())
    {
        // this connection is not in the list. This really shouldn't happen
        // and probably means something else is wrong.
        throw std::invalid_argument("No data available for session");
    }
    return it->second;
}

connection_hdl WebsocketServer::getHandleFromSessionId(int sessionId)
{
    for (auto it = m_connections.begin(); it != m_connections.end(); it++)
    {
        if (it->second.sessionid == sessionId)
            return it->first;
    }
    throw std::invalid_argument("No data available for session");
}

void WebsocketServer::onMessage(connection_hdl hdl, message_ptr msg)
{
    MsgHandler msgHandler;

    try {
        ConnectionData data;
        {
            std::lock_guard<std::mutex> guard(m_connectMutex);
            data = getDataFromConnHandle(hdl);
        }
        memcpy(&msgHandler, &data, sizeof(ConnectionData));
        msgHandler.msg = msg->get_payload();

        std::lock_guard<std::mutex> guard(m_msgMutex);

        m_msgs.push_back(msgHandler);
        cvMsg.notify_all();
        //Debug << "通知处理线程收到数据\n";
    }
    catch (std::invalid_argument &info) {
        //nonexistent
        SPDLOG_ERROR("warn:receive unexpected info,connection abnormal");
    }
    catch (...)
    {
        SPDLOG_ERROR("other error");
    }


}

int WebsocketServer::startServer()
{

    std::thread recvThread(&WebsocketServer::run, this);
    std::thread monitorThread(&WebsocketServer::monitorHeart, this);
    std::thread handleMsgThread(&WebsocketServer::handleMsg, this);
    std::thread serverIpThread(&WebsocketServer::serverIpBroadcast, this);

    recvThread.detach();
    monitorThread.detach();
    handleMsgThread.detach();
    serverIpThread.detach();
    return SUCCESS;
}

void WebsocketServer::serverIpBroadcast()
{
    prctl(PR_SET_NAME, "serverIpBroadcast");
    UdpClient udpClient;
    UdpServer udpServer;

    udpClient.initClient(kServerBroadcastPort);
    udpServer.initServer(kAppBroadcastPort);

    while (1)
    {
        char buf[100] = { 0 };
        char sendBuf[] = "res_server_ip";

        std::string strIp;
        if (udpServer.readData(buf, sizeof(buf) - 1, strIp))
        {
            continue;
        }
        if (!strncmp(buf, "query_server_ip", sizeof("query_server_ip")))
        {
            udpClient.writeData(sendBuf, static_cast<int>(strlen(sendBuf)), strIp, kServerBroadcastPort);
            SPDLOG_INFO("receive query_server_ip,send res_server_ip");
        }
    }
}



int WebsocketServer::run()
{
    try {
        m_server.set_reuse_addr(true);
        m_server.listen(12236);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        m_server.run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
        return FAILURE;

    }
    catch (...)
    {
        SPDLOG_ERROR("other exception");
        return FAILURE;
    }

    return FAILURE;
}

void WebsocketServer::monitorHeart()
{
    prctl(PR_SET_NAME, "websocketSerMonitorHeart");
    time_t now;
    while (1)
    {
        now = time(nullptr);
        {
            std::lock_guard<std::mutex> guard(m_connectMutex);
            for (auto it = m_connections.begin(); it != m_connections.end();)
            {
                if (now - it->second.timeLastHeart > 20)//超时未收到心跳，关闭连接
                {
                    SPDLOG_ERROR("heartbeat timeout,close connection,sessionid:{}", it->second.sessionid);
                    m_server.pause_reading(it->first);
                    m_server.close(it->first, 0, "timeout");
                    if (m_onCloseCb)
                        m_onCloseCb(it->second.sessionid);
                    m_connections.erase(it++);
                }
                else
                    it++;
            }
        }
        sleep(5);
    }
}


void WebsocketServer::handleMsg()
{
    prctl(PR_SET_NAME, "websocketSerMsgHandle");
    while (1)
    {
        MsgHandler msgHandler;
        {
            std::unique_lock<std::mutex> lock(m_msgMutex);
            while (m_msgs.empty())
            {
                cvMsg.wait(lock);
            }
            auto x = m_msgs.front();
            msgHandler.connData.sessionid = x.connData.sessionid;
            msgHandler.msg = x.msg;
            m_msgs.pop_front();
        }
        if (m_onMsgCb)
            m_onMsgCb(msgHandler.connData.sessionid, msgHandler.msg);

    }
}

/*
* 功能:发送消息给ws客户端
* 参数:
* 备注:如果需要打印发送的消息请填正确填写type，否则可不传递type值
* 示例:sendMsg(1000,"hello",11011)//打印发送的消息
* 示例:sendMsg(1000,"hello")//不打印发送的消息
*/
void WebsocketServer::sendMsg(int sessionId, std::string& res, int type)
{
    int idAct = sessionId / kMultiplyNum;
    connection_hdl hdl;

    
    SPDLOG_DEBUG("res to app/screen,msg={}", res);
    if (type != 0)
    {
        if (res.size() < 700)
        {
            SPDLOG_INFO("res to app/screen,msg={}", res);
        }
        else
        {
            SPDLOG_INFO("res to app/screen,msg= The first 300 characters: {}", res.substr(0, 300));
            SPDLOG_INFO("res to app/screen,msg= The last  300 characters: {}", res.substr(res.size() - 301, 300));
        }
    }

    try {
        std::lock_guard<std::mutex> guard(m_connectMutex);
        hdl = getHandleFromSessionId(idAct);
        m_server.send(hdl, res, websocketpp::frame::opcode::value::text);
    }
    catch (std::invalid_argument& info) {
        SPDLOG_ERROR("unrecognized sessionId,sessionId{} type {},msg{}", sessionId, type, info.what());
    }
    catch (std::exception& e)
    {
        SPDLOG_ERROR("websocketpp-send:{}", e.what());
    }
    catch (...)
    {
        SPDLOG_ERROR("websocketpp-send error");
    }
}

void WebsocketServer::sendMsg2AllSessions(std::string& res)
{
    SPDLOG_DEBUG("res to app/screen,msg={}", res);
    try {
        std::lock_guard<std::mutex> guard(m_connectMutex);
        for (const auto& kv : m_connections)
        {
            m_server.send(kv.first, res, websocketpp::frame::opcode::value::text);
        }
    }
    catch (std::exception& e)
    {
        SPDLOG_ERROR("websocketpp-send:{}", e.what());
    }
    catch (...)
    {
        SPDLOG_ERROR("websocketpp-send error");
    }
}

