#ifndef XML_CONFIG_ACCESS_HPP
#define XML_CONFIG_ACCESS_HPP

#include <stdexcept>
#include <string>
#include <mutex>
#include <set>
#include <map>
#include <list>
#include <memory>
#include <vector>
#include "xml2/tinyxml2.h"
#include "commondefine.h"

using namespace tinyxml2;


/* *******************

推荐用法: 可以像操作 jsoncpp 对象一样操作 xml 文件

XmlConfigAccess xmlConfig;

int ret = xmlConfig.OpenXmlFile("robotConfig.xml"); // 加载xml文件
if (ret) {
    std::cout << "load robotConfig.xml failed, error code:" << ret << std::endl;
    return;
}

std::cout << xmlConfig["robotBasicInfo"]["ubuntu_name"].GetValue() << std::endl; // 获取ubuntu_name结点值

xmlConfig["robotBasicInfo"]["ubuntu_name"] = "iom"; // 给ubuntu_name结点赋值

std::cout << xmlConfig["robotBasicInfo"]["ubuntu_name"].GetValue() << std::endl; // 再次获取ubuntu_name结点值

**/

class XmlConfigAccess
{
public: /*** *********************** 通用接口 *********************** ***/

    /* *******************
        线程安全
        @param1    strXmlFile xml 文件
        @returnval 0 表示正常打开，其他值表示 XMLError
    **/
    int OpenXmlFile(const std::string& strXmlFile)
    {
        return _OpenXmlFile(strXmlFile);
    }

    /* *******************
        线程安全
        @param1    keys 表示需要访问的 xml 结点的变长参数
        @returnval 返回获取的的 xml 结点的 value
    **/
    template <class ... Key>
    std::string GetXmlValue(const Key& ... keys)
    {
        return _GetXmlValue(keys ...);
    }

    /* *******************
        线程安全
        @param1    strXmlFile xml 文件
        @param2    keys 表示需要访问的 xml 结点的变长参数
        @returnval 返回获取的的 xml 结点的 value
    **/
    template <class ... Key>
    std::string GetXmlValueWithFile(const std::string& strXmlFile, const Key& ... keys)
    {
        return _GetXmlValueWithFile(strXmlFile, keys ...);
    }

    /* *******************
        线程安全
        @param1    strValue 表示需要获取的 xml 的结点的值
        @param2    keys 表示需要访问的 xml 结点的变长参数
        @returnval 返回是否成功 0 表示成功，-1表示失败
    **/
    template <class ... Key>
    int GetXmlForValue(std::string& strValue, const Key& ... keys)
    {
        return _GetXmlForValue(strValue, keys ...);
    }

    /* *******************
        线程安全
        @param1    strXmlFile xml 文件
        @param2    strValue 表示需要获取的 xml 的结点的值
        @param3    keys 表示需要访问的 xml 结点的变长参数
        @returnval 返回是否成功 0 表示成功，-1表示失败
    **/
    template <class ... Key>
    int GetXmlForValueWithFile(const std::string& strXmlFile, std::string& strValue, const Key& ... keys)
    {
        return _GetXmlForValueWithFile(strXmlFile, keys ...);
    }

    /* *******************
        线程安全
        @param1    strValue 表示需要设置的 xml 的结点的值
        @param2    keys 表示需要访问的 xml 结点的变长参数
        @returnval 0 表示正常设置，-1 表示文件不存在，其他值表示 XMLError
    **/
    template <class ... Key>
    int SetXmlValue(const std::string& strValue, const Key& ... keys)
    {
        return _SetXmlValue(strValue, keys ...);
    }

    /* *******************
        线程安全
        @param1    strXmlFile xml 文件
        @param2    strValue 表示需要设置的 xml 的结点的值
        @param3    keys 表示需要访问的 xml 结点的变长参数
        @returnval 0 表示正常设置，-1 表示文件不存在，其他值表示 XMLError
    **/
    template <class ... Key>
    int SetXmlValueWithFile(const std::string& strXmlFile, const std::string& strValue, const Key& ... keys)
    {
        return _SetXmlValueWithFile(strXmlFile, strValue, keys ...);
    }

