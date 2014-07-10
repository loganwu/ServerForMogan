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
}


int MsgMgr::Clear()
{
	google::protobuf::ShutdownProtobufLibrary();
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
	DumpMsg(m_stConndMsg.stGSPkg);

	return 0;
}

void MsgMgr::DumpMsg( Connd::Protocol::GSPkg& pkg )
{
	pkg.PrintDebugString();
}






