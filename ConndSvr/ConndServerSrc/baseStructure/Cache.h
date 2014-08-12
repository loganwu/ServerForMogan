#include "CacheDef.h"
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>

#ifndef  CACHE_INC
#define  CACHE_INC


#define CACHE_CHECK_SUM 0xAA8AA6AA 
#define CACHE_SIGN(uiInitSign) ((((uint64_t)uiInitSign)<<32)|CACHE_CHECK_SUM)

template <class T,class K>
struct MemBlock
{
    HMEM hNext;
    K key;
    T data;
};

template<class T,class K,int size,int hashsize>
class MemCache
{
    public:
        MemCache(){};
        ~MemCache(){};
        T* Alloc(K key,int& iRet);
        T* Get(K key);
        T* Set(K key,T* pData,int& iRet);
        T* GetByHandle(HMEM h);
        int Del(K key);
        int Init(unsigned int uiInitSign);
        int Erase(unsigned int uiInitSign);
        int Size(){return iCount;}
        int Capacity(){return size;}
        int FreeCapacity(); //using for test
        class iterator:public std::iterator<std::input_iterator_tag,T>
        {       
            public: 
                iterator(int iIdx,MemBlock<T,K>* pMb,int iSize):iIdx(iIdx),pMb(pMb),iSize(iSize){};
                iterator():iIdx(0),pMb(NULL),iSize(0){};
                T & operator *() const {return pMb[iIdx].data;}
                T * operator ->()const {return &(pMb[iIdx].data);}
                K get_key(){return pMb[iIdx].key;}
                bool operator != (iterator it)const 
                {
                    return iIdx != it.iIdx; 
                }
                bool operator ==(iterator it )const 
                {
                    return iIdx == it.iIdx; 
                }
                iterator & operator++() 
                {
                    while(iIdx<iSize)
                    {
                        iIdx++;
                        if(iIdx<iSize && pMb[iIdx].key>0)
                            return *this;
                    }
                    return *this;
                } 
                iterator & operator--() 
                {
                    while(iIdx>0)
                    {
                        iIdx--;
                        if(pMb[iIdx].key>0)
                            return *this;
                    }
                    return *this;
                } 
                iterator operator--(int) 
                {
                    iterator it=*this; 
                    --*this;
                    return it;
                }
                iterator operator++(int) 
                {
                    iterator it=*this; 
                    ++*this;
                    return it;
                }
                HMEM GetHandle(){return iIdx+1;}
            private:
                int iIdx;
                MemBlock<T,K>* pMb;
                int iSize;
        };      

        iterator begin()
        {
            iterator it(0,mb,size); 
            if(mb[0].key==0)
                it++;
            return it;
        }
        iterator end(){return iterator(size,mb,size);}
        enum{CACHE_HIT=0,CACHE_ADD=1,CACHE_NONE=-1,CACHE_INVALID_KEY=-2,CACHE_FULL=-3};
    private:
        HMEM hash[hashsize];
        HMEM hFree;
        MemBlock<T,K> mb[size];  //base 1
        unsigned int uiVer;
        unsigned int uiPreserved;
        uint64_t ullSignature; 
        int iCount;
};

template<class T,class K,int size,int hashsize>
int MemCache<T,K,size,hashsize>::FreeCapacity()
{
    if(hFree==INVALID) 
        return 0;
    MemBlock<T,K>* pMemBlock=GET_MB(hFree); 
    int iSize=1;
    while(pMemBlock->hNext!=INVALID)
    {
        pMemBlock=GET_MB(pMemBlock->hNext);
        iSize++;
    }
    return iSize;
}

template<class T,class K,int size,int hashsize>
int MemCache<T,K,size,hashsize>::Erase(unsigned int uiInitSign)
{
    memset(this,0,sizeof(*this));
    hFree=1;
    for(int i=1;i<size;i++) 
        GET_MB(i)->hNext=i+1;
    ullSignature=CACHE_SIGN(uiInitSign);
    return 0;
}


template<class T,class K,int size,int hashsize>
int MemCache<T,K,size,hashsize>::Init(unsigned int uiInitSign)
{
    if(ullSignature==CACHE_SIGN(uiInitSign))
        return 0;
    return Erase(uiInitSign);
}

template<class T,class K,int size,int hashsize>
T* MemCache<T,K,size,hashsize>::GetByHandle(HMEM h)
{
    MemBlock<T,K>* pMemBlock=GET_MB(h);
    if(pMemBlock)
    {
        return &(pMemBlock->data);
    }
    return NULL;
}

template<class T,class K,int size,int hashsize>
T* MemCache<T,K,size,hashsize>::Get(K key)
{
    if(key==0)
    {
        return NULL;
    }
    HMEM h=hash[key%hashsize];
    MemBlock<T,K>* pMemBlock=NULL;
    while(h!=INVALID)
    {
        pMemBlock=GET_MB(h);
        if(pMemBlock->key==key)
        {
            return &(pMemBlock->data);
        }
        h=pMemBlock->hNext;
    }
    return NULL;
}

template<class T,class K,int size,int hashsize>
T* MemCache<T,K,size,hashsize>::Alloc(K key,int& iRet)
{
    if(key==0)
    {
        iRet=CACHE_INVALID_KEY;
        return NULL;
    }
    T* pDest=Get(key);
    if(pDest)
    {
        iRet=CACHE_HIT;
        return pDest;
    }

    if(hFree==INVALID) 
    {
        iRet=CACHE_FULL;
        return NULL;
    }

    HMEM hAlloc=hFree;
    MemBlock<T,K>* pMemBlock=GET_MB(hAlloc); 
    pMemBlock->key=key;
    hFree=pMemBlock->hNext;
    HMEM* pHmem=&(hash[key%hashsize]);
    pMemBlock->hNext=*pHmem;
    *pHmem=hAlloc;
    iCount++;
    iRet=CACHE_ADD;
    return &(pMemBlock->data);
}

template<class T,class K,int size,int hashsize>
T* MemCache<T,K,size,hashsize>::Set(K key,T* pData,int& iRet)
{
    T* pDest=Alloc(key,iRet);
    if(pDest)
    {
        memcpy(pDest,pData,sizeof(T));
    }
    return pDest;
}

template<class T,class K,int size,int hashsize>
int MemCache<T,K,size,hashsize>::Del(K key)
{
    if(key==0)
    {
        return 0;
    }
    HMEM* pHmem=&(hash[key%hashsize]);
    HMEM hMem=*pHmem;
    if(*pHmem!=INVALID)
    {
        MemBlock<T,K>* pMemBlock=GET_MB(hMem); 
        if(pMemBlock->key==key)
        {
            *pHmem=pMemBlock->hNext;
            pMemBlock->hNext=hFree;
            pMemBlock->key=0; 
            hFree=hMem;
            iCount--;
            return 1;
        }
        MemBlock<T,K>* pPreMemBlock=NULL;
        HMEM hDel=INVALID;
        while(pMemBlock->hNext!=INVALID)
        {
            pPreMemBlock=pMemBlock;
            pMemBlock=GET_MB(pMemBlock->hNext);
            if(pMemBlock->key==key)
            {
                hDel=pPreMemBlock->hNext;
                pPreMemBlock->hNext=pMemBlock->hNext;
                pMemBlock->hNext=hFree;
                pMemBlock->key=0;
                hFree=hDel;
                iCount--;
                return 1;
            }
        }
    }
    return 0;
}


#endif   /* ----- #ifndef CACHE_INC  ----- */

