#ifndef _NETWORK_WRAPPER_H_
#define  _NETWORK_WRAPPER_H_

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"

extern struct event_base *gstEvtBase;

class NetworkWrapper
{
public:
	//NetworkWrapper:m_iPort(iPort)(int iPort){};
	NetworkWrapper();
	~NetworkWrapper();
public:
	int Init();
	int OnRun();
	int OnAAccept(uint32_t uIP, uint32_t uPort,int iFD);
	int Tick();
private:
	
private:
	int m_iPort;
	struct evconnlistener *m_stEvtlistener;
	struct event *m_stSigEvt;
	struct event *m_stTimeEvt;
};

#endif
