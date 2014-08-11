/************************************************************************/
/*prog entrance                                                         */  
/************************************************************************/
#include "NetworkWrapper.h"
#include "SingletonHolder.h"
#include "log.h"
#include "MemMgr.h"

#define _INIT_DEF(ClassName) do \
{ \
	if(SingletonHolder<ClassName>::Instance()->Init() != 0) \
	{ \
		logerr("%s init failure\n",#ClassName); \
	} \
	else \
	{ \
		logdbg("%s init success\n",#ClassName); \
	} \
} while (0);


int Init()
{
	_INIT_DEF(NetworkWrapper);
	_INIT_DEF(MemMgr);
	
	return 0;
}

void Proc()
{
	SingletonHolder<NetworkWrapper>::Instance()->OnRun();
}

int main(int argc, char **argv)
{
	Init();
	//Tick();
	Proc();

	printf("done\n");
	return 0;
}
