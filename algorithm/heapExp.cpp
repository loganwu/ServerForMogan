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
	vVector.push_back(5);
	vVector.push_back(3);
	Logan::make_heap(vVector.begin(),vVector.end());
	for(FixVector<int,50>::iterator it = vVector.begin(); it != vVector.end();it++)
		cout<<*it<<endl;
	vVector.push_back(9);
	cout<<"===================push heap 9======"<<endl;
	Logan::push_heap(vVector.begin(),vVector.end());
	for(FixVector<int,50>::iterator it = vVector.begin(); it != vVector.end();it++)
		cout<<*it<<endl;
	cout<<"===================pop heap======"<<endl;
	Logan::pop_heap(vVector.begin(),vVector.end());
	cout<<*vVector.back()<<endl;
	return 0;
}
