#include "Centremanage.h"
#include <sys/prctl.h>
#include <signal.h>
#include "centre_common.h"
#include <iostream>
#include <fstream>
#include <list>
#include <math.h>
#include <thread>

using namespace std;

static void signalHandler(int nSigno)
{
    switch (nSigno)
    {
    case SIGPIPE:
        SPDLOG_INFO("SignalHandler::received %d signal,Process will not exit\n", nSigno);
        break;
    default:
        SPDLOG_INFO("SignalHandler::%d signal unregister\n", nSigno);
        break;
    }
}

//设置SIGPIPE信号处理改为间接忽略, 不然默认处理会导致进程退出
void CentreManage::initSignalHandler()
{
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
}


#define JUDGE_POINTER(pointer, name) \
if (pointer) \
{ \
    return pointer; \
} \
else \
{ \
    SPDLOG_ERROR("{} is nullptr, app exit!", name); \
    sleep(1); \
    exit(1); \
}

NavicatCallback* CentralPMCallback::m_pNavicatCb = nullptr;
WebsocketSerCallback* CentralPMCallback::m_pWebsocketCb = nullptr;
DBCallback* CentralPMCallback::m_pExecuteDBCb = nullptr;
MqttCallback* CentralPMCallback::m_pMqttCb = nullptr;
RobotBusiness* CentralPMCallback::m_pRobotBusiness = nullptr;
TelnetSerCallback* CentralPMCallback::m_pTelnetSerCb = nullptr;
PushRtmpCallback* CentralPMCallback::m_pPushRtmpCb = nullptr;
StateManagerCallback* CentralPMCallback::m_pRobotStateManage = nullptr;
HttpCallback* CentralPMCallback::m_pHttpCb = nullptr;
AlgorithmCallback* CentralPMCallback::m_pAlgorithmCb = nullptr;
TaskSchedulerCallback* CentralPMCallback::m_pTaskSchedulerCb = nullptr;
TaskCommonCallback* CentralPMCallback::m_pTaskCommonCb = nullptr;
GeneralAbilityCallback* CentralPMCallback::m_pGeneralAbility = nullptr;
NavicatCallback* CentralPMCallback::m_pNewNavicatCb = nullptr;
TestRobotCallback* CentralPMCallback::m_pTestRobotCb = nullptr;


NavicatCallback* CentralPMCallback::getNavicatCbHandler()
{
    JUDGE_POINTER(m_pNavicatCb, "m_pNavicatCb"); 
}

WebsocketSerCallback* CentralPMCallback::getWebsocketCbHandler()
{
    JUDGE_POINTER(m_pWebsocketCb, "m_pWebsocketCb");
}

AlgorithmCallback* CentralPMCallback::getAlgorithmHandler()
{
    JUDGE_POINTER(m_pAlgorithmCb, "m_pAlgorithmCb");
}

TaskSchedulerCallback* CentralPMCallback::getTaskScheHandler()
{
    JUDGE_POINTER(m_pTaskSchedulerCb, "m_pTaskSchedulerCb");
}

GeneralAbilityCallback* CentralPMCallback::getGeneralAbilityHandler()
{
    JUDGE_POINTER(m_pGeneralAbility, "m_pGeneralAbility");
}

DBCallback* CentralPMCallback::getExecuteDBCbHandler()
{
    JUDGE_POINTER(m_pExecuteDBCb, "m_pExecuteDBCb");
}

MqttCallback* CentralPMCallback::getMqttCbHandler()
{
    JUDGE_POINTER(m_pMqttCb, "m_pMqttCb");
}

RobotBusiness* CentralPMCallback::getRobotBusinessHandler()
{
    JUDGE_POINTER(m_pRobotBusiness, "m_pRobotBusiness");
}

PushRtmpCallback* CentralPMCallback::getPushRtmpHandler()
{
    JUDGE_POINTER(m_pPushRtmpCb, "m_pPushRtmpCb");
}

HttpCallback* CentralPMCallback::getHttpHandler()
{
    JUDGE_POINTER(m_pHttpCb, "m_pHttpCb");
}

StateManagerCallback* CentralPMCallback::getRobotStateManageHandler()
{
    JUDGE_POINTER(m_pRobotStateManage, "m_pRobotStateManage");
}

TaskCommonCallback* CentralPMCallback::getTaskCommonHandler()
{
    JUDGE_POINTER(m_pTaskCommonCb, "m_pTaskCommonCb");
}

NavicatCallback* CentralPMCallback::getNewNavicatCbHandler()
{
    JUDGE_POINTER(m_pNewNavicatCb, "m_pNewNavicatCb");
}

CentreManage::CentreManage()
{
}

int CentreManage::init()
{
    // 初始化信号处理函数
    initSignalHandler();

    //初始化通用能力组件
    initGeneralAbility();

    //初始化数据库组件
    initDatabase();

    //初始化状态组件模块
    initStateManage();

    //初始化Http组件
    initHttp();

    //初始化mqtt组件
    initMqtt();

    //websocket server初始化
    initWebsocket();

    //导航交互组件初始化
    initNavicat();

    //任务通用组件初始化
    initTaskCommon();

    //初始化任务调度模块
    initTaskScheduler();

    //在线调试组件初始化
    initTelnetServer();

    //推流组件初始化
    initPushRtmp();

    //算法组件
    initAlgorithm();

    //相应机器人初始化
    initRobot();

    return 0;
}

void CentreManage::run()
{
    //开始通用能力组件
    startGeneralAbility();

    //开始状态管理组件
    startStateManage();

    //开始数据库组件
    startDatabase();

    //开始导航交互
    startNavicat();

    //开始APP交互组件
    startWebsocket();

    //开始云端交互组件 mqtt http
    startMqttHttp();

    //开始任务通用组件
    startTaskCommon();

    //开始任务调度组件
    startTaskScheduler();

    //开始调试组件
    startTelnetServer();

    //开始算法组件
    startAlgorithm();

    //开始相应机器人
    startRobot();

    startTest();
}

