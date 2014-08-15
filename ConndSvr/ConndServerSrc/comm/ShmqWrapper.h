#ifndef  _SHMQWRAPPER_H_
#define  _SHMQWRAPPER_H_

class ShmqWrapper
{
public:
	enum{ ELEMENT_SIZE = 64,
		 ELEMENT_COUNT = 100};
	ShmqWrapper();
	~ShmqWrapper();
	//iKey: shm key, iThreshold: noticify process when elements reach this Threshold
	int InitProducer(int iKey,int iThreshold);
	int InitConsumer(int iKey);
	int OnWrite(const char* pData, int iSize);
	int OnReadBlock(const char* pData, int iSize,int iTimeOut); // persist read, iTimeout second
	int OnReadNonBlock(const char* pData, int iSize); // nonblocking read
	int GetUsage();
	int GetUsedBlocks();
private:

	struct shm_queue *m_pShmQueue;
};

#endif   /* ----- #ifndef _SHMQWRAPPER_H_  ----- */