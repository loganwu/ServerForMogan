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
	LList<int>::iterator it2 = llist.end();
	it2 = LList<int>::iterator(it);
#if 1
	it  = llist.insert(it,1);
	it  = llist.insert(it,2);
	it  = llist.insert(it,3);
	it = llist.begin();
	cout<<"--------insert-------------"<<endl;
	for(;it != llist.end();it++)
		cout<<*it<<endl;

	llist.clear();

	llist.push_back(11);
	llist.push_back(12);
	llist.push_back(13);
	llist.erase(llist.begin());
	cout<<"--------push_back-------------"<<endl;
	for(it=llist.begin();it != llist.end();it++)
		cout<<*it<<endl;
	llist.clear();
	cout<<"--------size-------------"<<llist.size()<<endl;
	llist.push_back(14);
	llist.push_back(15);
	llist.push_back(16);
	for(it=llist.begin();it != llist.end();it++)
		cout<<*it<<endl;
	cout<<"--------transfer-------------"<<llist.size()<<endl;
	LList<int>::iterator first= ++llist.begin();
	LList<int>::iterator last= llist.end();
	llist.transfer(++llist.begin(),++first,last);
	for(it=llist.begin();it != llist.end();it++)
		cout<<*it<<endl;
	cout<<"--------reverse-------------"<<llist.size()<<endl;
	llist.reverse();
	for(it=llist.begin();it != llist.end();it++)
		cout<<*it<<endl;
#endif		
	return 0;
}
