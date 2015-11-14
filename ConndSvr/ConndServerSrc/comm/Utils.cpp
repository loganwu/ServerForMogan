#include "Utils.h"

void Utils::GetRandomListsWithFixedSum(int *ivOutput, int iCount, int iSum, int iMin, int iMax)
{
    int iNewSum = (iSum - iCount*iMin);
	int iNewMax = iMax - iMin;
	int iAverage = iSum/iCount;	
	int iTotal = 0;
	int remainder = 0;
	srand( (unsigned)time(NULL) );
	for (int i = 0; i < iCount;i++)
	{
		ivOutput[i] = rand()%(iNewMax+1);
		iTotal += ivOutput[i];
	}
    if( iTotal == 0 )
    {
        for (int i = 0; i < iCount;i++)
		{
			ivOutput[i] = iAverage;
		}
		remainder = iSum - (iAverage*iCount);
		if (0 != remainder)
		{
            int r = rand(); // 只要remainder^iCount<RAND_MAX，所以一个rand()就够了
            int iPos = 0;
            for( int i=0; i<remainder;i++)
	        {
                iPos = r%iCount;
            	if (ivOutput[iPos] >= iMax)
            	{
            		for (int j = 1; j < iCount;j++)
            	    {
            			iPos = (r+j)%iCount;
            			if (ivOutput[iPos] < iMax)
                  		{
            		        break;
                        }
            	     }
                }
                ++ivOutput[iPos];
                r /= iCount;
	         }
        }
    }
    else
    {
        // 将各项等比率扩张或缩小，使之等于iSum
        remainder = iSum;
        for (int i = 0;i < iCount;i++)
        {
        	ivOutput[i] = ivOutput[i] * iNewSum/iTotal + iMin;
        	if (ivOutput[i] >= iMax)
        	{
        		ivOutput[i] = iMax;
        	}
        	remainder -= ivOutput[i];
        }
        if( 0!= remainder)
        {
            int r = rand(); // 只要remainder^iCount<RAND_MAX，所以一个rand()就够了
            int iPos = 0;
            for( int i=0; i<remainder;i++)
	        {
                iPos = r%iCount;
            	if (ivOutput[iPos] >= iMax)
            	{
            		for (int j = 1; j < iCount;j++)
            	    {
            			iPos = (r+j)%iCount;
            			if (ivOutput[iPos] < iMax)
                  		{
            		        break;
                        }
            	     }
                }
                ++ivOutput[iPos];
                r /= iCount;
	         }
        }
    }
}


/*
Produce iCount of different integers range from 1 ~ iMax by weight factor
*/
int Utils::GetRandomListsNoDuplication(unsigned char *ivOutput, int iCount,int iMax, int *pRateField)
{
    if (iMax > MAX_HERO_NPCEVENT_NUM)
        return -1;
    if (iCount <= 0 || iCount > iMax)
        return -1;
    
    srand( (unsigned)time(NULL) );
    int ivRandArray[MAX_HERO_NPCEVENT_NUM]={0};
    for (int i =0; i < iMax;i++)
    {
        ivRandArray[i] = i+1;
    }
    
    for (int i =0,iSize = iMax; i < iCount;++i&&--iSize)
    {
        int iSubscript = RunDice((const int*)pRateField,iSize);        
        //swap subscript one with the last one
        int temp = ivRandArray[iSubscript];
        ivRandArray[iSubscript] = ivRandArray[iSize-1];
        ivOutput[i] = temp;
        pRateField[iSubscript] = pRateField[iSize-1];
    }
    return 0;
}

int Utils::GetRandomNum(int iMax)
{
	if(iMax<=0)
		return 0;
	return (rand()%iMax);
}
int Utils::GetRandomNum(int iMin,int iMax)
{
	if(iMax<=iMin)
		return iMin;
	return iMin+(rand()%(iMax-iMin+1));
}


int Utils::RunDice(const int *pRateField, int iFieldNum)
{
    if(iFieldNum<=0)
        return -1;

    int iRandom = rand();
    int iTotal = 0;
    for(int i=0; i<iFieldNum; i++) 
        iTotal += pRateField[i];

    if(iTotal<=0)
        return -1;
    iRandom = iRandom % iTotal;
    iTotal = 0;
    for(int i=0; i<iFieldNum; i++) 
    {
        iTotal += pRateField[i];
        if(iRandom < iTotal) 
            return i;
    }
    return -1;
}

int Utils::AllocByWeight(int iTotal,int* pWeight,int* pValue,int iNum)
{
    int iAlloc=0;
    int iTotalWeight=0;
    for(int i=0;i<iNum;i++)
        iTotalWeight+=pWeight[i];
    if(iTotalWeight<=0)
        iTotalWeight=1;
    for(int i=0;i<iNum;i++)
    {
        pValue[i]=iTotal*pWeight[i]/iTotalWeight;
        iAlloc+=pValue[i];
    }
    int iNeedAlloc=iTotal-iAlloc;
    if(iNeedAlloc>0)
    {
        for(int i=0;i<iNum;i++)
            pWeight[i]=(iTotal*pWeight[i]*10/iTotalWeight)%10;
        int iMax=0;
        int iIndex=0;
        while(iNeedAlloc>0)
        {
            iMax=0;
            iIndex=0;
            for(int i=0;i<iNum;i++)
            {
                if(pWeight[i]>iMax)
                {
                    iMax=pWeight[i];
                    iIndex=i;
                }
            }
            pValue[iIndex]++;
            iNeedAlloc--;
            pWeight[iIndex]=0;
        }
    }
    return 0;
}

int Utils::PickRandomVec(int *pVec,int iCount,int *pPickedVec,int iPickNum)
{
    int vecDup[20];
    if(iCount>20)
        return -1;
    memcpy(vecDup,pVec,sizeof(int)*iCount);
    int c=iCount;
    int idx=0;
    for(int i=0;i<iPickNum;i++)
    {
        if(i<iCount)
        {       
            idx=GetRandomNum(c);
            pPickedVec[i]=vecDup[idx];        
            if(idx<c-1)
                vecDup[idx]=vecDup[c-1];
            c--;    
        }       
        else    
        {       
            pPickedVec[i]=pVec[GetRandomNum(iCount)];
        }       
    }
    return 0;
}

