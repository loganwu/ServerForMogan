/*
 * =====================================================================================
 * 
 *       Filename:  HeapSort.h
 * 
 *    Description:  
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

	template <class RandomIt>
	inline void push_heap(RandomIt first, RandomIt last)
	{
		typedef typename std::iterator_traits<RandomIt>::value_type ValueType;
		typedef typename std::iterator_traits<RandomIt>::difference_type DistanceType;
	
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
		return;
	}
}
