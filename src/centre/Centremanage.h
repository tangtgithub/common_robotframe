#ifndef __CENTREMANAGE_H__
#define __CENTREMANAGE_H__

#include "CentralPMCallback.h"

class CentreManage :public CentralPMCallback
{
public:
    static CentreManage* getInstance()
    {
        static CentreManage cenm;
        return &cenm;
    }
    //继承自CentralPMCallback的接口
    int                  init() override;
    void                 run() override;
    void                 loop() override;

private:
    void initStateManage();
    void initGeneralAbility();
    void initConfig();
    void initSignalHandler();
    void initWebsocket();
    void initWebsocketClient();
    void initNavicat();
    void initDatabase();
    void initMqtt();
    void initTelnetServer();
    void initPushRtmp();
    void initAlgorithm();
    void initHttp();
    void initTaskScheduler();
    void initTaskCommon();
    void initRobot();
    void initTestRobot();
    void initRobotBusiness();


    void startNavicat();
    void startMqttHttp();
    void startWebsocket();
    void startDatabase();
    void startHttp();
    void startGeneralAbility();
    void startStateManage(); 
    void startTaskCommon();
    void startTaskScheduler();
    void startTelnetServer();
    void startAlgorithm();
    void startRobot();
    
    void startTest();

private:
    CentreManage();
    
};

#endif
