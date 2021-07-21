/** *******************************************************
* @brief        :中央处理模块对外接口
* @author       :qz
* @date         :2020-11-10
* @copyright    :Copyright (C) 2020 - All Rights Reserved
* @note         :
**********************************************************/

#ifndef __CENTRALPM_CALLBACK_H_
#define __CENTRALPM_CALLBACK_H_

#include "GeneralAbilityCallback.h"
#include "WebsocketSerCallback.h"
#include "NavicatCallback.h"
#include "DBCallback.h"
#include "MqttCallback.h"
#include "RobotBusiness.h"
#include "TelnetSerCallback.h"
#include "PushRtmpCallback.h"
#include "HttpCallback.h"
#include "TaskSchedulerCallback.h"
#include "TaskCommonCallback.h"
#include "TestRobotCallback.h"
#include "StateManagerCallback.h"
#include "AlgorithmCallback.h"
#include <iostream>

class CentralPMCallback
{

public:
    //初始化各模块
    virtual int init() { return 0; }
    //运行各模块
    virtual void run() {}
    //循环事件处理
    virtual void loop() {}

public:
    /*********************全局获取其他模块指针******************************/
    //禁止在各自模块的init接口调用其他模块，此时其他模块可能未完成初始化
    //获取中央管理模块指针
    static CentralPMCallback* getDeriveInstance();
    //获取导航模块指针
    static NavicatCallback* getNavicatCbHandler();
    //获取websocket模块指针
    static WebsocketSerCallback* getWebsocketCbHandler();
    //获取数据库模块指针
    static DBCallback* getExecuteDBCbHandler();
    //获取mqtt模块指针
    static MqttCallback* getMqttCbHandler();
    //获取机器人业务指针
    static RobotBusiness* getRobotBusinessHandler();
    //获取推流模块指针
    static PushRtmpCallback* getPushRtmpHandler();
    //获取http模块指针
    static HttpCallback* getHttpHandler();
    //获取算法模块指针
    static AlgorithmCallback* getAlgorithmHandler();
    //获取任务调度模块指针
    static TaskSchedulerCallback* getTaskScheHandler();
    //获取任务通用模块指针
    static TaskCommonCallback* getTaskCommonHandler();
    //获取新通用功能模块指针
    static GeneralAbilityCallback* getGeneralAbilityHandler();
    //获取状态机指针
    static StateManagerCallback* getRobotStateManageHandler();
    //获取新导航对接模块指针
    static NavicatCallback* getNewNavicatCbHandler();

    static TestRobotCallback* getTestRobotCbHandler();

    static CentralPMCallback* m_cpm;
    static NavicatCallback* m_pNavicatCb;
    static WebsocketSerCallback* m_pWebsocketCb;
    static DBCallback* m_pExecuteDBCb;
    static MqttCallback* m_pMqttCb;
    static RobotBusiness* m_pRobotBusiness;
    static TelnetSerCallback* m_pTelnetSerCb;
    static PushRtmpCallback* m_pPushRtmpCb;
    static StateManagerCallback* m_pRobotStateManage;
    static HttpCallback* m_pHttpCb;
    static AlgorithmCallback* m_pAlgorithmCb;
    static TaskSchedulerCallback* m_pTaskSchedulerCb;
    static TaskCommonCallback* m_pTaskCommonCb;
    static GeneralAbilityCallback* m_pGeneralAbility;
    static NavicatCallback* m_pNewNavicatCb;
    static TestRobotCallback* m_pTestRobotCb;
};



#endif
