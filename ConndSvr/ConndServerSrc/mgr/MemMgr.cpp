#include "MemMgr.h"
#include "log.h"

MemMgr::MemMgr()
{
}

MemMgr::~MemMgr()
{
}

char* MemMgr::AllocShm(long iSize,int iShmKey)
{
    int shmid;
    char* p=NULL;
    if ((shmid = shmget(iShmKey,0,0)) < 0)
    {
        logdbg("creating new shm\n");
        if ((shmid = shmget(iShmKey,iSize, IPC_CREAT | 0777)) < 0)
        {
            cout<<"Init shm failure! may be you should check your system shm limit!"<<endl;
            return NULL;
        }
    }
    else
    {
        //using exist shm
        logdbg("using exist shm:%d\n",iShmKey);
    }
    //check shm size first
    shmid_ds dsbuf; 
    if((shmctl(shmid,IPC_STAT,&dsbuf))<0)
    {
        logerr("shmctl failure\n");
        return NULL;
    }
    logdbg("shm size(%ld),need size(%ld).\n",
            dsbuf.shm_segsz,
            iSize);
    if((long)dsbuf.shm_segsz!=iSize)
    {
        logerr("shm size don't fit the need size,you should check you server edtion,or drop shm and rebood.\n");
        return NULL;
    }
    if ((p=(char*)shmat(shmid, 0 ,0)) == (void *) -1)
        return NULL;
    return p;
}

int MemMgr::Init()
{
    int iSessionShmKey=0;
    int iSessionShmExpireTime=0;

	logdbg("init cache in share memory size %ldM.\n",sizeof(ShmAlloc)/1024/1024);
	ShmAlloc* pShmAlloc=(ShmAlloc*)AllocShm(sizeof(ShmAlloc),20140808);
	if(!pShmAlloc)
		return -1;
	m_pRoleCache=&(pShmAlloc->roleCache);


    if(iSessionShmKey>0)
    {
        logdbg("init session in shm.size:%ldM\n",sizeof(ClientSessionCache)/1024/1024);
        m_pSessionCache=(ClientSessionCache*)AllocShm(sizeof(ClientSessionCache),iSessionShmKey);
    }
    else
    {
        logdbg("init session in mem.\n");
        m_pSessionCache=new ClientSessionCache;
    }

    if(!m_pSessionCache || !m_pRoleCache)
    {
        return -1;
    }

    if(m_pSessionCache->Init(iSessionShmExpireTime)==ClientSessionCache::SESSION_REUSE)
    {
        logdbg("Session reuse.\n");
    }
    else
    {
        logdbg("Session erase.\n");
    }
    m_pRoleCache->Init(2014080801);
    DumpInfo(cout);
    return 0;
}

#define _DUMP_SIZE(x) "\t USING MEMORY:"<<sizeof(x)/1024/1024<<"M"<<endl; 

void MemMgr::DumpInfo(ostream& os)
{
//     os<<"=========================MEMORY INFO==========================="<<endl;
//     os<<"MAX ONLINE USER NUM:"<<MAX_SESSION_NUM<<endl;
//     os<<"MAX SERVER USER NUM:"<<MAX_USER_NUM<<endl;
// 
//     os<<"SESSION CACHE SIZE:"<<m_pSessionCache->Capacity()<<_DUMP_SIZE(ClientSessionCache);
//     os<<"ROLE  CACHE SIZE:"<<m_pRoleCache->Capacity()<<_DUMP_SIZE(RoleCache);
//     os<<"HERO  CACHE SIZE:"<<m_pHeroCache->Capacity()<<_DUMP_SIZE(HeroCache);
// 	os<<"RANK  CACHE SIZE:"<<"sizeof(RankInfo)/sizeof(RankData):"<<sizeof(RankInfo)<<"/"<<sizeof(RankData)<<";total:"<<m_pRankCache->Capacity()<<_DUMP_SIZE(RankCache);
//     os<<"TOTAL CACHE SIZE:"<< (sizeof(ClientSessionCache)+
//             sizeof(RoleCache)+
//             sizeof(RankCache)+
//             sizeof(HeroCache))/1024/1024 <<"M"<<endl;
}

int MemMgr::GetRoleCacheUsedRate()
{
    if(!m_pRoleCache)
		return -1;
	return m_pRoleCache->Size()*100/(m_pRoleCache->Capacity()+1);
}

