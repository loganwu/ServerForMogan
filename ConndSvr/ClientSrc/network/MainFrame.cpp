#include "Network.h"
#include <unistd.h>

int main(int argc, char**argv)
{
	NetWork stNetWork;
	const char sIP[] = "127.0.0.1";
	int iPort = 9999;

	stNetWork.InitSock(sIP,iPort);
	stNetWork.SendMsg();
	sleep(1);
	while(stNetWork.RecMsg() < 0)
	{
		sleep(1);
	}
#if 0
	while (fgets(sendline, 10000,stdin) != NULL)
	{
		sendto(sockfd,sendline,strlen(sendline),0,
			(struct sockaddr *)&servaddr,sizeof(servaddr));
		n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
		recvline[n]=0;
		fputs(recvline,stdout);
	}
#endif

	return 0;
}
