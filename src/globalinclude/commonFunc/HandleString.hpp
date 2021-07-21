/** *******************************************************
* @brief        :字符串格式处理，获取各种指定格式的字符串
* @author       :tangt
* @date         :2020-12-31
* @copyright    :Copyright (C) 2020 - All Rights Reserved
* @note         :
**********************************************************/

#ifndef __HANDLE_STRING_H__
#define __HANDLE_STRING_H__

#include <time.h>
#include <string>

namespace HandleString {

    void trimBlank(std::string &s) 
    {
        if (s.empty()) 
        {
            return ;
        }
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
    
    void  split(const std::string& s, char delimiter, std::vector<std::string> &vecRes)
    {
        using namespace std;
        string token;
        istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))                                                                                                      
        {
            vecRes.push_back(token);
        }
    }

    void setPrecision(std::string& s,uint8_t digit)
    {
        std::string::size_type pos;
        pos = s.find(".");
        if ( pos != std::string::npos && pos+digit < s.size())
        {
            s = s.substr(0,pos+digit);
        }
    }

}
#endif
