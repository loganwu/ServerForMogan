/*
 * =====================================================================================
 * 
 *       Filename:  HashTable.h
 * 
 *    Description: fixSize hashtable with fixed buckets reference GNU C++ /usr/include/c++/4.1.2/ext/hashtable.h 
 * 
 *        Version:  1.0
 *        Created:  11/25/2015 05:33:51 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 * 
 * =====================================================================================
 */

#ifndef  HASHTABLE_INC
#define  HASHTABLE_INC
#include <utility>
#include <iterator>

namespace Logan
{
	template <class T, class K >
	struct HashNode
	{
		T value;
		K key;
		HashNode<T>* next;
	};

	template <class T,K>
	struct HashIterator: public  std::iterator< std::forward_iterator_tag, T >
	{
		typedef HashIterator<T,K> self;
		typedef HashNode<T,K>* linktype;

		HashIterator() : pNode(NULL){}
		HashIterator(linktype p) : pNode(p){}
		~HashIterator() : {}

		reference operator*()
		{
			return pNode->value;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		self&  operator++()
		{
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}

		linktype  pNode;
	};
	
	// data type key type data size
	template < class T, class K,int N>
	class HashTable
	{
	public:
		typedef HashIterator<T,K> iterator;
		typedef size_t nodeIndex;
		HashTable(){}	
		~HashTable(){}	
		
		pair< iterator,int> insert(const K& k, const T& t);
		int erase(const K& k);
	private:
		
		enum { _S_num_primes = 28 };
		static const size_t __stl_prime_list[_S_num_primes] =
		{
			53ul,         97ul,         193ul,       389ul,       769ul,
			1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
			49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
			1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
			50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		size_t next_prime_size(size_t n);
		{
			const size_t* __first = __stl_prime_list;
			const size_t* __last = __stl_prime_list + (int)_S_num_primes;
			const size_t* pos = std::lower_bound(__first, __last, __n);
			return pos == __last ? *(__last - 1) : *pos;
		}
	private:
		const bucket_size = next_prime_size(N); 
		nodeIndex buckets[bucket_size];

		
	};// HashTable class

}

#endif   /* ----- #ifndef HASHTABLE_INC  ----- */

