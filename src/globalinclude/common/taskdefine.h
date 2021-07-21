/** *******************************************************
* @brief        : 任务相关的定义
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 
**********************************************************/
#ifndef __TASKDEFINE_H
#define __TASKDEFINE_H

#include <string>
#include <stdint.h>
#include <pthread.h>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <iostream>



namespace task{

struct TaskBasicInfo { //任务的调度基本数据类型
    uint32_t id;///任务id
    uint8_t taskType;///任务类型 1:清扫
    uint32_t server_task_id; //服务器的任务id
    uint8_t taskStatus; //任务状态 1：未开始 ，2：正在执行，3：任务异常，4：已完成，5：已取消,6：取消中 7：暂停
    uint8_t taskMode;  //任务模式  0：一次性任务 1：循环
    std::string taskName;  //任务名称
    uint8_t executationType;////执行类型 1：立即执行 2：定时执行 3:不执行
    uint8_t taskPriority;////优先级 1-极低，2-普通，3-正常，4-紧急
    std::string  beginDate; //开始日期 2019-05-20
    std::string  endDate;  // 结束日期若结束时间为空的循环任务，则一直执行+末尾'\0'
    std::string  beginTime; //8:59:08
    std::string  endTime;  //18:59:08
    uint8_t isAllDay;  //是否全天 0：时间段 1：全天  2：单次执行
    uint16_t executTimes; //执行次数：仅在循环任务中有效
    uint8_t executEndType; //任务结束方式 0:一直执行，1:其他时间，2:完成执行次数
    uint8_t repeatType; //重复类型 周或月或年 1：每天 2：每周 3：每个月
    std::string  repeatDate; //重复日期
    uint32_t mapId;  //本体地图id   
};

};


#endif
