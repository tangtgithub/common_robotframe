#ifndef __WEBSOCKET_SERVER_H
#define __WEBSOCKET_SERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <list>
#include <condition_variable>
#include "basecommon.h"

const int kListenPort = 12236;
const int kServerBroadcastPort = 1222;
const int kAppBroadcastPort = 1223;


typedef websocketpp::server<websocketpp::config::asio> WppServer;
typedef WppServer::message_ptr message_ptr;
typedef std::function<void(int)> OpenSessionCb;
typedef std::function<void(int)> CloseSessionCb;
typedef std::function<void(int,std::string)> MsgSessionCb;
class WebsocketServer;
typedef std::shared_ptr<WebsocketServer> WebsocketServerPtr;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

struct ConnectionData
{
    int sessionid;
    time_t timeLastHeart;
};

struct MsgHandler
{
    ConnectionData connData;
    std::string msg;
};


class WebsocketServer
{
public:
    WebsocketServer();
    ~WebsocketServer();

    int startServer();
    void sendMsg(int sessionId, std::string& res, int type = 0);
    void sendMsg2AllSessions(std::string& res);
    void setOnOpenCb(OpenSessionCb cb) { m_onOpenCb = cb; }
    void setOnCloseCb(CloseSessionCb cb) { m_onCloseCb = cb; }
    void setOnMsgCb(MsgSessionCb cb) { m_onMsgCb = cb; }

private:
    void onOpen(connection_hdl hdl);
    void onClose(connection_hdl hdl);
    void onMessage(connection_hdl hdl, message_ptr msg);
    ConnectionData getDataFromConnHandle(connection_hdl hdl);
    connection_hdl getHandleFromSessionId(int sessionId);
    void handleMsg();
    void monitorHeart();
    void serverIpBroadcast();
    int run();

private:
    typedef std::map<connection_hdl, ConnectionData, std::owner_less<connection_hdl>> ConnectionList;
    typedef std::list<MsgHandler> MsgList;

    ConnectionList m_connections;
    MsgList m_msgs;
    WppServer m_server;
    int m_sessionid;
    std::mutex m_connectMutex;
    std::mutex m_msgMutex;
    std::condition_variable cvMsg;
    std::string m_strStartTime;
    OpenSessionCb m_onOpenCb;
    CloseSessionCb m_onCloseCb;
    MsgSessionCb m_onMsgCb;
};



#endif
