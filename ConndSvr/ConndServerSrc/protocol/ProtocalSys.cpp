#include "ProtocalSys.h"
#include "SingletonHolder.h"
#include "Session.h"
#include "MsgMgr.h"

using namespace Connd::Protocol;
using namespace Connd::Data;
using namespace Connd::Common;

ProtocalSys::ProtocalSys()
{
}
ProtocalSys::~ProtocalSys()
{
}

int ProtocalSys::SendLoginInfo( ConndMsg &stMsg )
{
	m_stPkg.Clear();
	m_stPkg.mutable_header()->set_cmdid(SC_LOGIN);
	m_stPkg.mutable_header()->set_packagelen(0);
	SCLoginData *pSCLoginData = m_stPkg.mutable_body()->mutable_sclogindata();
	pSCLoginData->mutable_user()->CopyFrom(stMsg.stGSPkg.body().cslogindata().user());
	pSCLoginData->set_ret(0);
	//last set package len into header
	m_stPkg.mutable_header()->set_packagelen(m_stPkg.ByteSize());

	return SingletonHolder<MsgMgr>::Instance()->OnSendPkg(m_stPkg,*stMsg.pSession);
}