    /* *******************
        线程不安全
        @param1    strKey 表示需要访问的 xml 结点
        @returnval 返回类对象本身
    **/
    XmlConfigAccess& operator [] (const std::string& strKey)
    {
        m_vKey.push_back(strKey);
        return *this;
    }

    /* *******************
        线程不安全
        @param1    strValue 表示需要设置的 xml 的结点的值
        @returnval none
    **/
    void operator = (const std::string& strValue)
    {
        _SetXmlValue(strValue, m_vKey);
        m_vKey.clear();
    }

    /* *******************
        线程不安全
        获取通过重载运算符 [] 访问到的结点的值
        @returnval none
    **/
    std::string GetValue()
    {
        std::vector<std::string> vKey = std::move(m_vKey);
        return _GetXmlValue(vKey);
    }

public: /*** 一些需要谨慎使用的接口，可能会获取不到想要的值，因为一个key可能会重复 ***/

    /* *******************
        @param1    strKey key 值
        @returnval 返回获取的 xml 结点的 value
    **/
    std::string GetXmlKeyValue(const std::string& strKey)
    {
        return _GetXmlKeyValue(strKey);
    }

    /* *******************
        @param1    strXmlFile xml 文件
        @param2    strKey key 值
        @returnval 返回获取的所有 xml 结点的 value
    **/
    std::string GetXmlKeyValueWithFile(const std::string& strXmlFile, const std::string& strKey)
    {
        return _GetXmlKeyValueWithFile(strXmlFile, strKey);
    }

    /* *******************
        @param1    strValue 待赋值的 value 参数
        @param2    strKey key 值
        @returnval 返回获取的所有 xml 叶子结点的 value
    **/
    int GetXmlKeyValue(std::string& strValue, const std::string& strKey)
    {
        return _GetXmlKeyValue(strValue, strKey);
    }

    /* *******************
        @param1    strXmlFile xml 文件
        @param2    strValue 待赋值的 value 参数
        @param3    strKey key 值
        @returnval 0 表示正常设置，-1 表示失败
    **/
    int GetXmlKeyValueWithFile(const std::string& strXmlFile, std::string& strValue, const std::string& strKey)
    {
        return _GetXmlKeyValueWithFile(strXmlFile, strValue, strKey);
    }

public: /*** ********************* 一些特化的接口 ********************* ***/

