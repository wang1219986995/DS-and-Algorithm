//

#ifndef MYSTL_STL_CONSTRUCT_H
#define MYSTL_STL_CONSTRUCT_H

#include <new.h>
#include "type_traits.h"
#include "stl_alloc.h"
#include "stl_iterator.h"

namespace MySTL{
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


template <class Tp>
inline void __Destroy(Tp* pointer)
{
    pointer->~Tp();
}

template <class Tp>
inline void destroy(Tp* pointer)
{
    __Destroy(pointer);
}

template <class ForwardIterator>
void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for(; first != last; ++first)
        MySTL::destroy(&*first);
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
inline void __Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, VALUE_TYPE(first));
}

inline void __Destroy(char*, char*) {}
inline void __Destroy(int*, int*) {}
inline void __Destroy(long*, long*) {}
inline void __Destroy(float*, float*) {}
inline void __Destroy(double*, double*) {}
inline void __Destroy(wchar_t*, wchar_t*) {}

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



template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __Destroy(first, last);
}



}



#endif //MYSTL_STL_CONSTRUCT_H
