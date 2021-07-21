/** *******************************************************
* @brief        :算法模块管理
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :本类是整个算法模块的封装管理类
**********************************************************/
#ifndef __ALGORITHM_MANAGE_H_
#define __ALGORITHM_MANAGE_H_

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "AlgorithmCallback.h"
#include "GridMap.h"
#include "GeneralAlgorithm.h"

namespace algorithm {

class AlgorithmManage : public AlgorithmCallback
{
public:
    AlgorithmManage();
    ~AlgorithmManage();
    
    void init() override;
    void start() override;
    
    /* 根据地图最大、最小点、分辨率 加载一张栅格地图
    */
    void loadMap(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution = 0.05) override;

    /* 把机器人运行轨迹加入到栅格地图中，相当于把运行轨迹对应的栅格状态置成可通行状态
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 机器人运行轨迹，可传多条
    */
    int addWalkPath(const std::vector<std::vector<algorithm::Point>> &walkPath ) override;
    int addWalkPath(const std::vector<algorithm::Point> &walkPath ) override;

    /* 根据现有的运行轨迹，找到一个和目标点point最近的点，把这两点之间的路径也作为运行轨迹加入到栅格地图中
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  algorithm::Point 需要扩展运行轨迹的目标点
    */
    int addWalkPath(const algorithm::Point &point ) override;

    /* 传入一到多条路径，在这些路径上查找离目标点最近的点
    * returnValue int 0:表示查找成功  -1:表示查找失败
    * parameter1  std::vector<std::vector<algorithm::Point>> 传入的一到多条路径
    * parameter2  algorithm::Point 目标点
    * parameter3  algorithm::Point 出参，返回找到的最近的点
    */
    int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint ) override;

    // skipPointNum : 跳点查找，点数太密太多的时候，一次查找花费时间长，可以通过skipPointNum指定跳点数来查找
    int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint , size_t skipPointNum) override;

    /* 根据折线各端点，每隔distance的距离生成一个点，最终生成一组密集的点
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<GridMap::Point> : 折线各端点坐标
    * parameter2  vector<GridMap::Point> : 输出折线上的密集点
    * parameter3  double : 点之间的距离 
    */
    int generatePointsByLine(const std::vector<algorithm::Point>& linePoints, std::vector<algorithm::Point>& generatePoints, double distance = 0.02) override;

    /* 计算两点间的距离
    * returnValue double 返回两点间直线距离
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    double calDistancePoints(const algorithm::Point& firstP, const algorithm::Point& secondP) override;

    /* 获取机器人的运行轨迹线
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 出参:机器人运行轨迹
    */
    int getWalkPath(std::vector<std::vector<algorithm::Point>> &walkPaths) override;


    /* 计算两点连线的角度
    * returnValue double 返回两点连线的角度
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    double angle(const algorithm::Point& firstPoint, const algorithm::Point& secondPoint) override;

private:
    GridMapPtr          m_pGridMap; 
    GeneralAlgorithmPtr m_pGeneralAlgorithm;
};

};
#endif