    /* *******************
        @param1    strXmlFile xml 文件
        @param1    sValues 获取的所有 xml 叶子结点的 value 的 set 集合
        @returnval 返回获取的所有 xml 叶子结点的 value 的字符串拼接
    **/
    std::string GetXmlValues(const std::string& strXmlFile, std::set<uint32_t>& sValues) // cphu 模块的特化版本
    {
        return _GetXmlValues(strXmlFile, sValues);
    }

private:
    int _OpenXmlFile(const std::string& strXmlFile)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __OpenXmlFile(strXmlFile);
    }

    template <class ... Key>
    std::string _GetXmlValue(const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __GetXmlValue(keys ...);
    }

    template <class ... Key>
    std::string _GetXmlValueWithFile(const std::string& strXmlFile, const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        __OpenXmlFile(strXmlFile);
        return __GetXmlValue(keys ...);
    }

    template <class ... Key>
    int _GetXmlForValue(std::string& strValue, const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __GetXmlForValue(strValue, keys ...);
    }

    template <class ... Key>
    int _GetXmlForValueWithFile(const std::string& strXmlFile, std::string& strValue, const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        __OpenXmlFile(strXmlFile);
        return __GetXmlForValue(strValue, keys ...);
    }

    template <class ... Key>
    int _SetXmlValue(const std::string& strValue, const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __SetXmlValue(strValue, keys ...);
    }

    template <class ... Key>
    int _SetXmlValueWithFile(const std::string& strXmlFile, const std::string& strValue, const Key& ... keys)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        __OpenXmlFile(strXmlFile);
        return __SetXmlValue(strValue, keys ...);
    }

    std::string _GetXmlKeyValue(const std::string& strKey)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __GetXmlKeyValue(strKey);
    }

    std::string _GetXmlKeyValueWithFile(const std::string& strXmlFile, const std::string& strKey)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        __OpenXmlFile(strXmlFile);
        return __GetXmlKeyValue(strKey);
    }

    int _GetXmlKeyValue(std::string& strValue, const std::string& strKey)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __GetXmlKeyValue(strValue, strKey);
    }

    int _GetXmlKeyValueWithFile(const std::string& strXmlFile, std::string& strValue, const std::string& strKey)
    {
        std::lock_guard<std::mutex> lk(m_lock);
        __OpenXmlFile(strXmlFile);
        return __GetXmlKeyValue(strValue, strKey);
    }

    std::string _GetXmlValues(const std::string& strXmlFile, std::set<uint32_t>& sValues) // cphu 模块的特化版本
    {
        std::lock_guard<std::mutex> lk(m_lock);
        return __GetXmlValues(strXmlFile, sValues);
    }

