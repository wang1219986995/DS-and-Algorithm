//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_CONSTRUCT_H
#define MYSTL_STL_CONSTRUCT_H

#include <new.h>

namespace MySTL{


template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new ((void*)p) T1(value);
}























}



#endif //MYSTL_STL_CONSTRUCT_H
