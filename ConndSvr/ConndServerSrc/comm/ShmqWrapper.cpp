#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

#include "ShmqWrapper.h"
#include "SingletonHolder.h"
#include "shm_queue.h"
#include "log.h"


ShmqWrapper::ShmqWrapper()
{

}

ShmqWrapper::~ShmqWrapper()
{
	if (m_pShmQueue)
	{
		sq_destroy(m_pShmQueue);
	}
}

int ShmqWrapper::InitProducer( int iKey )
{
	m_pShmQueue = NULL;
	m_pShmQueue = sq_create(iKey, ELEMENT_SIZE, ELEMENT_COUNT, 1, 1);
	if (!m_pShmQueue)
	{
		logerr("shm queue InitProducer failure: %s\n",sq_errorstr(NULL));
		return -1;
	}
	return 0;
}

int ShmqWrapper::InitConsumer( int iKey )
{
	m_pShmQueue = NULL;
	m_pShmQueue = sq_open(iKey);
	if (!m_pShmQueue)
	{
		logerr("shm queue InitConsumer failure: %s\n",sq_errorstr(NULL));
		return -1;
	}
	return 0;
}

int ShmqWrapper::Onwrite( const char* pData, int iSize )
{
	if(sq_put(m_pShmQueue, (void *)pData, iSize) != 0)

	{
		logerr("Shmqueue write failure: %s\n",sq_errorstr(m_pShmQueue));
		return -1;
	}
	return 0;
}

int ShmqWrapper::OnRead( const char* pData, int iSize )
{
	int event_fd = sq_get_eventfd(m_pShmQueue);

	while(true)
	{
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(event_fd, &fdset);
		struct timeval to = {1, 0}; // 1s
		sq_sigon(m_pShmQueue); // now we are entering into sleeping sys call
		int ret = select(event_fd+1, &fdset, NULL, NULL, &to);
		sq_sigoff(m_pShmQueue); // no longer needs signal
		if(ret<0)
		{
			continue;// error
		}
		if(FD_ISSET(event_fd, &fdset)) 
			sq_consume_event(m_pShmQueue);
		int len = sq_get(m_pShmQueue, (void*)pData, iSize,NULL);
		if(len<0) // ¶ÁÊ§°Ü
		{
		}
		else if(len==0) // empty
		{
			return 0;
		}
		else // get data
		{
			logdbg("Receive data: len %d\n",len);
			return len;
		}
	}
}

