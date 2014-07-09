#include <arpa/inet.h>

#include "Session.h"
#include "NetworkWrapper.h"
#include "log.h"
#include "SingletonHolder.h"
#include "SessionMgr.h"
#include "MsgMgr.h"

/************************************************/
/*libevent cb def*/

/************************************************/

void conn_read_cb(struct bufferevent *bev, void *user_data)
{
	//switch to class function;
	Session *pSession = static_cast<Session*>(user_data);
	SingletonHolder<MsgMgr>::Instance()->GetOneConndMsg(pSession);
}

// void conn_writecb(struct bufferevent *bev, void *user_data)
// {
	//switch to class function;
//Session *pSession = static_cast<Session*>(user_data);
//pSession->OnWrite();
// 	struct evbuffer *output = bufferevent_get_output(bev);
// 	if (evbuffer_get_length(output) == 0)
// 	{
// 		logdbg("flushed answer\n");
// 	}
// }

void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	//switch to class function;
	Session *pSession = static_cast<Session*>(user_data);
	pSession->OnErrEvent(events);
}


Session::Session():m_iSockFD(-1),m_uIP(0),m_uPort(0),m_bufEvent(NULL)
{

}

Session::~Session()
{
	OnClose();
	OnFreeBufEvent();
}

int Session::OnConnect()
{
	logdbg("session connect[%s:%d]\n",m_strIP.c_str(), m_uPort);
 
 	m_bufEvent = bufferevent_socket_new(gstEvtBase, m_iSockFD, BEV_OPT_CLOSE_ON_FREE);
 	if (!m_bufEvent) 
 	{
 		logerr("Error constructing bufferevent!");
 		return -1;
 	}
 	bufferevent_setcb(m_bufEvent, conn_read_cb, NULL, conn_eventcb, this);
 	bufferevent_enable(m_bufEvent, EV_READ);
	return 0;
}

int Session::OnClose()
{
	logdbg("session close[%s:%d]\n",m_strIP.c_str(), m_uPort);
	if (m_iSockFD > 0)
	{
		close(m_iSockFD);
		m_iSockFD = -1;
	}
	return 0;
}

int Session::SetSockHandle( int iFD )
{
	m_iSockFD = iFD;
	return 0;
}

int Session::SetAddr( uint32_t uIP, uint32_t uPort)
{
	m_uIP = uIP;
	m_uPort = uPort;
	struct in_addr addr;
	addr.s_addr = htonl(m_uIP);
	m_strIP = std::string(inet_ntoa(addr));

	return 0;
}

 SessionKey Session::GetKey()
{
	SessionKey key;
	key.uPort = m_uPort;
	key.uIP = m_uIP;
	return key;
}

 int Session::OnRead(char *pBuf, int &iBufLen)
 {
	iBufLen = bufferevent_read(m_bufEvent,pBuf,iBufLen);

	return iBufLen;
 }

 int Session::OnWrite()
 {
	 return 0;
 }

 int Session::OnErrEvent( uint16_t events )
 {
	if (events & BEV_EVENT_EOF) 
	{
		logdbg("session closed[%s:%d]\n",m_strIP.c_str(), m_uPort);
	} else if (events & BEV_EVENT_ERROR) 
	{
		logdbg("Got an error on the connection[%s:%d] start \n",m_strIP.c_str(), m_uPort);
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	OnFreeBufEvent();
	//TODO add to tasklist to delay remove
	SingletonHolder<SessionMgr>::Instance()->Del(this);

	return 0;
 }

 void Session::OnFreeBufEvent()
 {
	 if (m_bufEvent)
	 {
		 bufferevent_free(m_bufEvent);
		 m_bufEvent = NULL;
	 }
 }

 const std::string & Session::OnGetIPAddr()
 {
	 return m_strIP;
 }

 int Session::OnGetPort()
 {
	 return m_uPort;
 }
