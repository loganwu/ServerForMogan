#include "MsgMgr.h"
#include "log.h"
#include <string.h>
//using namespace Connd;
MsgMgr::MsgMgr()
{

}

MsgMgr::~MsgMgr()
{
	google::protobuf::ShutdownProtobufLibrary();
}

int MsgMgr::Init()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	memset(m_sNetBuf,0,sizeof(m_sNetBuf));

	return 0;
}


int MsgMgr::Clear()
{
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}


bool MsgMgr::EncodeMsg(Connd::Protocol::GSPkg& pkg, char *buf,int iBufLen)
{
	if (!pkg.SerializeToArray(buf,iBufLen))
	{
		logerr("SerializeToArray failed for pkg\n");
		return false;
	}

	return true;
}

bool MsgMgr::DecodeMsg(Connd::Protocol::GSPkg& pkg, const char *buf , int iBufLen)
{
	if (!pkg.ParseFromArray(buf, iBufLen))
	{
		logerr("parse pkg from buf failed\n");
		return false;
	}

	return true;
}

int MsgMgr::GetOneConndMsg(Session *pSession)
{
	if (!pSession)
	{
		logerr("Session NULL\n");
		return -1;
	}
	
	m_stConndMsg.Reset();
	m_stConndMsg.pSession = pSession;
	m_stConndMsg.iDataLen = sizeof(m_sNetBuf)-1;
	memset(m_sNetBuf,0,sizeof(m_sNetBuf));

	pSession->OnRead(m_sNetBuf,m_stConndMsg.iDataLen);
	// TODO Check pakcet integraty with iReclen
	if (m_stConndMsg.iDataLen  <= 0)
	{
		logerr("Rec a Uncompleted pkt\n");
		return -1;
	}
	
	if (!DecodeMsg(m_stConndMsg.stGSPkg,m_sNetBuf,m_stConndMsg.iDataLen))
	{
		return -2;
	}
	m_stConndMsg.uCmdId = m_stConndMsg.stGSPkg.header().cmdid();
	DumpMsg(m_stConndMsg.stGSPkg);

	return m_stMsgDispatcher.Dispatch(m_stConndMsg);

}

void MsgMgr::DumpMsg( Connd::Protocol::GSPkg& pkg )
{
#ifdef DEBUG
	logdbg("=====================================\n");
	pkg.PrintDebugString();
	logdbg("=====================================\n");
#endif
}

int MsgMgr::OnSendPkg( Connd::Protocol::GSPkg &stPkg, Session &stSession)
{
	LOG_FUNCTION;
	DumpMsg(stPkg);
	if (EncodeMsg(stPkg,m_sNetBuf,stPkg.header().packagelen()))
	{
		return stSession.OnWrite(m_sNetBuf,stPkg.header().packagelen());
	}
	return 0;
}