//主线程，不能退出
//todo:找个事情做做吧
void CentreManage::loop()
{
    while (1)
    {
        sleep(3600);
    }
}


void CentreManage::initGeneralAbility()
{
    m_pGeneralAbility = GeneralAbilityCallback::getDeriveInstance();
    m_pGeneralAbility->init();
}

void CentreManage::startGeneralAbility()
{
    m_pGeneralAbility->start();
}
    
void CentreManage::initStateManage()
{
#ifdef STATE_SWITCH
    m_pRobotStateManage = StateManagerCallback::getDeriveInstance();
    m_pRobotStateManage->init();
#else
    m_pRobotStateManage = new StateManagerCallback;
#endif
}

void CentreManage::startStateManage()
{
    m_pRobotStateManage->start();
}

void CentreManage::initTelnetServer()
{
#ifdef TELNET_SWITCH
    m_pTelnetSerCb = TelnetSerCallback::getDeriveInstance();
    m_pTelnetSerCb->init(7890);
#else
    m_pTelnetSerCb = new TelnetSerCallback;
#endif
}

void CentreManage::startTelnetServer()
{
    m_pTelnetSerCb->start();
}

void CentreManage::initPushRtmp()
{
#ifdef PUSHRTMP_SWITCH
    m_pPushRtmpCb = PushRtmpCallback::getDeriveInstance();
#else
    m_pPushRtmpCb = new PushRtmpCallback;
#endif
}

void CentreManage::initAlgorithm()
{
#ifdef ALGORITHM_SWITCH
    m_pAlgorithmCb = AlgorithmCallback::getDeriveInstance();
    m_pAlgorithmCb->init();
#else
    m_pAlgorithmCb = new AlgorithmCallback;
#endif
}

void CentreManage::startAlgorithm()
{
    m_pAlgorithmCb->start();
}

void CentreManage::initHttp()
{
#ifdef HTTP_SWITCH
    m_pHttpCb = HttpCallback::getDeriveInstance();
    m_pHttpCb->init();
#else
    m_pHttpCb = new HttpCallback();
#endif

}

void CentreManage::initTaskScheduler()
{
#ifdef TASK_SCHEDULER_SWITCH
    m_pTaskSchedulerCb = TaskSchedulerCallback::getDeriveInstance();
    m_pTaskSchedulerCb->init(kNewTaskPreempt);
#else
    m_pTaskSchedulerCb = new TaskSchedulerCallback();
#endif
}

void CentreManage::startTaskScheduler()
{
    m_pTaskSchedulerCb->start();
}

void CentreManage::initTaskCommon()
{
#ifdef TASK_COMMON_SWITCH
    m_pTaskCommonCb = TaskCommonCallback::getDeriveInstance();
    m_pTaskCommonCb->init();
#else
    m_pTaskCommonCb = new TaskCommonCallback();
#endif
}

void CentreManage::startTaskCommon()
{
    m_pTaskCommonCb->start();
}

void CentreManage::initDatabase()
{
#ifdef DATABASE_SWITCH
    m_pExecuteDBCb = DBCallback::getDeriveInstance();
    m_pExecuteDBCb->init();
#else
    m_pExecuteDBCb = new DBCallback;
#endif
}

void CentreManage::initRobotBusiness()
{
    if (nullptr == m_pRobotBusiness)
    {
        m_pRobotBusiness = new RobotBusiness();
    }
}

void CentreManage::initTestRobot()
{
#ifdef TEST_ROBOT
    m_pTestRobotCb = TestRobotCallback::getDeriveInstance();
    m_pTestRobotCb->initTestRobot(m_pRobotBusiness);
#else
    m_pTestRobotCb = new TestRobotCallback;
#endif
}

void CentreManage::initRobot()
{

    initTestRobot();

    //It needs to be at the end of the robot initialization
    initRobotBusiness();
}

void CentreManage::startRobot()
{
#ifdef STATE_SWITCH
    CentralPMCallback::getRobotStateManageHandler()->changeState(emStateId::AwaitOrderStateId);//进入待命状态
#endif
    m_pTestRobotCb->startTestRobot();
}


void CentreManage::startNavicat()
{
    getNavicatCbHandler()->start();
}

void CentreManage::startMqttHttp()
{
    startHttp();
}

void CentreManage::startWebsocket()
{
    getWebsocketCbHandler()->start();
}

void CentreManage::startDatabase()
{
    m_pExecuteDBCb->start();
    
}

void CentreManage::startTest()
{
#ifdef TEST_SWITCH
    m_pTestCb->startTest();
#endif
}

void CentreManage::startHttp()
{
#ifdef HTTP_SWITCH
    m_pHttpCb->start();
#endif
}

void CentreManage::initNavicat()
{
#ifdef NAVICAT_SWITCH
    m_pNavicatCb = NavicatCallback::getDeriveInstance();
    m_pNavicatCb->init();
#else
    m_pNavicatCb = new NavicatCallback();
#endif


}

void CentreManage::initWebsocket()
{
#ifdef WEBSOCKET_SWITCH
    m_pWebsocketCb = WebsocketSerCallback::getDeriveInstance();
    m_pWebsocketCb->init();
#else
    m_pWebsocketCb = new WebsocketSerCallback();
#endif
}


void CentreManage::initMqtt()
{
#ifdef MQTT_SWITCH
    m_pMqttCb = MqttCallback::getDeriveInstance();
#else
    m_pMqttCb = new MqttCallback();
#endif
}

