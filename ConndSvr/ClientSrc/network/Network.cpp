#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <iostream>

#include "log.h"
#include "Network.h"


using namespace Connd::Protocol;
using namespace Connd::Common;
using namespace Connd::Data;
using namespace std;

NetWork::NetWork()
{
	memset(m_sNetBuf,0,sizeof(m_sNetBuf));
}


NetWork::~NetWork()
{
	google::protobuf::ShutdownProtobufLibrary();
}

int NetWork::InitSock(const char *szIp, const int iPort)
{
	if ((NULL == szIp) || (iPort <= 0))
	{
		return -1;
	}

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	m_iSockFd = socket(AF_INET, SOCK_STREAM, 0);

	if (m_iSockFd < 0)
	{
		logerr("Client socket init fail\n");
		return -1;
	}

	memset(&m_stServerAddr, 0, sizeof(m_stServerAddr));
	m_stServerAddr.sin_family = AF_INET;
	m_stServerAddr.sin_port   = htons(iPort);
	m_stServerAddr.sin_addr.s_addr = inet_addr(szIp);     

	if ( connect(m_iSockFd, (struct sockaddr *)&m_stServerAddr, sizeof(m_stServerAddr)) != 0)
	{
		logerr("Client connect fail\n");
		return -2;
	}

	logdbg("ImAgent socket init ok.");

	return 0;
}

int NetWork::SendMsg()
{
	memset(m_sNetBuf,0,sizeof(m_sNetBuf));

 	Connd::Protocol::GSHeader *stHeader = m_stGSPkg.mutable_header();
 	Connd::Protocol::GSBody *stBody = m_stGSPkg.mutable_body();
	stHeader->set_cmdid(Connd::Common::CS_LOGIN);
	stHeader->set_version(1);
	stHeader->set_packagelen(0);
	Connd::Data::CSLoginData *stCSLoginData = stBody->mutable_cslogindata();
	stCSLoginData->mutable_user()->set_qq(12345678);
	stCSLoginData->mutable_user()->set_appid(88888);;
	int iSendLen = m_stGSPkg.ByteSize();
	stHeader->set_packagelen(iSendLen);
	logdbg("sendlen %d\n",iSendLen);
	EncodeMsg(m_stGSPkg,m_sNetBuf,iSendLen);

	if (sendto(m_iSockFd,m_sNetBuf,iSendLen,0,
		(struct sockaddr *)&m_stServerAddr,sizeof(m_stServerAddr)) < 0)
	{
		logerr("Send err\n");
	}
	
	return 0;
}


bool NetWork::EncodeMsg(Connd::Protocol::GSPkg& pkg, char *buf,int iBufLen)
{
	if (!pkg.SerializeToArray(buf,iBufLen))
	{
		logerr("SerializeToArray failed for pkg\n");
		return false;
	}

	return true;
}

bool NetWork::DecodeMsg(Connd::Protocol::GSPkg& pkg, const char *buf , int iBufLen)
{
	if (!pkg.ParseFromArray(buf, iBufLen))
	{
		logerr("parse pkg from buf failed\n");
		return false;
	}

	return true;
}

bool NetWork::SerializeMessage(const ::google::protobuf::Message& pkg, std::string& buf)
{
	std::ostringstream oss;

// 	for (size_t i = 0; i < LEN_SIZE; ++i)
// 	{
// 		oss << ' ';
// 	}
// 
// 	oss << "{{";

	if (!pkg.SerializeToOstream(&oss))
	{
		logerr("SerializeToOstream() failed for pkg\n");
		return false;
	}

	//oss << "}}";
	buf = oss.str();

	return true;
}