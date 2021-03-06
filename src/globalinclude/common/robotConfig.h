/* ------------------------------------ 机器人定义模块 ------------------------------------ */
#ifndef ROBOTCONFIG_H
#define ROBOTCONFIG_H
#define ROBOT_TEST 1
#define FILELOG

#if(ROBOT_TEST)
#define TEST_ROBOT

#elif(ROBOT_OTHER)
#define OTHER_ROBOT

#endif

/* ------------------------------------ 不同机器人配置 ------------------------------------ */

/* ------------------------------------ 测试机器人 ------------------------------------ */
#ifdef TEST_ROBOT
#define ALGORITHM_SWITCH
#define TASK_SCHEDULER_SWITCH
#define TASK_COMMON_SWITCH
#define DATABASE_SWITCH
#define NAVICAT_SWITCH 
#define WEBSOCKET_SWITCH
#define MQTT_SWITCH
#define TELNET_SWITCH
#define STATE_SWITCH
#define HTTP_SWITCH
#define PUSHRTMP_SWITCH
#endif


#endif /* ROBOTCONFIG_H */
