#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

#include "ShmqWrapper.h"
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

int ShmqWrapper::InitProducer( int iKey,int iThreshold)
{
	m_pShmQueue = NULL;
	m_pShmQueue = sq_create(iKey, ELEMENT_SIZE, ELEMENT_COUNT, iThreshold, 1);
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

int ShmqWrapper::OnWrite( const char* pData, int iSize )
{
	if(sq_put(m_pShmQueue, (void *)pData, iSize) != 0)

	{
		logerr("Shmqueue write failure: %s\n",sq_errorstr(m_pShmQueue));
		return -1;
	}
	return 0;
}

int ShmqWrapper::OnReadBlock( const char* pData, int iSize, int iTimeOut)
{
	int event_fd = sq_get_eventfd(m_pShmQueue);
	struct timeval tv;

	while(true)
	{
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(event_fd, &fdset);
		struct timeval to = {iTimeOut, 0}; // 1s
		sq_sigon(m_pShmQueue); // now we are entering into sleeping sys call
		int ret = select(event_fd+1, &fdset, NULL, NULL, &to);
		sq_sigoff(m_pShmQueue); // no longer needs signal
		if(ret<0)
		{
			if (!(errno == EINTR || errno==EAGAIN)) // error
			{
				logerr("OnReadBlock select error\n");
			}
			else
			{
				logdbg("OnReadBlock select Intterupt or try again\n");
			}
			continue;
		}
		else if (ret == 0) //timeout
		{
			logdbg("OnReadBlock time out\n");
			return 0;
		}
		if(FD_ISSET(event_fd, &fdset)) 
			sq_consume_event(m_pShmQueue);
		int len = sq_get(m_pShmQueue, (void*)pData, iSize,&tv);
		if(len<0) // read failure
		{
			logerr("ShmRead Failuer:%s\n",sq_errorstr(m_pShmQueue));
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

int ShmqWrapper::OnReadNonBlock( const char* pData, int iSize )
{
	struct timeval tv;
	int len = sq_get(m_pShmQueue, (void*)pData, iSize,&tv);
	if(len<0) // read failure
	{
		logdbg("ShmRead Failuer:%s\n",sq_errorstr(m_pShmQueue));
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

int ShmqWrapper::GetUsage()
{
	return sq_get_usage(m_pShmQueue);
}

int ShmqWrapper::GetUsedBlocks()
{
	return sq_get_used_blocks(m_pShmQueue);
}



