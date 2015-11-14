#ifndef  UTILS_INC
#define  UTILS_INC

#include <time.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <iconv.h>
#include <stdarg.h>
using namespace std;

#define MAX_HERO_NPCEVENT_NUM 500

class Utils
{
public:

    static int PickRandomVec(int *pVec,int iCount,int *pPickedVec,int iPickNum);
	static int GetRandomNum(int iMax);
	static int GetRandomNum(int iMin,int iMax);
	static int RunDice(const int *pRateField, int iFieldNum);
    static int AllocByWeight(int iTotal,int* pWeight,int* pValue,int iNum);
    static void GetRandomListsWithFixedSum(int *ivOutput, int iCount, int iSum, int iMin, int iMax);
    static int GetRandomListsNoDuplication(unsigned char *ivOutput, int iCount,int iMax, int *pRateField);
};

#endif   /* ----- #ifndef UTILS_INC  ----- */

