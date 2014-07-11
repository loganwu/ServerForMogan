#include "BusiEnterGame.h"
#include "GSProtocol.pb.h"
#include "ProtocalSys.h"
#include "SingletonHolder.h"

using namespace Connd::Protocol;
using namespace Connd::Data;
using namespace Connd::Common;

int BusiEnterGame::OnLogin( ConndMsg& stMsg )
{
	if(CheckAvgLoad()!=0)
	{
		return -1;
	}

	/*Do some login transaction logic */
	const CSLoginData &stData = stMsg.stGSPkg.body().cslogindata();
	int iUin = stData.user().qq();
	// int iRoleId = GenRoleIdMap(iUin);

	SingletonHolder<ProtocalSys>::Instance()->SendLoginInfo(stMsg);

	return 0;
}

int BusiEnterGame::OnEnterZone( ConndMsg& stMsg )
{
	return 0;
}

int BusiEnterGame::FinishLogin( /*RoleInfo* pstRoleInfo*/ )
{
	return 0;
}

int BusiEnterGame::CheckAvgLoad()
{
	return 0;
}
