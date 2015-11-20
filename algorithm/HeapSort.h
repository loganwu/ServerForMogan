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

namespace Logan
{

	//Assume that [first,last-1) is already a heap, and new element in last-1;
	template <class RandomIt>
	inline void push_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		DistanceType top= (DistanceType)0;
		ValueType newEle = *(last - 1);	
		DistanceType cur = last - 1 - first;
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
	
	template < class RandomIt,class DistanceType, class ValueType>
	inline void adjust_heap(RandomIt first, DistanceType hole, DistanceType len, ValueType value)
	{
		DistanceType top = hole;
		DistanceType rChild = 2 * hole + 2;
		while(rChild < len)
		{
			if(*(first+rChild) < *(first+rChild-1))
				rChild--;
			*(first+hole)= *(first+rChild);
			hole = rChild;
			rChild = 2 * rChild + 2;
		}
		if(rChild == len)
		{
			*(first+hole)= *(first+rChild);
			hole = rChild - 1;	
		}
		push_heap(first,first+hole); //push value
		return;
	}

	template < class RandomIt>
	inline void pop_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		ValueType toPush = *(last -1 );
		*(last - 1) = *first; // pop the first ele to last-1
		//DistanceType hole = 0;
		adjust_heap(first,0,last-first-1,toPush);
		return;
	}

	template < class RandomIt>
	inline void make_heap(RandomIt first, RandomIt last)
	{
		if((last - first) < 2)
			return;
		RandomIt curIt = first + 2;
		while(true)
		{
			if(last < curIt)
				break;
			push_heap(first,curIt++);
		}
		return;
	}
}
