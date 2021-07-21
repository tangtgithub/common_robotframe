#include "AlgorithmManage.h"
#include "algorithm_common.h"
#include<chrono>
#include<thread>



using namespace algorithm;

AlgorithmManage::AlgorithmManage() : m_pGridMap(new GridMap)
                                     , m_pGeneralAlgorithm(new GeneralAlgorithm)
{
    m_pGridMap->init(m_pGeneralAlgorithm);
    
}

AlgorithmManage::~AlgorithmManage()
{

}
    
void AlgorithmManage::init()
{

}

void AlgorithmManage::start() 
{
    SPDLOG_INFO("algorithm start!!!");

}
    
void AlgorithmManage::loadMap(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution)
{
    m_pGridMap->load(minPoint, maxPoint, resolution);
}

int AlgorithmManage::addWalkPath(const std::vector<std::vector<algorithm::Point>> &walkPath )
{
    return m_pGridMap->addWalkPath(walkPath); 
}

int AlgorithmManage::addWalkPath(const std::vector<algorithm::Point> &walkPath )
{
    std::vector<std::vector<algorithm::Point>> walkPaths;
    walkPaths.push_back(walkPath);
    return m_pGridMap->addWalkPath(walkPaths);
}

int AlgorithmManage::addWalkPath(const algorithm::Point &point )
{
    return m_pGridMap->addWalkPath(point); 
}


int AlgorithmManage::findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint )
{
    return m_pGeneralAlgorithm->findNearestPoint(paths, targetPoint, findPoint);
}

int AlgorithmManage::findNearestPoint(const std::vector<std::vector<algorithm::Point>> paths, const algorithm::Point& targetPoint, algorithm::Point& findPoint , size_t skipPointNum) 
{
    return m_pGeneralAlgorithm->findNearestPoint(paths, targetPoint, findPoint, skipPointNum);
}



int AlgorithmManage::generatePointsByLine(const std::vector<algorithm::Point>& linePoints, std::vector<algorithm::Point>& generatePoints, double distance )
{
    return m_pGeneralAlgorithm->generatePointsByLine(linePoints, generatePoints, distance);
}

double AlgorithmManage::calDistancePoints(const algorithm::Point& firstP, const algorithm::Point& secondP)
{
    return m_pGeneralAlgorithm->calDistancePoints(firstP, secondP);
}

int AlgorithmManage::getWalkPath(std::vector<std::vector<algorithm::Point>> &walkPaths)
{
    return m_pGridMap->getWalkPath(walkPaths);
}

double AlgorithmManage::angle(const algorithm::Point& firstPoint, const algorithm::Point& secondPoint)
{
    return m_pGeneralAlgorithm->angle(firstPoint,secondPoint);
}

