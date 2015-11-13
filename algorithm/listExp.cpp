/*
 * =====================================================================================
 *
 *       Filename:  listExp.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2015 04:27:09 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 *
 * =====================================================================================
 */

#include "LList.h"
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{

	LList<int> llist;
	LList<int>::iterator it = llist.begin();
	it  = llist.insert(it,1);
	cout<<*it<<endl;
	cout<<"----------------"<<endl;
	it  = llist.insert(it,2);
	cout<<*it<<endl;
	cout<<"----------------"<<endl;
	it  = llist.insert(it,3);
	cout<<*it<<endl;
	cout<<"----------------"<<endl;
	it = llist.begin();
	cout<<"begin"<<*it<<endl;
	cout<<"----------------"<<endl;
	it = llist.end();
	cout<<"end"<<*it<<endl;
	for(; it != llist.end();it++);
	{
		cout<<*it<<endl;
	}
	return 0;
}
