/************************************************************************/
/*prog entrance                                                         */  
/************************************************************************/
#include "NetworkWrapper.h"
#include "SingletonHolder.h"
#include "log.h"


#if 0
void Tick(int sock, short event, void *arg)
{
	logdbg("Tick\n"); 

	struct timeval tv;  
	tv.tv_sec = 1;  
	tv.tv_usec = 0;   
	event_add((struct event*)arg, &tv);  
}

int AddTick()
{
	
	struct timeval tv; 
	struct event  *stTimeEvt = NULL;
	tv.tv_sec = 1;  
	tv.tv_usec = 0;

	if (!gstEvtBase)
	{
		logerr("gstEvtBase NULL");
	}
	stTimeEvt = evtimer_new(gstEvtBase,Tick, (void *)stTimeEvt);

	if (!stTimeEvt || event_add(stTimeEvt, &tv)<0) 
	{
		logerr("Could not create/add a time event!\n");
		return -1;
	}
	return 0;
}
#endif

int Init()
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
