#ifndef __CONNDMSG_H__
#define  __CONNDMSG_H__

#include <stdint.h>
#include "GSProtocol.pb.h"
#include "Session.h"

#define MAX_CONND_MSG_ID 128
#define CONNDCALLBACK_MSG_ID_BASE 1

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

#endif