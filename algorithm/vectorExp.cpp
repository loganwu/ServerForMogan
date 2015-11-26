/*
 * =====================================================================================
 *
 *       Filename:  vectorExp.cpp
 *
 *    Description: Vector example 
 *
 *        Version:  1.0
 *        Created:  11/11/2015 01:00:43 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 *
 * =====================================================================================
 */

#include "FixVector.h"
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{

	FixVector<int,50>::iterator iter;
	FixVector<int,50> vVector;
	vVector.push_back(1);
	vVector.push_back(0);
	vVector.push_back(2);
	cout<<"capacity "<<vVector.capacity()<<endl;
	cout<<"size "<<vVector.size()<<endl;
	cout<<"back value "<<vVector.back()<<endl;
	for(iter=vVector.begin();iter != vVector.end();iter++)
	{
		cout<<*iter<<endl;
	}
	vVector.pop_back();
	vVector.push_back(3);
//	vVector.push_back(33);
//	vVector.push_back(34);
//	vVector.push_back(14);
	cout<<"capacity "<<vVector.capacity()<<endl;
	cout<<"size "<<vVector.size()<<endl;
	for(iter=vVector.begin();iter != vVector.end();iter++)
	{
		cout<<*iter<<endl;
	}
	//iter = find(vVector.begin(),vVector.end(),2);
	vVector.sort();
	cout<<"sort "<<endl;
	for(iter=vVector.begin();iter != vVector.end();iter++)
	{
		cout<<*iter<<endl;
	}
	iter = vVector.find(4);
	if(iter!=vVector.end())
		cout<<"find "<<*iter<<endl;
	return 0;
}
