#ifndef __SESSION_H__
#define __SESSION_H__

#include <stdint.h>
#include <string>
#include "SessionKey.h"

class Session
{
public:
	Session();
	~Session();

	int OnConnect();
	int OnClose();
	int SetAddr(uint32_t uIP, uint32_t uPort);
	int SetSockHandle(int iFD);
	SessionKey GetKey();
	int OnRead(char *pBuf, int &iBufLen);
	int OnWrite();
	int OnErrEvent(uint16_t events);
	const std::string & OnGetIPAddr();
	int OnGetPort();
private:
	void OnFreeBufEvent();
private:
	int m_iSockFD;
	uint32_t m_uIP;
	uint32_t m_uPort;
	std::string m_strIP;
	struct bufferevent *m_bufEvent;
};

#endif
