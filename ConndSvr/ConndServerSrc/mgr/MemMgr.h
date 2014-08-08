#ifndef  MEMMGR_INC
#define  MEMMGR_INC

#include "Cache.h"
#include "SessionCache.h"
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "SortedVector.h"
#include "CommDef.h"
#include "RoleInfo.h"
#include "SessionInfo.h"

using namespace std;

typedef MemCache<RoleInfo,uint64_t,MAX_USERS,121151> RoleCache; //all role cache
typedef SessionCache<SessionInfo,MAX_SESSIONS> ClientSessionCache; //all client session


struct ShmAlloc
{
    struct
    {
        int preserved[256];
    }preserved;
    RoleCache roleCache;
	//HeroCache heroCache;

};


class MemMgr
{
    public:
        MemMgr();
        ~MemMgr();
        int Init();
        void DumpInfo(ostream& os);
        ClientSessionCache* GetSessionCache()
        {
            return m_pSessionCache;
        }
        RoleCache* GetRoleCache()
        {
            return m_pRoleCache;
        }
		
        int GetRoleCacheUsedRate();
    private:        
        char* AllocShm(long iSize,int iShmKey);
        ClientSessionCache* m_pSessionCache;
		RoleCache* m_pRoleCache;
};





#endif   /* ----- #ifndef MEMMGR_INC  ----- */

