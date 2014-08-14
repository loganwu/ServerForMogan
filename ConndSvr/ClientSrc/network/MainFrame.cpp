#include "Network.h"
#include <unistd.h>
#include <stdio.h>
#include "ShmqWrapper.h"


int main(int argc, char**argv)
{
	NetWork stNetWork;
	ShmqWrapper stShmq;
	const char sIP[] = "127.0.0.1";
	int iPort = 9999;
	char buff[128] = {0};
	int iLen = 0;

	stNetWork.InitSock(sIP,iPort);
	stNetWork.SendMsg();
	stShmq.InitConsumer(20140814);
	sleep(1);
	stNetWork.RecMsg();
	stShmq.OnRead(buff,iLen);
	printf("Read from shmq buf: %s\n",buff);
// 	while(stNetWork.RecMsg() < 0)
// 	{
// 		sleep(1);
// 		stNetWork.RecMsg();
// 
// 	}
	printf("Client Termined\n");
	return 0;
}
