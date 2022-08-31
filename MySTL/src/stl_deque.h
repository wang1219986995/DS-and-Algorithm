//
// Created by GW00243464 on 2022/8/24.
//

#ifndef MYSTL_STL_DEQUE_H
#define MYSTL_STL_DEQUE_H

#include <stdlib.h>
#include "stl_alloc.h"
namespace MySTL
{

inline size_t deque_buf_size(size_t size)
{ return size < 512 ? size_t(512 / size) : size_t(1); }


template <class Tp, class Alloc, bool is_static>
class Deque_alloc_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;

    allocator_type get_allocator() const { }


protected:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type Map_allocator_type;
    allocator_type M_node_allocator;
    allocator_type M_map_allocator;
    Tp** M_map;
    size_t M_map_size;
};
































}























#endif //MYSTL_STL_DEQUE_H
