/*
 * =====================================================================================
 *
 *       Filename:  heapExp.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2015 08:06:24 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 *
 * =====================================================================================
 */

#include "HeapSort.h"
#include "FixVector.h"
#include <iostream>
//#include <algorithm>
using namespace std;
int main()
{
		
	FixVector<int,50> vVector;
	vVector.push_back(1);
	vVector.push_back(0);
	vVector.push_back(2);
	Logan::push_heap(vVector.begin(),vVector.end());
	return 0;
}
