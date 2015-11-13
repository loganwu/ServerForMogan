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
#include <iostream>

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
	struct iterator:public std::iterator<std::bidirectional_iterator_tag,T>
	{
		iterator():node(0){}
		iterator(link_type pMb):node(pMb){}

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

		bool operator==(iterator& it) const
		{
			return (node == it.node);
		}
		bool operator!=(iterator& it) const
		{
			return (node != it.node);
		}

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
		
	LList()
	{
		pMB = new list_node;
		pMB->next = pMB;
		pMB->prev = pMB;
	}
	~LList(){}

	iterator insert(iterator position, const T& x)
	{
		link_type temp_node = create_node(x);
		temp_node->next = position.node;
		temp_node->prev = position.node->prev;
		(position.node->prev)->next = temp_node;
		position.node->prev= temp_node;
		return iterator(temp_node);
	}


private:

	link_type create_node(const T& data)
	{
		link_type p = new list_node;
		p->data = data;
	//	construct(&p->data,data);
		return p;
	}

private:
	link_type pMB;	
};
#endif   /* ----- #ifndef LLIST_INC  ----- */

