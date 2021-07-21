#ifndef __EXECUTEDBCALLBACK_H
#define __EXECUTEDBCALLBACK_H

#include <string>
#include "json/json.h"
#include "commondefine.h"
#include <vector>
#include <time.h>


class DBCallback
{
public:
    static DBCallback* getDeriveInstance();
    virtual void init() {}
    virtual void start() {}
    virtual int setKeyValue(const std::string& key, const std::string& value) {return 0;}
    virtual std::string getKeyValue(const std::string& key) {return "";}
};

#endif
