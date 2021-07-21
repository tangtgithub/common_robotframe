#include "GridMap.h"
#include "algorithm_common.h"

//debug
#include<opencv2/opencv.hpp>
#include <cmath>

using namespace algorithm;

GridMap::GridMap() : m_pGeneralAlgorithm(nullptr)
{
    
}

GridMap::GridMap(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution) 
    : m_mapMinPoint(minPoint),
      m_mapMaxPoint(maxPoint),
      m_dResolution(resolution)
{
    m_vecGridData.clear();

    int size = getMapWidth() * getMapHeight(); 
    for (int i=0; i<size; ++i)
    {
        m_vecGridData.push_back(kOccupyGrid);
    }

}

GridMap::~GridMap()
{
    
}

void GridMap::load(const algorithm::Point& minPoint, const algorithm::Point& maxPoint, double resolution)
{
    m_walkPaths.clear();
    m_vecGridData.clear();

    m_mapMinPoint = minPoint;
    m_mapMaxPoint = maxPoint;
    m_dResolution = resolution;

    int size = getMapWidth() * getMapHeight(); 
    for (int i=0; i<size; ++i)
    {
        m_vecGridData.push_back(kOccupyGrid);
    }
    SPDLOG_INFO("GridMap::load gridmap, m_mapMinPoint={},{}, m_dResolution={}",m_mapMinPoint.dx,m_mapMinPoint.dy,m_dResolution);
}

int GridMap::position2GridIndex(const algorithm::Point& point, size_t& index)
{

    GridMap::GridPos pos;
    position2GridPos(point,pos);
    gridPos2GridIndex(pos, index);

    if ( index > m_vecGridData.size()-1 )
    {
        SPDLOG_ERROR("position2GridIndex::error,point x={},y={}",point.dx, point.dy);
        return -1;
    }
    else
    {
        return 0;
    }
}

int GridMap::position2GridPos(const algorithm::Point& point, GridMap::GridPos& pos)
{
    double x = point.dx - m_mapMinPoint.dx;
    double y = point.dy - m_mapMinPoint.dy;

    pos.row = static_cast<size_t>(y/m_dResolution);
    pos.col = static_cast<size_t>(x/m_dResolution);

    if ( pos.row == getMapHeight() )
    {
        pos.row = getMapHeight()-1;
    }
    if ( pos.col == getMapWidth() )
    {
        pos.col = getMapWidth()-1;
    }
    return 0;

}

int GridMap::gridPos2Position(const GridMap::GridPos& pos, algorithm::Point& point)
{
    
    point.dx = static_cast<double>(pos.col) * m_dResolution + m_mapMinPoint.dx;
    point.dy = static_cast<double>(pos.row) * m_dResolution + m_mapMinPoint.dy;
    
    if ( point.dx < m_mapMinPoint.dx || point.dx > m_mapMaxPoint.dx )
    {
        SPDLOG_ERROR("gridPos2Position::x error,row={},col={}", pos.row, pos.col);
        return -1;
    }
    if ( point.dy < m_mapMinPoint.dy || point.dy > m_mapMaxPoint.dy )
    {
        SPDLOG_ERROR("gridPos2Position::y error,row={},col={}", pos.row, pos.col);
        return -1;
    }
    return 0;
}

int GridMap::gridIndex2Position(size_t index, algorithm::Point& point)
{
    GridMap::GridPos pos;
    if ( gridIndex2GridPos(index, pos) == -1 )
    {
        SPDLOG_ERROR("gridIndex2Position::error,index={}", index);
        return -1;
    }
    return gridPos2Position(pos, point);
}

int GridMap::gridIndex2GridPos(size_t index, GridMap::GridPos& pos)
{
    pos.row = index / getMapWidth();
    pos.col = index % getMapWidth();

    if ( pos.row < getMapHeight() && pos.col < getMapWidth() )
    {
        return 0;
    }
    else
    {
        SPDLOG_ERROR("gridIndex2Position::error,index={}", index);
        return -1;
    }
}

int GridMap::gridPos2GridIndex(const GridMap::GridPos& pos, size_t &index)
{
    index = pos.row * getMapWidth() + pos.col;

    if ( index > m_vecGridData.size()-1)
    {
        SPDLOG_ERROR("gridPos2Position::error,row={},col={}", pos.row, pos.col);
        return -1;
    }
    return 0;
}

int8_t GridMap::getPointOccupyStatus(const algorithm::Point& point)
{
    size_t index = 0;
    if ( position2GridIndex(point, index) == -1 )
    {
        return -1;
    }

    return m_vecGridData[index];
}

