#include "Network.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char**argv)
{
	NetWork stNetWork;
	const char sIP[] = "127.0.0.1";
	int iPort = 9999;

	stNetWork.InitSock(sIP,iPort);
	stNetWork.SendMsg();
	sleep(1);
	stNetWork.RecMsg();
// 	while(stNetWork.RecMsg() < 0)
// 	{
// 		sleep(1);
// 		stNetWork.RecMsg();
// 
// 	}
	printf("Client Termined\n");
	return 0;
}
