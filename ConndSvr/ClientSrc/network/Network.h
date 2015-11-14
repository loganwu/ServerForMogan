#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <netinet/in.h>
#include "GSProtocol.pb.h"

class NetWork
{
public:
    NetWork();
    ~NetWork();
	enum {MAX_NET_BUFFER_SIZE = 256*1024};
	int InitSock(const char *szIp, const int iPort);
    
    int SendMsg();
	int RecMsg();
    
    int GetSockFd()
    {
        return m_iSockFd;   
    }

private:
	bool EncodeMsg(Connd::Protocol::GSPkg& pkg, char *buf,int iBufLen);
	bool DecodeMsg(Connd::Protocol::GSPkg& pkg, const char *buf , int iBufLen);
	bool SerializeMessage(const ::google::protobuf::Message& pkg, std::string& buf);
private:
    char m_sNetBuf[MAX_NET_BUFFER_SIZE];
    int m_iSockFd;
    struct sockaddr_in     m_stServerAddr;
	Connd::Protocol::GSPkg m_stGSPkg;
 
};

#endif
