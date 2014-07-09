#ifndef __SESSION_MGR_H__
#define __SESSION_MGR_H__

#include <map>
#include "Session.h"
#include "SessionKey.h"

class SessionMgr
{
	typedef std::map<SessionKey, Session *> SMAP;
public:
	SessionMgr();
	~SessionMgr();
	Session *Get(uint32_t uIP,uint32_t uPort);
	int Set(uint32_t uIP,uint32_t uPort);
	int Clear();
	int Del( SessionKey stKey );
	int Del( Session *pSession );
	int Tick();
	int TimeOutTask();

private:
	SMAP m_stSessionMap;

};

#endif
