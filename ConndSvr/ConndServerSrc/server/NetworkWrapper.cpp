#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "NetworkWrapper.h"
#include "log.h"
#include "SingletonHolder.h"
#include "Session.h"
#include "SessionMgr.h"

struct event_base *gstEvtBase;

NetworkWrapper::NetworkWrapper()
{
	m_iPort = 9999;
}

NetworkWrapper::~NetworkWrapper()
{
	if (m_stEvtlistener)
	{
		evconnlistener_free(m_stEvtlistener);
	}
	if (gstEvtBase)
	{
		event_base_free(gstEvtBase);
	}
	if (m_stSigEvt)
	{
		event_free(m_stSigEvt);
	}
}

void timer_cb(int sock, short event, void *user_data)
{
	logdbg("Tick\n");
	NetworkWrapper *pNetWp = (NetworkWrapper *)user_data;
	pNetWp->Tick();

}

void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base*)user_data;
	struct timeval delay = { 2, 0 };

	logdbg("Caught an interrupt signal; exiting cleanly in two seconds.\n");
	//cleanup task
	SingletonHolder<SessionMgr>::Instance()->Clear();
	event_base_loopexit(base, &delay);
}


void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
// 	struct event_base *base = evconnlistener_get_base(listener);
// 	struct bufferevent *bev = NULL;
// 
// 	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
// 	if (!bev) 
// 	{
// 		logerr("Error constructing bufferevent!");
// 		//event_base_loopbreak(base);
// 		return;
// 	}
// 	bufferevent_setcb(bev, conn_read_cb, NULL, conn_eventcb, &fd);
// 	bufferevent_enable(bev, EV_READ);
	struct sockaddr_in *stScokAddr = (struct sockaddr_in *)sa;
	uint32_t ip = ntohl(stScokAddr->sin_addr.s_addr);
	uint32_t port = ntohs(stScokAddr->sin_port);
	SingletonHolder<NetworkWrapper>::Instance()->OnAAccept(ip,port,fd);

}

void accept_error_cb(struct evconnlistener *listener, void *user_data)
{
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        logerr("Got an error %d (%s) on the listener. "
                "Shutting down.\n", err, evutil_socket_error_to_string(err));

        event_base_loopexit(base, NULL);
}

int NetworkWrapper::Init()
{
	struct sockaddr_in sin;

	gstEvtBase = event_base_new();
	if (!gstEvtBase) 
	{
		logerr("Could not initialize libevent!\n");
		return -1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_iPort);

	m_stEvtlistener = evconnlistener_new_bind(gstEvtBase, accept_conn_cb, (void *)gstEvtBase,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sin,
		sizeof(sin));

	if (!m_stEvtlistener)
	{
		logerr("Could not create a listener!\n");
		return -1;
	}
	evconnlistener_set_error_cb(m_stEvtlistener, accept_error_cb);
	m_stSigEvt = evsignal_new(gstEvtBase, SIGINT, signal_cb, (void *)gstEvtBase);

	if (!m_stSigEvt || event_add(m_stSigEvt, NULL)<0) {
		logerr("Could not create/add a signal event!\n");
		return -1;
	}

	/*add tick*/
	struct timeval tv; 
	tv.tv_sec = 1;  
	tv.tv_usec = 0;

	m_stTimeEvt = evtimer_new(gstEvtBase,timer_cb, (void *)this);

	if (!m_stTimeEvt || event_add(m_stTimeEvt, &tv)<0) 
	{
		logerr("Could not create/add a time event!\n");
		return -1;
	}

	return 0;
}

int NetworkWrapper::OnRun()
{
	event_base_dispatch(gstEvtBase);
	return 0;
	
}

int NetworkWrapper::OnAAccept( uint32_t uIP, uint32_t uPort,int iFD )
{
	Session *pSession = SingletonHolder<SessionMgr>::Instance()->Get(uIP,uPort);
	if (!pSession)
	{
		logerr("Session Full!\n");
		close(iFD);
		return -1;
	}
	pSession->SetSockHandle(iFD);
	pSession->OnConnect();
	return 0;
}

int NetworkWrapper::Tick()
{
	struct timeval tv;  
	tv.tv_sec = 1;  
	tv.tv_usec = 0;   
	event_add(m_stTimeEvt, &tv);  

	return 0;
}
