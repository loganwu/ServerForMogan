/************************************************************************/
/*prog entrance                                                         */  
/************************************************************************/
#include "NetworkWrapper.h"
#include "SingletonHolder.h"
#include "log.h"
#include "MemMgr.h"
#include "ShmqWrapper.h"

struct GameConf
{
	int iMemcacheKey;
	int iSessionKey;
	int iShmqueKey;
	GameConf(){memset(this,0,sizeof(*this));}
};

GameConf g_stConf;

#define _INIT_DEF(ClassName) do \
{ \
	if(SingletonHolder<ClassName>::Instance()->Init() != 0) \
	{ \
		logerr("%s init failure\n",#ClassName); \
		return -1;\
	} \
	else \
	{ \
		logdbg("%s init success\n",#ClassName); \
	} \
} while (0);


int Init()
{
	g_stConf.iShmqueKey = 20140814;
	_INIT_DEF(NetworkWrapper);
	_INIT_DEF(MemMgr);
	if(SingletonHolder<ShmqWrapper>::Instance()->InitProducer(g_stConf.iShmqueKey ) != 0)
	{
		logerr("ShmqWrapper init failure\n");
	}
	else 
	{ 
		logdbg("ShmqWrapper init success[key %d]\n",g_stConf.iShmqueKey); 
	} 

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
