//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_CONSTRUCT_H
#define MYSTL_STL_CONSTRUCT_H

#include <new.h>
#include "type_traits.h"
#include "alloc.h"
#include "stl_iterator.h"
namespace MySTL{

// Construct
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new ((void*)p) T1(value);
}

template <class T1>
inline void _Construct(T1* p)
{
    new ((void*) p) T1();
}

template <class T1, class T2>
inline void construct(T1* p, const T2& value)
{
    _Construct(p, value);
}

template <class Tp>
inline void construct(Tp* p)
{
    _Construct(p);
}


// Destroy
template <class Tp>
inline void _Destroy(Tp* pointer)
{
    pointer->~Tp();
}

template <class ForwardIterator>
void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for(; first != last; ++first)
        destroy(&*first);
}

template <class ForwardIterator>
void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

template <class ForwardIterator, class Tp>
inline void __destroy(ForwardIterator first, ForwardIterator last, Tp*)
{
    typedef typename type_traits<Tp>::has_trivial_destructor Trivial_destructor;
    __destroy_aux(first, last, Trivial_destructor());
}

template <class ForwardIterator>
inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, VALUE_TYPE(first));
}

//TODO: 这里的Destroy函数什么都没做，那收回内存的步骤是什么时候进行的呢？
// 先destroy再回收内存么？
inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}
inline void _Destroy(wchar_t*, wchar_t*) {}

template <class Tp>
inline void destroy(Tp* pointer)
{
    _Destroy(pointer);
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    _Destroy(first, last);
}



}



#endif //MYSTL_STL_CONSTRUCT_H
