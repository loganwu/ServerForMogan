/*
 * =====================================================================================
 * 
 *       Filename:  HeapSort.h
 * 
 *    Description: default Max-Heap 
 * 
 *        Version:  1.0
 *        Created:  11/19/2015 04:32:41 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 * 
 * =====================================================================================
 */

#ifndef  HEAPSORT_INC
#define  HEAPSORT_INC

#endif   /* ----- #ifndef HEAPSORT_INC  ----- */

#include <iterator>
#include <iostream>

namespace Logan
{

	//common func
	template < class RandomIt,class DistanceType, class ValueType>
	inline void _push_heap(RandomIt first, DistanceType hole,ValueType toPush)
	{
		DistanceType top= (DistanceType)0;
		ValueType newEle = toPush; 
		DistanceType cur = hole; 
		DistanceType parent = (cur-1)/2;
		while(*(first+parent) < newEle  && top < cur)
		{
			*(first+cur) = *(first+parent);
			cur = parent;
			parent = (cur-1)/2;
		}
		*(first+cur) = newEle;
	
		return;
	}
	//Assume that [first,last-1) is already a heap, and new element in last-1;
	template <class RandomIt>
	inline void push_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		DistanceType hole= (DistanceType)(last - 1 - first);
		ValueType newEle = *(last - 1);	
		_push_heap(first,hole,newEle);
			
		return;
	}
	
	template < class RandomIt,class DistanceType, class ValueType>
	inline void adjust_heap(RandomIt first, DistanceType hole, DistanceType len, ValueType value)
	{
		//DistanceType top = hole;
		DistanceType rChild = 2 * hole + 2;
		std::cout<<"len "<<len<<std::endl;
		while(rChild < len)
		{
			//std::cout<<"pop heap rChild "<<rChild<<std::endl;
			if(*(first+rChild) < *(first+rChild-1))
				rChild--;
			*(first+hole)= *(first+rChild);
			hole = rChild;
			rChild = 2 * rChild + 2;
		}
		if(rChild == len)
		{
		//	std::cout<<"pop heap last rChild "<<rChild<<std::endl;
			*(first+hole)= *(first+rChild-1);
			hole = rChild - 1;	
		}
		
		//std::cout<<"pop heap hole end ";
		//std::cout<<"hole "<<hole<<"value "<<*(first+hole)<<std::endl;
		_push_heap(first,hole,value); //push value
		return;
	}

	template < class RandomIt>
	inline void pop_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		ValueType toPush = *(last -1 );
		*(last - 1) = *first; // pop the first ele to last-1
		DistanceType hole = 0;
		adjust_heap(first,hole,last-first-1,toPush);
		return;
	}

	template < class RandomIt>
	inline void make_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		if((last - first) < 2)
			return;
	#if 0 // myown way down to up 
		RandomIt curIt = first + 2;
		while(true)
		{
			if(last < curIt)
				break;
			push_heap(first,curIt++);
		}
	#endif
	//stl way up to down
		DistanceType len = last -first;
		DistanceType hole = (len - 2)/2;
		while(true)
		{
			adjust_heap(first,hole,len,*(first+hole));
			if(0 == hole) return;
			hole--;
		}

		return;
	}

	template < class RandomIt>
	inline void sort_heap(RandomIt first, RandomIt last)
	{
		while(first+1 < last)
			pop_heap(first,last--);

	}
}
