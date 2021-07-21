/** *******************************************************
* @brief        :栅格地图
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         :本模块主要负责栅格地图的构建和管理
**********************************************************/
#ifndef __GRID_MAP_H_
#define __GRID_MAP_H_

#include<cstdint>
#include<vector>
#include<string>
#include "AlgorithmCallback.h"
#include <memory>
#include "GeneralAlgorithm.h"

namespace algorithm {

/****************栅格状态**************************/
const int8_t kOccupyGrid = 100; //占据
const int8_t kFreeGrid = 0; //空闲
const int8_t kUnknowGird = -1; //未知
/**************************************************/

class GridMap;
typedef std::shared_ptr<GridMap> GridMapPtr; 

class GridMap
{
public:
    
    struct GridPos
    {
        size_t row; //行号
        size_t col; //列号
    };

    GridMap();
    GridMap(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution);
    ~GridMap();

    void init(GeneralAlgorithmPtr pGeneralAlgo)
    {
        m_pGeneralAlgorithm = pGeneralAlgo;
    }

    /* 根据地图最大、最小点、分辨率 加载一张栅格地图
    */
    void load(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution = 0.05);

    /* 获取地图的宽度
    * returnValue uint32_t 单位：栅格个数
    */
    uint32_t getMapWidth()
    {
        return static_cast<uint32_t>((m_mapMaxPoint.dx - m_mapMinPoint.dx) / m_dResolution);
    }

    /* 获取地图的高度
    * returnValue uint32_t 单位：栅格个数
    */
    uint32_t getMapHeight()
    {
        return static_cast<uint32_t>((m_mapMaxPoint.dy - m_mapMinPoint.dy) / m_dResolution);
    }

    /* 坐标点的位置转换成栅格行列坐标
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  algorithm::Point: 点坐标
    * parameter2  GridMap::GridPos: 求出的栅格的行列号
    */
    int position2GridPos(const algorithm::Point& point, GridMap::GridPos& pos);


    /* 栅格行列坐标转换成点位坐标
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  GridMap::GridPos: 栅格的行列号
    * parameter2  algorithm::Point: 输出点位坐标
    */
    int gridPos2Position(const GridMap::GridPos& pos, algorithm::Point& point);

    /* 获取坐标点的占据状态
    * returnValue int8_t 100:表示有障碍物，不可同行; 0: 表示空闲，可以同行; -1：表示状态未知
    * parameter1  algorithm::Point: 点坐标
    */
    int8_t getPointOccupyStatus(const algorithm::Point& point);


    /* 获取栅格的占据状态
    * returnValue int8_t 100:表示有障碍物，不可同行; 0: 表示空闲，可以同行; -1：表示状态未知
    * parameter1  size_t 栅格索引
    */
    int8_t getGridOccupyStatus(size_t index);
    
    /* 获取栅格的占据状态
    * returnValue bool true:可通行 false：不可通行
    * parameter1  size_t 栅格行号
    * parameter2  size_t 栅格列号
    */
    bool getGridOccupyStatus(size_t row, size_t col);


    /* 设置坐标点的占据状态
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  algorithm::Point: 点坐标
    * parameter2  int8_t: 占据状态 
    */
    int setPointOccupyStatus(const algorithm::Point& point, int8_t status);

    int setAllPointOccupyStatus(int8_t status);


    /* 把机器人运行轨迹加入到栅格地图中，相当于把运行轨迹对应的栅格状态置成可通行状态
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 机器人运行轨迹，可传多条
    */
    int addWalkPath(const std::vector<std::vector<algorithm::Point>> &walkPaths );


    /* 根据现有的运行轨迹，找到一个和目标点point最近的点，把这两点之间的路径也作为运行轨迹加入到栅格地图中
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  algorithm::Point 需要扩展运行轨迹的目标点
    */
    int addWalkPath(const algorithm::Point &point );

    /* 获取机器人的运行轨迹线
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 出参:机器人运行轨迹
    */
    int getWalkPath(std::vector<std::vector<algorithm::Point>> &walkPaths);

    /* 将当前栅格地图转成png文件并保存到文件系统中
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  std::string: 需要保存的png的绝对路径名
    */
    int switchToPng(const std::string& path);
    int switchToSourcePng(const std::string& path);

    GridMapPtr clone();
    

private:

    /* 坐标点的位置转换成栅格在数组中的索引
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  algorithm::Point: 点坐标
    * parameter2 size_t 栅格数组索引
    */
    int position2GridIndex(const algorithm::Point& point, size_t& index);

    /* 栅格索引转成坐标点的位置
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  size_t: 栅格索引
    * parameter2  algorithm::Point: 求出的点坐标
    */
    int gridIndex2Position(size_t index, algorithm::Point& point);

    /* 栅格索引转成栅格行列坐标
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  size_t: 栅格索引
    * parameter2  GridMap::GridPos: 求出的栅格的行列号
    */
    int gridIndex2GridPos(size_t index, GridMap::GridPos& pos);

    /* 栅格行列号转成栅格索引
    * returnValue int 转换成功返回0 失败返回-1
    * parameter1  GridMap::GridPos: 栅格的行列号
    * parameter2  size_t: 求出的栅格索引
    */
    int gridPos2GridIndex(const GridMap::GridPos& pos, size_t &index);

private:
    algorithm::Point           m_mapMinPoint;
    algorithm::Point           m_mapMaxPoint;

    double                     m_dResolution;

    /* 栅格数据，每个值都代表这个栅格的占据状态
     * 100:表示有障碍物，不可通行; 0: 表示空闲，可以通行; -1：表示状态未知 */
    std::vector<int8_t>        m_vecGridData; 
    
    /* 运行轨迹线
     * 为了节省内存，此处存储的是addWalkPath传进来的运行轨迹，并未经过扩展 */
    std::vector<std::vector<algorithm::Point>> m_walkPaths; 

    GeneralAlgorithmPtr        m_pGeneralAlgorithm;
};


};

#endif
