#include "DBCallback.h"
#include "ExecuteDBCb.h"


DBCallback* DBCallback::getDeriveInstance()
{
    static ExecuteDBCb dbcb;
    return &dbcb;
}
