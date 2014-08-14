#ifndef  _SHMQWRAPPER_H_
#define  _SHMQWRAPPER_H_

class ShmqWrapper
{
public:
	enum{ ELEMENT_SIZE = 64,
		 ELEMENT_COUNT = 100};
	ShmqWrapper();
	~ShmqWrapper();
	int InitProducer(int iKey);
	int InitConsumer(int iKey);
	int OnWrite(const char* pData, int iSize);
	int OnRead(const char* pData, int iSize);
	int GetUsage();
	int GetUsedBlocks();
private:

	struct shm_queue *m_pShmQueue;
};

#endif   /* ----- #ifndef _SHMQWRAPPER_H_  ----- */