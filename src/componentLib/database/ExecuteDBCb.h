#ifndef __EXECUTEDB_H__
#define __EXECUTEDB_H__
#include <string>
#include <pthread.h>
#include "json/json.h"
#include <vector>
#include "DBCallback.h"

class ExecuteDBCb : public DBCallback
{
public:

    void init() override;
    void start() override;
    int setKeyValue(const std::string& key, const std::string& value) override;
    std::string getKeyValue(const std::string& key) override;

};
#endif