private:
    int __OpenXmlFile(const std::string& strXmlFile)
    {
        m_strXmlFile = strXmlFile;
        if (m_mConfig.find(m_strXmlFile) == m_mConfig.end()) {
            std::shared_ptr<XMLDocument> pCXmlDoc = std::make_shared<XMLDocument>();
            std::string strXmlPath = kRobotConfigDir + m_strXmlFile;

            int ret = pCXmlDoc->LoadFile(strXmlPath.c_str());
            if (ret) {
                return ret;
            }

            m_mConfig[m_strXmlFile] = pCXmlDoc;

            std::vector<std::string> vKey;
            __GetValue(pCXmlDoc->RootElement()->FirstChildElement(), vKey);
        }

        return 0;
    }

    void __GetValue(XMLElement* pChildElement, std::vector<std::string>& vKey)
    {
        while (pChildElement) {
            if (pChildElement->GetText() == nullptr) {
                vKey.push_back(pChildElement->Value());
                __GetValue(pChildElement->FirstChildElement(), vKey);
                pChildElement = pChildElement->NextSiblingElement();
                vKey.pop_back();
                continue;
            }
            vKey.push_back(pChildElement->Value());
            m_mvKeyValue[m_strXmlFile][vKey] = pChildElement->GetText();
            m_mKeyValue[m_strXmlFile][pChildElement->Value()] = pChildElement->GetText();
            vKey.pop_back();
            pChildElement = pChildElement->NextSiblingElement();
        }
    }

    template <class ... Key>
    std::string __GetXmlValue(const Key& ... keys)
    {
        if (m_mConfig.find(m_strXmlFile) == m_mConfig.end()) {
            return "";
        }
        return m_mvKeyValue[m_strXmlFile][{keys ...}];
    }

    template <class ... Key>
    int __GetXmlForValue(std::string& strValue, const Key& ... keys)
    {
        if (m_mConfig.find(m_strXmlFile) == m_mConfig.end()) {
            return -1;
        }

        strValue = m_mvKeyValue[m_strXmlFile][{keys ...}];
        return strValue.empty() ? -1 : 0;
    }

    template <class ... Key>
    int __SetXmlValue(const std::string& strValue, const Key& ... keys)
    {
        if (m_mConfig.find(m_strXmlFile) == m_mConfig.end()) {
            return -1;
        }

        std::vector<std::string> vKey = { keys ... };
        std::shared_ptr<XMLDocument> pCXmlDoc = m_mConfig[m_strXmlFile];
        XMLElement* pRootElement = pCXmlDoc->RootElement();

        int ret = __SetValue(pCXmlDoc, pRootElement, strValue, vKey);
        if (!ret) {
            m_mvKeyValue[m_strXmlFile][vKey] = strValue;
            m_mKeyValue[m_strXmlFile][*vKey.rbegin()] = strValue;
        }

        return ret;
    }

    int __SetValue(std::shared_ptr<XMLDocument>& pCXmlDoc, XMLElement* pChildElement, const std::string& strValue, const std::vector<std::string>& strPathVec)
    {
        if (!pChildElement) return -1;

        for (std::size_t i = 0; i < strPathVec.size(); ++i) {
            if (strPathVec[i].empty()) {
                return -1;
            }
            if (!pChildElement->FirstChildElement(strPathVec[i].c_str())) {
                XMLElement* pParam = pCXmlDoc->NewElement(strPathVec[i].c_str());
                pChildElement->InsertEndChild(pParam);
            }
            pChildElement = pChildElement->FirstChildElement(strPathVec[i].c_str());
        }
        pChildElement->SetText(strValue.c_str());

        return __SaveFile(pCXmlDoc, m_strXmlFile);;
    }

    int __SaveFile(std::shared_ptr<XMLDocument>& pCXmlDoc, const std::string& strFile)
    {
        std::string strXmlPath = kRobotConfigDir + strFile;
        return pCXmlDoc->SaveFile(strXmlPath.c_str());
    }

    std::string __GetXmlKeyValue(const std::string& strKey)
    {
        return m_mKeyValue[m_strXmlFile][strKey];
    }

    int __GetXmlKeyValue(std::string& strValue, const std::string& strKey)
    {
        if (m_mConfig.find(m_strXmlFile) == m_mConfig.end()) {
            return -1;
        }

        strValue = m_mKeyValue[m_strXmlFile][strKey];

        return strValue.empty() ? -1 : 0;
    }

    std::string __GetXmlValues(const std::string& strXmlFile, std::set<uint32_t>& sValues) // cpu 模块的特化版本
    {
        std::string strXmlPath = kRobotConfigDir + strXmlFile;
        std::shared_ptr<XMLDocument> pCXmlDoc = std::make_shared<XMLDocument>();
        int ret = pCXmlDoc->LoadFile(strXmlPath.c_str());
        if (ret) {
            return {};
        }

        std::string strValues;
        __GetValues(pCXmlDoc->RootElement()->FirstChildElement(), sValues, strValues);
        return strValues.substr(2); // 去除开头的逗号
    }

    void __GetValues(XMLElement* pChildElement, std::set<uint32_t>& sValues, std::string& strValues) // cpu 模块的特化版本
    {
        while (pChildElement) {
            if (pChildElement->GetText() == nullptr) {
                __GetValues(pChildElement->FirstChildElement(), sValues, strValues);
                pChildElement = pChildElement->NextSiblingElement();
                continue;
            }
            sValues.insert(std::atoi(pChildElement->GetText()));
            strValues += ", " + std::string(pChildElement->GetText());
            pChildElement = pChildElement->NextSiblingElement();
        }
    }

public:
    XmlConfigAccess() {}
    ~XmlConfigAccess() {}

private:
    XmlConfigAccess(XmlConfigAccess& rhs) = delete;
    XmlConfigAccess& operator = (XmlConfigAccess& rhs) = delete;

private:
    std::map<std::string, std::shared_ptr<XMLDocument>> m_mConfig;
    std::map<std::string, std::map<std::vector<std::string>, std::string>> m_mvKeyValue;
    std::map<std::string, std::map<std::string, std::string>> m_mKeyValue;
    std::string m_strXmlFile;
    std::mutex m_lock;
    std::vector<std::string> m_vKey;
};

#endif /* XML_CONFIG_ACCESS_HPP */