int GridMap::setPointOccupyStatus(const algorithm::Point& point, int8_t status)
{
    size_t index = 0;

    if ( position2GridIndex(point, index) == -1)
    {
        std::cout << "setPointOccupyStatus::position2GridIndex fail" << "point:" << point.dx << "," << point.dy << std::endl;
        SPDLOG_ERROR("setPointOccupyStatus::position2GridIndex fail, point:x={},y={}", point.dx,point.dy);
        return -1;
    }
    m_vecGridData[index] = status;

    return 0;
}

int GridMap::setAllPointOccupyStatus(int8_t status)
{
    for (auto &grid : m_vecGridData)
    {
        grid = status;
    }
    return 0;
}

int8_t GridMap::getGridOccupyStatus(size_t index)
{
    if ( index > m_vecGridData.size()-1 )
    {
        return -1;
    }
    else
    {
        return m_vecGridData[index];
    }
}

bool GridMap::getGridOccupyStatus(size_t row, size_t col)
{
    size_t index;
    GridPos pos;
    pos.row = row;
    pos.col = col;
    gridPos2GridIndex(pos, index);
    int8_t gridStatus = getGridOccupyStatus(index);

    return gridStatus == kFreeGrid ? true : false;

}

int GridMap::addWalkPath(const algorithm::Point &point )
{
    std::vector<std::vector<algorithm::Point>> walkPathsAdd;
    std::vector<algorithm::Point> walkPathAdd;

    std::vector<std::vector<algorithm::Point>> walkPaths;

    if (getWalkPath(walkPaths))
    {
        SPDLOG_ERROR("addWalkPath error, getWalkPath fail");
        return -1;
    }

    algorithm::Point walkPoint;

    if (m_pGeneralAlgorithm->findNearestPoint(walkPaths, point, walkPoint))
    {
        SPDLOG_ERROR("addWalkPath error, findNearestPoint fail");
        return -2;
    }

    walkPathAdd.push_back(point);
    walkPathAdd.push_back(walkPoint);
    walkPathsAdd.push_back(walkPathAdd);
    addWalkPath(walkPathsAdd);
    return 0;
}

int GridMap::addWalkPath(const std::vector<std::vector<algorithm::Point>> &walkPaths )
{
    if ( !m_pGeneralAlgorithm )
    {
        return -1;
    }

    std::vector<std::vector<algorithm::Point>> generatePaths;
    for ( const auto& vec : walkPaths ) 
    {
        algorithm::Point first = *vec.begin();
        algorithm::Point last = *vec.rbegin();
        SPDLOG_INFO("addWalkPath::line point size={},first={},{},last={},{}",vec.size(),first.dx, first.dy, last.dx, last.dy);
        m_walkPaths.push_back(vec);
        std::vector<algorithm::Point> generatePath;
        m_pGeneralAlgorithm->generatePointsByLine(vec,generatePath,m_dResolution/2.0);

        //debug
#if 1
        for ( size_t i=0; i<generatePath.size()-1; ++i )
        {
            double dis = m_pGeneralAlgorithm->calDistancePoints(generatePath[i],generatePath[i+1]);
            if ( dis > m_dResolution )
            {
                SPDLOG_ERROR("generatePointsByLine::distance={},fisrP={},{}, secondP={},{}",dis,generatePath[i].dx,generatePath[i].dy,dis,generatePath[i+1].dx,generatePath[i+1].dy);

            }

        }
#endif
        generatePaths.push_back(generatePath);
    }

    for ( const auto& vec : generatePaths ) 
    {
        for ( const auto& point : vec )
        {
            setPointOccupyStatus(point, kFreeGrid);
        }

#if 1 //debug
        for ( size_t i=0; i<vec.size()-1; ++i )
        {
            GridPos pos1;
            GridPos pos2;
            position2GridPos(vec[i],pos1);
            position2GridPos(vec[i+1],pos2);
            if ( std::abs(static_cast<int>(pos1.row) - static_cast<int>(pos2.row)) > 1 || std::abs(static_cast<int>(pos1.col) - static_cast<int>(pos2.col)) > 1)
            {
                SPDLOG_ERROR("addWalkPath fail::pos1={},{} pos2={},{}",pos1.row,pos1.col, pos2.row, pos2.col);
                SPDLOG_ERROR("addWalkPath fail, point1:x={},y={}  point2:x={},y={}", vec[i].dx,vec[i].dy, vec[i+1].dx,vec[i+1].dy);

            }
            size_t index1 = 0;
            size_t index2 = 0;

            if ( position2GridIndex(vec[i], index1) == -1)
            {
                SPDLOG_ERROR("setPointOccupyStatus::position2GridIndex fail, point:x={},y={}", vec[i].dx,vec[i].dy);

            }
            if ( position2GridIndex(vec[i+1], index2) == -1)
            {
                SPDLOG_ERROR("setPointOccupyStatus::position2GridIndex fail, point:x={},y={}", vec[i+1].dx,vec[i+1].dy);

            }
            if ( std::abs(static_cast<int>(index1) - static_cast<int>(index2)) > static_cast<int>(getMapWidth()+1) )
            {
                SPDLOG_ERROR("setPointOccupyStatus::position2GridIndex fail, index1={},index2={}<Plug>PeepOpenoint1:x={},y={}<Plug>PeepOpenoint2:x={},y={}", index1,index2,vec[i].dx,vec[i].dy, vec[i+1].dx,vec[i+1].dy);

            }
        }
#endif
    }
    return 0;
}

