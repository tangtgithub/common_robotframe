/** *******************************************************
* @brief        : 观察者
* @author       :tangt
* @date         :2020-06-22
* @copyright    :Copyright (C) 2021 - All Rights Reserved
* @note         : 如果一个模块想要其他模块转发消息给自己时，
*                 需要继承 MsgCallback，然后通过其他模块中的注册观察者的接口注册即可
**********************************************************/
#ifndef __CLOUD_MSG_CALLBACK_H__
#define __CLOUD_MSG_CALLBACK_H__

#include <string>
#include <memory>
#include <vector>

class MsgCallback
{
public:
	MsgCallback() {}
	virtual ~MsgCallback() {}

	virtual void onMsg(uint16_t cmd, std::shared_ptr<char> msg) { return; }
	virtual void onMsg(int cmd, int sessionId, const std::string &msg) { return; }
	virtual void onMsg(int cmd, const std::string& msg) { return; }
};

struct CmdObserver
{
	std::vector<uint16_t> cmds;
	MsgCallback* p_handler;
};

#endif
