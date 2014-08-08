#ifndef  SESSIONCACHE_INC
#define  SESSIONCACHE_INC

#include "CacheDef.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#define SESSION_INIT_STAMP 0x20110622

template <class T>
struct SessionBlock
{
    int iSessionId;
    HMEM hNext;
    HMEM hPre;
    T data;
};

template<class T,int size>
class SessionCache
{
    public:
        SessionCache(){};
        ~SessionCache(){};
        T* Alloc(int& iSessionId);
        T* Get(int iSessionId);
        T* Set(int iSessionId,T* pData);
        int Del(int iSessionId);
        int Init(unsigned int uiSessionExpireTime);
        int Size(){return iCount;}
        int Capacity(){return size;}
        int FreeCapacity(); //using for test
        int Touch();
        class iterator:public std::iterator<std::input_iterator_tag,T>
        {       
            public: 
                iterator(SessionBlock<T>* pSB,HMEM h):mb(pSB),hCur(h){};
                T & operator *() const {return GET_MB(hCur)->data;}
                T * operator ->()const {return &(GET_MB(hCur)->data);}
                bool operator != (iterator it)const 
                {
                    return hCur != it.hCur; 
                }
                bool operator ==(iterator it )const 
                {
                    return hCur == it.hCur; 
                }
                iterator & operator++() 
                {
                    SessionBlock<T>* pSB=GET_MB(hCur);
                    if(pSB)
                    {
                        hCur=pSB->hNext;
                    }
                    return *this;
                } 
                iterator operator++(int) 
                {
                    iterator it=*this; 
                    ++*this;
                    return it;
                }
            private:
                SessionBlock<T>* mb;
                HMEM hCur;
        };      
        iterator begin()
        {
            return iterator(mb,hAlloc); 
        }
        iterator end(){return iterator(mb,INVALID);}
        enum{SESSION_REUSE=0,SESSION_RESET=1};
    private:
        HMEM hAlloc;
        HMEM hFree;
        SessionBlock<T> mb[size];  //base 1
        unsigned int uiStamp; 
        unsigned int uiActiveTime;
        int iCount;
};

template<class T,int size>
int SessionCache<T,size>::FreeCapacity()
{
    if(hFree==INVALID) 
        return 0;
    SessionBlock<T>* pSessionBlock=GET_MB(hFree); 
    int iSize=1;
    while(pSessionBlock->hNext!=INVALID)
    {
        pSessionBlock=GET_MB(pSessionBlock->hNext);
        iSize++;
    }
    return iSize;
}

template<class T,int size>
int SessionCache<T,size>::Touch()
{
    uiActiveTime=time(NULL);
    return 0;
}

template<class T,int size>
int SessionCache<T,size>::Init(unsigned int uiSessionExpireTime)
{
    unsigned int t=time(NULL);
    if(uiStamp!=SESSION_INIT_STAMP || uiActiveTime+uiSessionExpireTime<t)
    {
        memset(this,0,sizeof(*this));
        hFree=1;
        hAlloc=INVALID;
        for(int i=1;i<size;i++) 
            GET_MB(i)->hNext=i+1;
        for(int i=2;i<=size;i++) 
            GET_MB(i)->hPre=i-1;
        uiActiveTime=time(NULL);
        uiStamp=SESSION_INIT_STAMP;
        return SESSION_RESET;
    }
    return SESSION_REUSE;
}

template<class T,int size>
T* SessionCache<T,size>::Get(int iSessionId)
{
    HMEM h=iSessionId;
    SessionBlock<T>* pMemBlock=GET_MB(h);
    if(pMemBlock && pMemBlock->iSessionId==iSessionId)
    {
        return &(pMemBlock->data);
    }
    return NULL;
}

template<class T,int size>
T* SessionCache<T,size>::Alloc(int& iSessionId)
{
    HMEM hNew=hFree;
    SessionBlock<T>* pSB=GET_MB(hNew); 
    SessionBlock<T>* pNSB=GET_MB(hNew); 
    if(!pSB)
        return NULL;
    pSB->iSessionId=hNew;
    iSessionId=hNew;
    hFree=pSB->hNext;

    pSB->hNext=hAlloc;
    pSB->hPre=INVALID;
    pNSB=GET_MB(hAlloc);
    if(pNSB)
        pNSB->hPre=hNew;
    hAlloc=hNew;
    iCount++;
    return &(pSB->data);
}

template<class T,int size>
T* SessionCache<T,size>::Set(int iSessionId,T* pData)
{
    T* pDest=Get(iSessionId);
    if(pDest)
    {
        memcpy(pDest,pData,sizeof(T));
    }
    return pDest;
}

template<class T,int size>
int SessionCache<T,size>::Del(int iSessionId)
{
    HMEM h=iSessionId;
    SessionBlock<T>* pSB=GET_MB(h);
    if(pSB && pSB->iSessionId==iSessionId)
    {
        SessionBlock<T>* pPreSB=GET_MB(pSB->hPre);
        if(pPreSB)
            pPreSB->hNext=pSB->hNext;
        else
            hAlloc=pSB->hNext;
        SessionBlock<T>* pNextSB=GET_MB(pSB->hNext);
        if(pNextSB)
            pNextSB->hPre=pPreSB?pSB->hPre:INVALID;

        iCount--;
        pSB->iSessionId=0;
        pSB->hNext=hFree;
        hFree=h;
        return 0;
    }
    return -1;
}
#endif   /* ----- #ifndef SESSIONCACHE_INC  ----- */
/* vi:set ts=4: */ 

