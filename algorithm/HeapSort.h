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

	//Assume that [first,last) is already a heap, and new element in last;
	template <class RandomIt>
	inline void push_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
		DistanceType top= (DistanceType)0;
		ValueType newEle = *(last - 1);	
		DistanceType cur = last - 1 - first;
		DistanceType parent = (cur-1)/2;
		while(*(first+parent) < *(first+cur) && parent < top && cur != 0)
		{
			*(first+parent) = *(first+cur);
			cur = parent;
			parent = (cur-1)/2;
		}
		*(first+cur) = newEle;
	
		return;
	}
	
	template < class RandomIt>
	inline void pop_heap(RandomIt first, RandomIt last)
	{
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
