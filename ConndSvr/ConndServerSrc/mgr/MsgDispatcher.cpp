#include <string.h>

#include "MsgDispatcher.h"
#include "SingletonHolder.h"
#include "GScommon.pb.h"
#include "log.h"
#include "BusiEnterGame.h"

using namespace Connd::Common;


#define BIND_CONND_MESSAGE(fun,msgid) _BIND_CONND_MESSAGE(fun,msgid) 
#define REG_CONND_MESSAGE(msgid) _REG_CONND_MESSAGE(msgid)

#define _BIND_CONND_MESSAGE(fun,msgid) \
    int g_callback_##msgid(ConndMsg& stMsg) \
    { \
        return fun(stMsg); \
    }

#define _REG_CONND_MESSAGE(msgid) \
    m_vCallBackFuns[msgid-CONNDCALLBACK_MSG_ID_BASE]=g_callback_##msgid;

//add new protocal command should BIND_CONND_MESSAGE &REG_CONND_MESSAGE
BIND_CONND_MESSAGE(SingletonHolder<BusiEnterGame>::Instance()->OnLogin,CS_LOGIN);

MsgDispatcher::MsgDispatcher()
{
	bzero(m_vCallBackFuns,sizeof(m_vCallBackFuns));

	REG_CONND_MESSAGE(CS_LOGIN);
}

MsgDispatcher::~MsgDispatcher()
{
	
}

int MsgDispatcher::Dispatch(ConndMsg& stMsg)
{
	int iRet = 0;

	if ( CONNDCALLBACK_MSG_ID_BASE <= stMsg.uCmdId && MAX_CONND_MSG_ID >= stMsg.uCmdId &&
		m_vCallBackFuns[stMsg.uCmdId-CONNDCALLBACK_MSG_ID_BASE])
	{
		iRet=m_vCallBackFuns[stMsg.uCmdId-CONNDCALLBACK_MSG_ID_BASE](stMsg);
	}
	else
	{
	    logerr("Unknown  message cmd,cmdid:%d\n",stMsg.uCmdId);
		return -1;
	}
	
	if(iRet<0)
    {
        logerr("Dispatch  message error,msgid:%d,iret=%d\n",stMsg.uCmdId,iRet);
		return iRet;
    }
	logdbg("Suc Dispatch  message msgid:%d\n",stMsg.uCmdId);
    return 0;		
}

