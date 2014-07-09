#ifndef __MSGMGR_H__
#define  __MSGMGR_H__

#include "GSProtocol.pb.h"
#include "Session.h"

struct ConndMsg
{
	uint32_t uCmdId;
	int iDataLen;
	char* pszData;
	Session* pSession;
	Connd::Protocol::GSPkg stGSPkg;
	void Reset() 
	{
		uCmdId=0;
		iDataLen=0;
		pszData = NULL;
		pSession = NULL;
	}
	ConndMsg()
	{
		Reset();
	};
};

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
private:
	bool EncodeMsg(Connd::Protocol::GSPkg& pkg, char *buf, int iBufLen);
	bool DecodeMsg(Connd::Protocol::GSPkg& pkg, const char *buf, int iBufLen);
	void DumpMsg(Connd::Protocol::GSPkg& pkg);

	ConndMsg m_stConndMsg;
	char m_sNetBuf[MAX_NET_BUFFER_SIZE];
};

#endif