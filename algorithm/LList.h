/*
 * =====================================================================================
 * 
 *       Filename:  LList.h
 * 
 *    Description: rev1.0 must include iterator ~
 * 
 *        Version:  1.0
 *        Created:  11/11/2015 04:17:20 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 * 
 * =====================================================================================
 */

#ifndef  LLIST_INC
#define  LLIST_INC

#include <iterator>

template <class T>
struct _list_node
{
	typedef _list_node<T>* pointer;
	pointer next;
	pointer prev;
	T data;
};

template<class T>
class LList
{
public:
	typedef T value_type;
	typedef int size_type;
	typedef T& reference;
	typedef _list_node<T> list_node;
	typedef list_node* link_type;
public:
	LList():pMB(0){}
	~LList(){}

	class iterator:public std::iterator<std::bidirectional_iterator_tag,T>
	{
	public:
		iterator():node(0){}
		iterator(link_type pMb):node(pMb){}
	public:
		T& operator*()
		{
			return node->data;
		}
		T* operator->()
		{
			return &(node->data);
		}
		
		iterator& operator++()
		{
			node = (link_type)(node->next);
			return *this;
		}
		iterator operator++(int)
		{
			iterator temp = *this;
			++*this;
			return temp;
		}
		iterator& operator--()
		{
			node = (link_type)(node->prev);
			return *this;
		}
		iterator operator--(int)
		{
			iterator temp = *this;
			--*this;
			return temp;
		}
		iterator begin() const
		{
			return iterator(pMB->next);  
		}
		iterator end() const
		{
			return iterator(pMB);  
		}

		bool operator==(iterator& it)
		{
			return (node == it.node);
		}
		bool operator!=(iterator& it)
		{
			return (node != it.node);
		}
	private:
		link_type node;
	
	}; // class iterator define
public:

		iterator begin() const
		{
			return iterator(pMB->next);  
		}
		iterator end() const
		{
			return iterator(pMB);  
		}



private:
	link_type pMB;	
};
#endif   /* ----- #ifndef LLIST_INC  ----- */

