#ifndef  SORTEDVECTOR_INC
#define  SORTEDVECTOR_INC

#include <stdio.h>

template<class T,int N>
struct SortedVector 
{
    int iCount; 
    T data[N];
    SortedVector()
    {
        Reset();
    }
    void Reset()
    {
        iCount=0;
		memset(data,0,sizeof(data));
    }
    bool IsFull()
    {
        return (iCount==N);
    }
    int insert(T key)
    {
        int low=0;
        int high=iCount-1; 
        int mid=0;   
        if(iCount>=N)
            return -1;
        if(iCount==0)
        {
            data[0]=key;
            iCount=1;
            return 0;
        }
        while (low <= high)   
        {        
            mid=(low+high)/2;
            if (data[mid]==key)        
                return -1;
            if (data[mid] > key)
                low=mid+1;
            else    
                high = mid -1;
        }
        if(key<data[mid])
            mid++;
        memmove(&(data[mid+1]),&(data[mid]),sizeof(T)*(iCount-mid));
        data[mid]=key;
        iCount++;
        return 0;
    }
    int remove(T key)
    {
        int idx=search(key);
        if(idx<0)
            return -1;
        if(iCount-idx-1>0)
            memmove(&data[idx],&data[idx+1],sizeof(T)*(iCount-idx-1));
        iCount--;
        return 0;
    }
    int search(T key)
    {
        int low=0;
        int high=iCount-1; 
        int mid=0;   
        while (low <= high)   
        {        
            mid=(low+high)/2;
            if (data[mid]==key)        
                return mid;
            if (data[mid] > key)
                low=mid+1;
            else    
                high = mid -1;
        }
        return -1;
    }
    int capacity()
    { 
        return N;
    }
	int size()
	{
        return iCount;
	}
	T* GetByIdx(int iIdx)
	{
       if(iIdx<0 || iIdx>=N)
	   	  return NULL;
	   return &(data[iIdx]);
	}
    void dump()
    {
        printf("===count %d====\n",iCount);
        for(int i=0;i<iCount;i++)
            printf(" %d%s",data[i],((i+1)%15)==0?"\n":"");
        printf("\n===============\n");
    }
};

#endif   /* ----- #ifndef SORTEDVECTOR_INC  ----- */

