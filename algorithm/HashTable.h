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
#include <iostream>

namespace Logan
{
	enum { _S_num_primes = 28 };
	enum{DISABLE = -1};
	static const size_t stl_prime_list[_S_num_primes] =
	{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};

	size_t next_prime_size(size_t n)
	{
		const size_t* first = stl_prime_list;
		const size_t* last = stl_prime_list + (int)_S_num_primes;
		const size_t* pos = std::lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}
	typedef size_t linktype;

	template <class T, class K >
	struct HashNode
	{
		T value;
		K key;
		linktype  next;
	};

	template < class T, class K,int N>
	class HashTable;

	template <class T,class K,int N>
	struct HashIterator: public  std::iterator< std::forward_iterator_tag, T >
	{
		typedef HashIterator<T,K,N> self;
		typedef HashTable<T,K,N> hashtable;
		typedef HashNode<T,K> hashnode;

		HashIterator() : pNode(NULL), ht(NULL){}
		HashIterator(hashnode* p, hashtable* hash) : pNode(p),ht(hash){}
		~HashIterator()  {}

		T& operator*()
		{
			return pNode->value;
		}

		T* operator->()
		{
			return &(operator*());
		}

		self&  operator++()
		{
			hashnode* old = pNode;
			if(old->next == DISABLE) //next node null
			{
				linktype num = ht->bucket_num(old->key);
				linktype cur = ht->pBuckets[num];
				for(; cur == DISABLE && num < ht->bucket_size();num++)
					cur = ht->pBuckets[num];
				if(DISABLE == cur)
					pNode = NULL;
				else
					pNode = &(ht->vData[cur]);
				
			}
			else
			{
				pNode = &(ht->vData[pNode->next]);
			}
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}

		hashnode* pNode;
		hashtable* ht;
	};
	
	// data type key type data size
	template < class T, class K,int N>
	class HashTable
	{
	public:
		enum{HASH_EXIST= -2, HASH_FULL = -1,HASH_SUCC};
		typedef HashIterator<T,K,N> iterator;
		HashTable(){init();}	
		~HashTable(){if(NULL == pBuckets) delete[] pBuckets;}	
		
		linktype bucket_num(K key){return (linktype)(key%N);};
		size_t bucket_size(){return next_prime_size(N);};
		size_t count(){return eleCount;};
		std::pair< iterator,int> insert(const K& k, const T& t);
		int erase(const K& k);
		T* get(const K& k);
		//void clear();

	private:
		
		void init()
		{
			memset(this,0,sizeof(*this));
			pBuckets = new linktype[next_prime_size(N)];
			for(int i =0;i < N;i++)
				vData[i].next = i+1;
				vData[N-1].next = DISABLE;
			
			for(int i =0;i < bucket_size();i++)
				pBuckets[i] = DISABLE;

		}
	public:
		linktype* pBuckets;	
		linktype free;
		HashNode<T,K> vData[N];
	private:
		size_t eleCount;

		
	};// HashTable class

	template<class T, class K, int N>
	std::pair< typename HashTable<T,K,N>::iterator,int> HashTable<T,K,N>::insert(const K& k, const T& t)
	{
		iterator it;
		if(DISABLE == free || eleCount > N)
		{
			std::cout<<"ele full "<<k<<std::endl;
			return std::make_pair(it,-1);
		}

		linktype num = bucket_num(k);
		linktype cur = pBuckets[num];
		while( DISABLE != cur)	
		{
			if(vData[cur].key == k)
			{
				std::cout<<"replicated key "<<k<<std::endl;
				return std::make_pair(it,-2);
			}
			cur = vData[cur].next;
		}

		vData[free].value = t;
		vData[free].key= k;
		linktype freeNext = vData[free].next;
		vData[free].next = pBuckets[num];
		pBuckets[num] = free;	
		

		//update free 
		eleCount++;
		free = freeNext;
		iterator it1( &(vData[free]),this);

		return std::make_pair(it1,0);
	}

	template<class T, class K, int N>
	int HashTable<T,K,N>::erase(const K& k )
	{
		linktype num = bucket_num(k);
		linktype cur = pBuckets[num];
		linktype prev = cur; 
		while( DISABLE != cur)	
		{
			if(vData[cur].key == k)
			{
				std::cout<<"erase key "<<k<<std::endl;
				pBuckets[num] = vData[cur].next;
				vData[cur].next = free;
				free = cur;
				vData[prev].next = vData[cur].next;
				
				eleCount--;

				return 0;
			}
			prev = cur;
			cur = vData[cur].next;
		}

		return -1;
	}

	template<class T, class K, int N>
	T* HashTable<T,K,N>::get(const K& k )
	{
		linktype num = bucket_num(k);
		linktype cur = pBuckets[num];
		while( DISABLE != cur)	
		{
			if(vData[cur].key == k)
			{
				std::cout<<"get key "<<k<<std::endl;
				return &(vData[cur].value);
			}
			cur = vData[cur].next;
		}

		return NULL;
	}
}

#endif   /* ----- #ifndef HASHTABLE_INC  ----- */

