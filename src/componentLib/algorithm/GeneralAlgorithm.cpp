
#include "algorithm_common.h"
#include <cmath>
#include <chrono>
#include "GeneralAlgorithm.h"

using namespace algorithm;

#define PI acos(-1)


int GeneralAlgorithm::findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint )
{
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    double minDis = 8888888;
    for ( const auto& vec : paths ) 
    {
        for ( const auto& point : vec )
        {
            if ( calDistancePoints( point, targetPoint ) < minDis )
            {
                findPoint = point;
                minDis = calDistancePoints( point, targetPoint );
            }
        }
    }
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_ns = endTime - startTime;

    SPDLOG_INFO("findNearestPoint paths::point x={},y={},minDistance={},calculate time={}",findPoint.dx,findPoint.dy,minDis, elapsed_ns.count());
    return 0;
}


int GeneralAlgorithm::findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint , size_t skipPointNum)
{
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    double minDis = 8888888;
    for ( const auto& vec : paths ) 
    {
        for ( size_t i=0; i<vec.size(); )
        {
            if ( calDistancePoints( vec[i], targetPoint ) < minDis )
            {
                findPoint = vec[i];
                minDis = calDistancePoints( vec[i], targetPoint );
            }
            i = i+skipPointNum;
        }
    }
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_ns = endTime - startTime;

    SPDLOG_INFO("findNearestPoint paths::point x={},y={},minDistance={},calculate time={}",findPoint.dx,findPoint.dy,minDis, elapsed_ns.count());
    return 0;
}

int GeneralAlgorithm::generatePointsByLine(const std::vector<algorithm::Point>& linePoints, std::vector<algorithm::Point>& generatePoints, double distance)
{
    if (linePoints.size() <= 1)
    {
        SPDLOG_ERROR("generatePointsByLine fail,point num error");
        return FAILURE;
    }

    algorithm::Point tmpPoint;
    for (vector<algorithm::Point>::size_type i = 0; i < linePoints.size() - 1; ++i)
    {
        tmpPoint.dx = linePoints[i].dx;
        tmpPoint.dy = linePoints[i].dy;
        generatePoints.push_back(tmpPoint);

        double k = 0.0;
        double b = 0.0;
        bool bVerticalLine = false;
        double radian = 0.0;

        if ( (linePoints[i + 1].dx - linePoints[i].dx) != 0 )
        {
            k = (linePoints[i + 1].dy - linePoints[i].dy) / (linePoints[i + 1].dx - linePoints[i].dx);
            radian = atan( abs(linePoints[i + 1].dy - linePoints[i].dy) / abs(linePoints[i + 1].dx - linePoints[i].dx));
        }
        else
        {
            bVerticalLine = true;
        }

        b = linePoints[i].dy - k * linePoints[i].dx;

        if ( bVerticalLine )
        {
            for (int count = 0; count < static_cast<int>(fabs(linePoints[i + 1].dy - linePoints[i].dy) / distance); ++count)
            {
                tmpPoint.dy = linePoints[i+1].dy > linePoints[i].dy ? tmpPoint.dy+distance : tmpPoint.dy - distance;
                generatePoints.push_back(tmpPoint);
            }
        }
        else
        {
            double varx = distance * cos(radian);
            for (int count = 0; count < static_cast<int>(fabs(linePoints[i + 1].dx - linePoints[i].dx) / varx); ++count)
            {
                tmpPoint.dx = linePoints[i].dx > linePoints[i + 1].dx ? tmpPoint.dx - varx : tmpPoint.dx + varx;
                tmpPoint.dy = k * tmpPoint.dx + b;
                generatePoints.push_back(tmpPoint);
            }
        }
    }
    tmpPoint.dx = linePoints.rbegin()->dx;
    tmpPoint.dy = linePoints.rbegin()->dy;
    generatePoints.push_back(tmpPoint);
    //SPDLOG_INFO("generatePointsByLine success");
    return SUCCESS;
}

double GeneralAlgorithm::calDistancePoints(const algorithm::Point& firstP, const algorithm::Point& secondP)
{
    return sqrt(pow(firstP.dx-secondP.dx, 2) + pow(firstP.dy-secondP.dy, 2));
}

double GeneralAlgorithm::angle(const algorithm::Point& firstPoint, const algorithm::Point& secondPoint)
{
    double tmpAngle;
    double xx, yy;
    xx = secondPoint.dx - firstPoint.dx;
    yy = secondPoint.dy - firstPoint.dy;

    if (xx == 0.0)
        tmpAngle = PI / 2.0;
    else
        tmpAngle = atan(fabs(yy / xx));

    if ((xx < 0.0) && (yy >= 0.0))
        tmpAngle = PI - tmpAngle;
    else if ((xx < 0.0) && (yy < 0.0))
        tmpAngle = PI + tmpAngle;
    else if ((xx >= 0.0) && (yy < 0.0))
        tmpAngle = PI * 2.0 - tmpAngle;

    return (tmpAngle * 180 / PI);
}
