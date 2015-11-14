/*
 * =====================================================================================
 * 
 *       Filename:  FixVector.h
 * 
 *    Description: fixed array, reference /usr/include/c++/4.1.2/bits/stl_vector.h 
 * 
 *        Version:  1.0
 *        Created:  11/10/2015 10:25:39 AM CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 * 
 * =====================================================================================
 */

#ifndef  FIXVECTOR_INC
#define  FIXVECTOR_INC
#include <iostream>
#include "string.h"
template <class T,int N>
class FixVector
{
public:
	typedef T value_type;
	typedef value_type* iterator;
	typedef int size_type ;
	typedef T& reference;
public:
	FixVector():start(&data[0]),finish(&data[0]),end_of_storage(&data[N]){}
	~FixVector(){}
public:
	bool empty(){return start==finish;}
	int push_back(const T& value);
	void pop_back(){--finish;}
	int erase(iterator it);
	int erase(iterator first, iterator last);
	void clear();
	reference operator[](size_type  n){return *(begin()+n);}
	reference front(){return *start;}
	reference back(){return  *finish;}
	size_type capacity(){return size_type(end_of_storage - start);}
	size_type size(){return size_type(finish - start);}
	iterator begin(){return start;}
	iterator end(){return finish;}

private:
	iterator start;
	iterator finish;
	iterator end_of_storage;
	T data[N];
};

template <class T,int N>
int FixVector<T,N>::push_back(const T& value)
{
	if(finish == end_of_storage)	
	{
		std::cout<<"out of storage"<<std::endl;
		return -1;
	}
	*finish = value;
	finish++;
	return 0;
}

template <class T,int N>
int FixVector<T,N>::erase(iterator it)
{
	if(it<start || it >= finish)
	{
		std::cout<<"invalid iterator"<<std::endl;
		return -1;
	}
	memmove(it,it+1,sizeof(T)*(finish-it-1));	
	finish--;
	return 0;
}

template <class T,int N>
int FixVector<T,N>::erase(iterator first,iterator last)
{
	if(first > last || first <start || last>= finish)
	{
		std::cout<<"invalid iterator range"<<std::endl;
		return -1;
	}
	memmove(first,last+1,sizeof(T)*(finish-last-1));	
	finish = finish -(last-first+1);
	return 0;
}

#endif   /* ----- #ifndef FIXVECTOR_INC  ----- */

