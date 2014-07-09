#include "SessionMgr.h"
#include "log.h"
#include <string.h>

SessionMgr::SessionMgr()
{

}

SessionMgr::~SessionMgr()
{
	Clear();
}

Session * SessionMgr::Get( uint32_t uIP,uint32_t uPort )
{
	SMAP::iterator it;
	SessionKey stKey;
	stKey.uIP = uIP;
	stKey.uPort = uPort;
	it = m_stSessionMap.find(stKey);
	if (it != m_stSessionMap.end())
	{
		return it->second;
	}
	else
	{
		Session *session = new(std::nothrow) Session();
		if (!session)
		{
			logerr("New sesion failed!\n");
			return NULL;
		}
		session->SetAddr(uIP,uPort);
		m_stSessionMap.insert(std::pair<SessionKey, Session *>(stKey, session));
		return session;
	}
}

int SessionMgr::Clear()
{
	logdbg("Clear All Session\n");
	SMAP::iterator it;
	for (it = m_stSessionMap.begin(); it != m_stSessionMap.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	m_stSessionMap.clear();
	return 0;
}

int SessionMgr::Del( SessionKey stKey )
{
	SMAP::iterator it;
	it = m_stSessionMap.find(stKey);
	if (it != m_stSessionMap.end())
	{
		m_stSessionMap.erase(it);
		return 1; //remove success
	}
	else
	{
		return 0; //missed
	}
}

int SessionMgr::Del( Session *pSession )
{
	if (!pSession)
	{
		logerr("pSession NULL\n");
	}
	logdbg("session delete[%s:%d]\n",pSession->OnGetIPAddr().c_str(), pSession->OnGetPort());
	Del((pSession->GetKey()));
	return 0;
}

int SessionMgr::TimeOutTask()
{

}
