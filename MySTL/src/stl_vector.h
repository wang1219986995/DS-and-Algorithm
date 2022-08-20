//
// Created by GW00243464 on 2022/8/19.
//

#ifndef MYSTL_STL_VECTOR_H
#define MYSTL_STL_VECTOR_H

#include "stl_alloc.h"

namespace MySTL{


template <class Tp, class Allocator, bool IsStatic>
class Vector_alloc_base{
public:
    typedef typename Alloc_traits<Tp, Allocator>::allocator_type allocator_type;
    allocator_type get_allocator() const {return M_data_allocator;}

protected:
    allocator_type M_data_allocator;
    Tp* M_start;
    Tp* M_finish;
    Tp* M_end_of_storage;

    Tp* M_allocate(size_t n)
    {  return M_data_allocator.allocate(n); }

    void M_deallocate(Tp* p, size_t n)
    {  if(p)  M_data_allocator.deallocate(p, n);  }
};

























}











#endif //MYSTL_STL_VECTOR_H
