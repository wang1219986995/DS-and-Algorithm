//
// Created by GW00243464 on 2022/8/31.
//

#ifndef MYSTL_STL_ALGOBASE_H
#define MYSTL_STL_ALGOBASE_H
#include "stl_iterator_base.h"
namespace MySTL
{


template  <class ForwardIter1, class ForwardIter2, class Tp>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, Tp*)
{
    Tp tmp = *a;
    *a = *b;
    *b = tmp;
}


template <class ForwaredIter1, class ForwaredIter2>
inline void iter_swap(ForwaredIter1 a, ForwaredIter2 b)
{
    __iter_swap(a, b, VALUE_TYPE(a));
}

template <class Tp>
inline void swap(Tp& a, Tp& b)
{
    Tp tmp = a;
    a = b;
    b = tmp;
}


// min and max



















}

























#endif //MYSTL_STL_ALGOBASE_H
