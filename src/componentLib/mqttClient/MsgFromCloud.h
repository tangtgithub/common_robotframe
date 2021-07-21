#ifndef _COMM_CMD_H_
#define _COMM_CMD_H_

#include <string>

class MsgFromCloud
{
public:
    void onMsg(int cmd, std::string msg);

private:
};



#endif
