#include "BusiEnterGame.h"
#include "GSProtocol.pb.h"
#include "ProtocalSys.h"
#include "SingletonHolder.h"
#include "log.h"
#include "RoleInfo.h"
#include "ShmqWrapper.h"

using namespace Connd::Protocol;
using namespace Connd::Data;
using namespace Connd::Common;

int BusiEnterGame::OnLogin( ConndMsg& stMsg )
{
	LOG_FUNCTION;

	if(CheckAvgLoad()!=0)
	{
		return -1;
	}

	/*Do some login transaction logic */
	const CSLoginData &stData = stMsg.stGSPkg.body().cslogindata();
	int iUin = stData.user().qq();
	// int iRoleId = GenRoleIdMap(iUin);
	char test[64]="Hello It's me";
	//SingletonHolder<ShmqWrapper>::Instance()->OnWrite(test,64);

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
