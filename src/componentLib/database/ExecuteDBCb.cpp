#include "ExecuteDBCb.h"
#include "db_common.h"
#include <string>
#include <iostream>



void ExecuteDBCb::init()
{

}

void ExecuteDBCb::start()
{
    SPDLOG_INFO("database start!!!");

}


int ExecuteDBCb::setKeyValue(const std::string& key, const std::string& value)
{
    return 0;
}

std::string ExecuteDBCb::getKeyValue(const std::string& key)
{
    return "";
}
