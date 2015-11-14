#ifndef __MSGMGR_H__
#define  __MSGMGR_H__

#include "ConndMsg.h"
#include "MsgDispatcher.h"

class MsgMgr
{
public:
	MsgMgr();
	~MsgMgr();
	enum {MAX_NET_BUFFER_SIZE = 256*1024};
public:
	int Init();
	int Clear();
	int GetOneConndMsg(Session *pSession);
	int OnProcMsg();
	int OnSendPkg(Connd::Protocol::GSPkg &stPkg, Session &stSession);
private:
	bool EncodeMsg(Connd::Protocol::GSPkg& pkg, char *buf, int iBufLen);
	bool DecodeMsg(Connd::Protocol::GSPkg& pkg, const char *buf, int iBufLen);
	void DumpMsg(Connd::Protocol::GSPkg& pkg);

	MsgDispatcher m_stMsgDispatcher;
	ConndMsg m_stConndMsg;
	char m_sNetBuf[MAX_NET_BUFFER_SIZE];
};

#endif