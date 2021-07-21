#ifndef __ALGORITHM_CALLBACK_H
#define __ALGORITHM_CALLBACK_H

#include <string>
#include <vector>
#include <unistd.h>


namespace algorithm{

struct Point
{
    double dx;
    double dy;
    bool operator<(const Point& point) const
    {
        return dx < point.dx;
    }
};

};

class AlgorithmCallback
{
public:
    static AlgorithmCallback* getDeriveInstance();

    virtual void init() {}
    virtual void start() {}

    /* 根据地图最大、最小点、分辨率 加载一张栅格地图
    */
    virtual void loadMap(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution = 0.05) {}

    /* 把机器人运行轨迹加入到栅格地图中，相当于把运行轨迹对应的栅格状态置成可通行状态
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 机器人运行轨迹，可传多条
    */
    virtual int addWalkPath(const std::vector<std::vector<algorithm::Point>> &walkPath ) {return 0;}
    virtual int addWalkPath(const std::vector<algorithm::Point> &walkPath ) {return 0;}
    
    /* 根据现有的运行轨迹，找到一个和目标点point最近的点，把这两点之间的路径也作为运行轨迹加入到栅格地图中
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  algorithm::Point 需要扩展运行轨迹的目标点
    */
    virtual int addWalkPath(const algorithm::Point &point ) {return 0;}

    /* 将当前栅格地图转成png文件并保存到文件系统中
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  std::string: 需要保存的png的绝对路径名
    */
    virtual int switchToPng(const std::string& path) {return 0;}
    virtual int switchToPng(const std::vector<std::vector<algorithm::Point>> planPaths,const std::string& path) {return 0;}
    virtual int switchToSourcePng(const std::string& path) {return 0;}

    //测试接口，仅供内部测试
    virtual void test() { return; }

    /* 根据起点和多个目标点获取全部规划路径
    * @param startPos      输入参数, 起点
    * @param targets       输入参数, 多个目标点
    * @param paths         输出参数, 多条规划路径
    */
    virtual int getPlanPath(const algorithm::Point& startPos, const std::vector<algorithm::Point> &targets, std::vector<std::vector<algorithm::Point>> &paths) { return 0; }
    
    /* 传入一到多条路径，在这些路径上查找离目标点最近的点
    * returnValue int 0:表示查找成功  -1:表示查找失败
    * parameter1  std::vector<std::vector<algorithm::Point>> 传入的一到多条路径
    * parameter2  algorithm::Point 目标点
    * parameter3  algorithm::Point 出参，返回找到的最近的点
    */
    virtual int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint ) {return 0;}
    // skipPointNum : 跳点查找，点数太密太多的时候，一次查找花费时间长，可以通过skipPointNum指定跳点数来查找
    virtual int findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint, size_t skipPointNum) {return 0;}

    /* 根据折线各端点，每隔distance的距离生成一个点，最终生成一组密集的点
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<GridMap::Point> : 折线各端点坐标
    * parameter2  vector<GridMap::Point> : 输出折线上的密集点
    * parameter3  double : 点之间的距离 
    */
    virtual int generatePointsByLine(const std::vector<algorithm::Point>& linePoints, std::vector<algorithm::Point>& generatePoints, double distance = 0.05) {return 0;}

    /* 计算两点间的距离
    * returnValue double 返回两点间直线距离
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    virtual double calDistancePoints(const algorithm::Point& firstP, const algorithm::Point& secondP) {return 0.0;}

    /* 获取机器人的运行轨迹线
    * returnValue int 0:表示设置成功  -1:表示设置失败
    * parameter1  vector<vector<algorithm::Point>> 出参:机器人运行轨迹
    */
    virtual int getWalkPath(std::vector<std::vector<algorithm::Point>> &walkPaths) {return 0;}

    /* 计算两点连线的角度
    * returnValue double 返回两点连线的角度
    * parameter1  algorithm::Point : 第一个参与计算的点
    * parameter2  algorithm::Point : 第二个参与计算的点
    */
    virtual double angle(const algorithm::Point& firstPoint, const algorithm::Point& secondPoint) {return 0.0;}

    virtual void test2() {}
};
#endif
