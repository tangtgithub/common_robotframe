/** *******************************************************
* @brief        :通用算法
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :本模块主要负责一些通用算法的实现
**********************************************************/
#ifndef __GENERAL_ALGORITHM_H_
#define __GENERAL_ALGORITHM_H_

#include<cstdint>
#include<vector>
#include<string>
#include<memory>
#include "AlgorithmCallback.h"

namespace algorithm {

class GeneralAlgorithm
{
public:
    /* 传入一到多条路径，在这些路径上查找离目标点最近的点
    * returnValue int 0:表示查找成功  -1:表示查找失败
    * parameter1  std::vector<std::vector<algorithm::Point>> 传入的一到多条路径
    * parameter2  algorithm::Point 目标点
    * parameter3  algorithm::Point 出参，返回找到的最近的点
    */
    int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint );

    // skipPointNum : 跳点查找，点数太密太多的时候，一次查找花费时间长，可以通过skipPointNum指定跳点数来查找
    int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint , size_t skipPointNum);

    /* 根据折线各端点，每隔distance的距离生成一个点，最终生成一组密集的点
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<GridMap::Point> : 折线各端点坐标
    * parameter2  vector<GridMap::Point> : 输出折线上的密集点
    * parameter3  double : 点之间的距离 
    */
    int generatePointsByLine(const std::vector<algorithm::Point>& linePoints, std::vector<algorithm::Point>& generatePoints, double distance = 0.02);

    /* 计算两点间的距离
    * returnValue double 返回两点间直线距离
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    double calDistancePoints(const algorithm::Point& firstP, const algorithm::Point& secondP);

    /* 计算两点连线的角度
    * returnValue double 返回两点连线的角度
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    double angle(const algorithm::Point& firstPoint, const algorithm::Point& secondPoint);

};

typedef std::shared_ptr<GeneralAlgorithm> GeneralAlgorithmPtr; 

}
#endif
