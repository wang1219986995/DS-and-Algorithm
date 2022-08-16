//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_UNINITIALIZED_H
#define MYSTL_STL_UNINITIALIZED_H

#include <string.h>
#include "type_traits.h"
#include "stl_construct.h"
#include "stl_pair.h"
//TODO: 这里用了一些标准库的东西
#include <algorithm>

namespace MySTL{




template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy_aux(InputIter first, InputIter last,
                                            ForwardIter result, __true_type)
{
    return std::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy_aux(InputIter first, InputIter last,
                                            ForwardIter result, __false_type)
{
    ForwardIter cur = result;
    try{
        for(; first != last; ++last)
            _Construct(&*cur, *first);
        return cur;
    }
    catch(...){
        _Destroy(result, cur);
        throw;
    }
}

template <class InputIter, class ForwardIter, class Tp>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last,
                                        ForwardIter result, Tp*)
{
    typedef typename type_traits<Tp>::is_POD_type Is_POD;
    return __uninitialized_copy_aux(first, last, result, Is_POD());
}

template <class InputIter, class ForwareIter>
inline ForwareIter uninitialized_copy(InputIter first, InputIter last,
                                      ForwareIter result)
{
    return __uninitialized_copy(first, last, result, VALUE_TYPE(result));
}

inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}



// uninitialized_copy_n
template <class InputIter, class Size, class ForwardIter>
pair<InputIter, ForwardIter>
__uninitialized_copy_n(InputIter first, Size count, ForwardIter result, input_iterator_tag)
{
    ForwardIter cur = result;
    try {
        for(; count > 0; --count)
            _Construct(&*cur, *first);
    }
    catch (...)
    {
        _Destroy(result, cur);
        throw;
    }
}

template <class RandomAccessIter, class Size, class ForwardIter>
pair<RandomAccessIter, ForwardIter>
__uninitialized_copy_n(RandomAccessIter first, Size count, ForwardIter result, random_access_iterator_tag)
{
    RandomAccessIter last = first + count;
    return pair<RandomAccessIter, ForwardIter>
            (last,uninitialized_copy(first, last, result));
}

template <class InputIter, class Size, class ForwardIter>
inline pair<InputIter, ForwardIter>
        __uninitialized_copy_n(InputIter first, Size count, ForwardIter result)
{
    return __uninitialized_copy_n(first, count, result, ITERATOR_CATEGORY(first));
}

template <class InputIter, class Size, class ForwaredIter>
inline pair<InputIter, ForwaredIter>
        uninitialized_copy_n(InputIter first, Size count, ForwaredIter result)
{
    return __uninitialized_copy_n(first, count, result);
}



// uninitialized_fill
template <class ForwardIter, class Tp>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const Tp& x, __true_type)
{
    for(; first != last; ++first)
        *first = x;
}

template <class ForwardIter, class Tp>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const Tp& x, __false_type)
{
    ForwardIter cur = first;
    try {
        for(;cur != last; ++cur)
            _Construct(&*cur, x);
    }
    catch (...)
    {
        _Destroy(first, cur);
        throw ;
    }
}

template <class ForwardIter, class Tp>
inline void __uninitialized_fill(ForwardIter first, ForwardIter last, const Tp& x)
{
    typedef typename type_traits<Tp>::is_POD_type Is_POD;
    __uninitialized_fill_aux(first, last, x, Is_POD());
}

template <class ForwardIter, class Tp>
inline void uninitialized_fill(ForwardIter first, ForwardIter last, const Tp& x)
{
    __uninitialized_fill(first, last, x);
}


// uninitialized_fill_n
template <class ForwardIter, class Size, class Tp>
inline ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const Tp& x, __true_type)
{
    for(; n > 0; --n, ++first)
        *first = x;
    return first;
}

template <class ForwardIter, class Size, class Tp>
inline ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const Tp& x, __false_type)
{
    ForwardIter cur = first;
    try {
        for(; n > 0; --n, ++first)
            _Construct(&*cur, x);
        return cur;
    }
    catch (...)
    {
        _Destroy(first, cur);
        throw ;
    }
}

template <class ForwardIter, class Size, class Tp>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const Tp& x)
{
    typedef typename type_traits<Tp>::is_POD_type Is_POD;
    return __uninitialized_fill_aux(first, n, x, Is_POD());
}

template <class ForwardIter, class Size, class Tp>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const Tp& x)
{
    return __uninitialized_fill(first, n, x);
}


}




#endif //MYSTL_STL_UNINITIALIZED_H
