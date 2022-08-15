//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_UNINITIALIZED_H
#define MYSTL_STL_UNINITIALIZED_H

#include "type_traits.h"
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
    return __
}





















}




#endif //MYSTL_STL_UNINITIALIZED_H