int GridMap::getWalkPath(std::vector<std::vector<algorithm::Point>> &walkPaths)
{
    if ( !m_pGeneralAlgorithm )
    {
        return -1;
    }

    for ( const auto& vec : m_walkPaths ) 
    {
        std::vector<algorithm::Point> generatePath;
        m_pGeneralAlgorithm->generatePointsByLine(vec,generatePath,m_dResolution/2);
        walkPaths.push_back(generatePath);
    }
    return 0;
}

int GridMap::switchToSourcePng(const std::string& path)
{
#if 0
    uint32_t width = getMapWidth();
    uint32_t height = getMapHeight();

    cv::Mat srcMap(height, width, CV_8U);

    for ( size_t i=0; i<m_vecGridData.size(); ++i )
    {
        GridMap::GridPos pos;
        gridIndex2GridPos(i, pos);

        if ( m_vecGridData.at(i) == kOccupyGrid ) //占据栅格
        {
            srcMap.at<uchar>(static_cast<int>(pos.row), static_cast<int>(pos.col)) = 0;
        }
        else if ( m_vecGridData.at(i) == kFreeGrid ) // 自由栅格，可通行
        {
            srcMap.at<uchar>(static_cast<int>(pos.row), static_cast<int>(pos.col)) = 255;
        }
        else if ( m_vecGridData.at(i) == kUnknowGird ) //状态未知
        {
            srcMap.at<uchar>(static_cast<int>(pos.row), static_cast<int>(pos.col)) = 200;
        }
    }

    imwrite(path, srcMap);
#endif
    return 0;
}

int GridMap::switchToPng(const std::string& path)
{
#if 1 //debug
    uint32_t width = getMapWidth();
    uint32_t height = getMapHeight();

    if ( width == 0 || height == 0 )
    {
        return -1;
    }

    cv::Mat srcMap(height, width, CV_8U);
    std::vector<uint8_t> vecMapBuffer(width * height);

    /*栅格从地图左下角开始递增，而opencv的mat数据是从地图的左上角开始递增，所以此处需要转换一下*/
    for ( size_t i=0; i<vecMapBuffer.size(); ++i )
    {
        size_t col = i%width;
        size_t row = height - i / width - 1;
        size_t index = row * width + col;

        if ( m_vecGridData.at(i) == kOccupyGrid ) //占据栅格
        {
            vecMapBuffer.at(index) = 0;
        }
        else if ( m_vecGridData.at(i) == kFreeGrid ) // 自由栅格，可通行
        {
            vecMapBuffer.at(index) = 255;
        }
        else if ( m_vecGridData.at(i) == kUnknowGird ) //状态未知
        {
            vecMapBuffer.at(index) = 200;
        }
    }

    size_t index = 0;
    for (size_t row = 0; row < height; row++)
    {
        for (size_t col = 0; col < width; col++)
        {
            srcMap.at<uchar>(static_cast<int>(row), static_cast<int>(col)) = vecMapBuffer[index];
            index++;
        }
    }
    std::cout << "switchToPng:" << vecMapBuffer.size() << " " << *vecMapBuffer.begin() << std::endl;
    imwrite(path, srcMap);
#endif
    return 0;
}



GridMapPtr GridMap::clone()
{
    GridMapPtr pGridMap = std::make_shared<GridMap>();
    pGridMap->m_mapMinPoint = m_mapMinPoint;
    pGridMap->m_mapMaxPoint = m_mapMaxPoint;
    pGridMap->m_dResolution = m_dResolution;
    pGridMap->m_vecGridData = m_vecGridData;
    pGridMap->m_walkPaths = m_walkPaths;
    pGridMap->m_pGeneralAlgorithm = m_pGeneralAlgorithm;
    return pGridMap;
}
