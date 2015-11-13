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

template <class T>
struct list_iterator:public std::iterator<std::bidirectional_iterator_tag,T>
{

	typedef list_iterator<T> self;
	typedef _list_node<T>* link_type;
	list_iterator():node(0){}
	explicit list_iterator(link_type pLink):node(pLink){}
	list_iterator(const self& x):node(x.node){}

	T& operator*()
	{
		return node->data;
	}
	T* operator->()
	{
		return &(node->data);
	}
		
	self& operator++()
	{
		node = (link_type)(node->next);
		return *this;
	}
	self operator++(int)
	{
		self temp = *this;
		++*this;
		return temp;
	}
	self& operator--()
	{
		node = (link_type)(node->prev);
		return *this;
	}
	self operator--(int)
	{
		self temp = *this;
		--*this;
		return temp;
	}

	bool operator==(const self& it) const
	{
		return (node == it.node);
	}
	bool operator!=(const self& it) const
	{
		return (node != it.node);
	}

	link_type node;
	
}; // class list_iterator define

template<class T>
class LList
{
public:
	typedef T value_type;
	typedef int size_type;
	typedef T& reference;
	typedef _list_node<T> list_node;
	typedef _list_node<T>* link_type;
	typedef list_iterator<T> iterator;

public:

	iterator begin() const
	{
		return iterator(pMB->next);  
	}
	iterator end() const
	{
		return iterator(pMB);  
	}
		
	void init()
	{

		pMB->next = pMB;
		pMB->prev = pMB;
	}
	LList()
	{
		pMB = new list_node;
		init();
	}
	~LList()
	{
		if(pMB != 0)
			delete pMB;
	}

	iterator insert(iterator position, const T& x)
	{
		link_type temp_node = create_node(x);
		temp_node->next = position.node;
		temp_node->prev = position.node->prev;
		(position.node->prev)->next = temp_node;
		position.node->prev= temp_node;
		return iterator(temp_node);
	}
	
	void push_back(const T& x)
	{
		insert(end(),x);
		return;
	}
	void clear();

	iterator erase(iterator pos);
	iterator transfer(iterator pos,iterator first,iterator last);

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

template<class T>
void  LList<T>::clear()
{
	link_type pos = pMB->next;//begin
	while(pos != end().node) //end == pMB
	{
		link_type preNode = (link_type)(pos->prev);
		link_type nextNode = (link_type)(pos->next);

		preNode->next = nextNode;
		nextNode->prev = preNode;
		delete pos;
		pos = nextNode;
	}
	init();
	return;
}

template<class T>
list_iterator<T> LList<T>::erase(iterator pos)
{
	if(pos == end())
		return end();
	link_type preNode = (link_type)(pos.node->prev);
	link_type nextNode = (link_type)(pos.node->next);

	preNode->next = nextNode;
	nextNode->prev = preNode;
	delete pos.node;

	return iterator(nextNode);
}

template <class T>
list_iterator<T>  LList<T>::transfer(iterator pos,iterator first,iterator last)
{
	return iterator(0);
}

#endif   /* ----- #ifndef LLIST_INC  ----- */

