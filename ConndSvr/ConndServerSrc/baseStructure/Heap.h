// =====================================================================================
// 
//       Filename:  hsort.h
// 
//    Description:  fast sort base on heap
// 
//        Version:  1.0
//        Created:  08/13/2007 10:08:35 AM CST
//       Revision:  none
//       Compiler:  g++
//         Editor:  vim (ts=4,sw=4,et)
//         Author:  Jaco Wu (), jacowu@tencent
//        Company:  Tencent
// 
//        History:
//               1. Date:
//                  Author:
//                  Modification:
// =====================================================================================

#ifndef  HSORT_INC
#define  HSORT_INC

template <int v>
struct Int2Type
{
    enum {value=v};
};

template<class T>
struct _pod_traits
{
    typedef Int2Type<0> _is_assignable;
};


template <class T>
inline void _copy_impl(T* to,T* from,Int2Type<0>)
{
    memcpy(to,from,sizeof(T));
}

template<class T>
inline void _copy_impl(T* to,T* from,Int2Type<1>)
{
    *to=*from;
}

template <class T>
inline void _fast_copy(T* to,T* from)
{
    typename _pod_traits<T>::_is_assignable is_assignable;
    _copy_impl(to,from,is_assignable);
}

template <class T>
inline void _fast_switch(T* p,T* q)
{
    typename _pod_traits<T>::_is_assignable is_assignable;
    T s;
    _copy_impl(&s,p,is_assignable);
    _copy_impl(p,q,is_assignable);
    _copy_impl(q,&s,is_assignable);
}

template <class T>
void heap_adjust(T* list,int s,int m)
{
    T t;
    _fast_copy(&t,&(list[s-1]));
    for(int j=2*s;j<=m;j*=2)
    {
        if(j<m && list[j-1] > list[j+1-1])
            j++;
        if(!(t > list[j-1]))
            break;
        _fast_switch(&(list[s-1]),&(list[j-1]));
        s=j;
    }
    _fast_copy(&(list[s-1]),&t);
}

template <class T>
void heap_sort(T * list, int length)
{
    for(int i=length/2;i>0;i--)
        heap_adjust(list,i,length);

    for(int j=length;j>1;j--)
    {
        _fast_switch(&(list[0]),&(list[j-1]));
        heap_adjust(list,1,j-1);
    }
}


#endif   /* ----- #ifndef HSORT_INC  ----- */
/* vi:set ts=4: */ 
