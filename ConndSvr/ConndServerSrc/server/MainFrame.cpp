/************************************************************************/
/*prog entrance                                                         */  
/************************************************************************/
#include "NetworkWrapper.h"
#include "SingletonHolder.h"
#include "log.h"

nt Init()
{
	SingletonHolder<NetworkWrapper>::Instance()->Init();
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
