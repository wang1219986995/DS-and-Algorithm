//
// Created by GW00243464 on 2022/8/24.
//

#ifndef MYSTL_STL_DEQUE_H
#define MYSTL_STL_DEQUE_H

#include <stdlib.h>
#include "stl_algo.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
namespace MySTL
{

inline size_t deque_buf_size(size_t size)
{ return size < 512 ? size_t(512 / size) : size_t(1); }

template <class Tp, class Ref, class Ptr>
struct Deque_iterator
{
    typedef Deque_iterator<Tp, Tp&, Tp*>                iterator;
    typedef Deque_iterator<Tp, const Tp&, const Tp*>    const_iterator;
    static size_t S_buffer_size() { return deque_buf_size(sizeof(Tp)); }

    typedef random_access_iterator_tag  iterator_category;
    typedef Tp                          value_type;
    typedef Ptr                         Pointer;
    typedef Ref                         reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef Tp**                        Map_pointer;
    typedef Deque_iterator              Self;



    Tp* M_cur;
    Tp* M_first;
    Tp* M_last;
    Map_pointer M_node;


    void M_set_node(Map_pointer new_node)
    {
        M_node = new_node;
        M_first = *new_node;
        M_last = M_first + difference_type(S_buffer_size());

    }

    difference_type operator-(const Self& x) const
    {  return difference_type(S_buffer_size()) * (M_node - x.M_node - 1) + (M_cur - M_first) + (x.M_last - x.M_cur);  }

    Self& operator++()
    {
        ++M_cur;
        if(M_cur == M_last)
        {
            M_set_node(M_node + 1);
            M_cur = M_first;
        }
        return *this;
    }

    Self& operator++(int)
    {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    Self& operator--()
    {
        if(M_cur == M_first)
        {
            M_set_node(M_node - 1);
            M_cur = M_last;
        }
        --M_cur;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp = *this;
        --*this;
        return tmp;
    }

    Self& operator+=(difference_type n)
    {
        difference_type offset = n + (M_cur - M_first);
        if(offset >= 0 && offset < difference_type(S_buffer_size()))
            M_cur += n;
        else
        {
            difference_type node_offset = offset > 0 ?
                    offset / difference_type(S_buffer_size()) :
                    -difference_type((-offset - 1) / S_buffer_size()) - 1;
            M_set_node(M_node + node_offset);
            M_cur = M_first + (offset - node_offset * difference_type(S_buffer_size()));
        }
        return *this;
    }

    Self& operator+(difference_type n) const
    {
        Self tmp = *this;
        return tmp += n;
    }

    Self& operator-=(difference_type n)     {  return *this += -n; }

    Self& operator-(difference_type n) const
    {
        Self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(const Self& x) const { return M_cur == x.M_cur; }
    bool operator!=(const Self& x) const { return !(*this == x); }
    bool operator<(const Self& x) const
    { return (M_node == x.M_node) ? (M_cur < x.M_cur) : (M_node < x.M_node); }
    bool operator>(const Self& x) const { return (x < *this); }
    bool operator<=(const Self& x) const { return !(x < *this); }
    bool operator>=(const Self& x) const { return !(*this < x); }
};






template <class Tp, class Alloc, bool is_static>
class Deque_alloc_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;

    allocator_type get_allocator() const { return M_node_allocator; }

    Deque_alloc_base(const allocator_type& a)
    :M_node_allocator(a), M_map_allocator(a), M_map(0), M_map_size(0) {}


protected:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type Map_allocator_type;
    allocator_type M_node_allocator;
    allocator_type M_map_allocator;

    Tp* M_allocate_node()
    {  return M_node_allocator.allocate(deque_buf_size(sizeof(Tp))); }

    void M_deallocate_node(Tp* p)
    {  M_node_allocator.deallocate(p, deque_buf_size(sizeof(Tp))); }

    Tp** M_allocate_map(size_t n)
    { return M_map_allocator.allocate(n); }

    void M_deallocate_map(Tp** p, size_t n)
    {  M_map_allocator.deallocate(p, n);  }

    Tp** M_map;
    size_t M_map_size;
};


// Specialization for instanceless allocators.
template <class Tp, class Alloc>
class Deque_alloc_base<Tp, Alloc, true>
{
public:
    typedef typename Alloc_traits<Tp,Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    Deque_alloc_base(const allocator_type&) : M_map(0), M_map_size(0) {}

protected:
    typedef typename Alloc_traits<Tp, Alloc>::Alloc_type Node_alloc_type;
    typedef typename Alloc_traits<Tp*, Alloc>::Alloc_type Map_alloc_type;

    Tp* _M_allocate_node() 
    {  return Node_alloc_type::allocate(deque_buf_size(sizeof(Tp)));  }
    
    void _M_deallocate_node(Tp* p) 
    {  Node_alloc_type::deallocate(p, deque_buf_size(sizeof(Tp)));  }
    
    Tp** _M_allocate_map(size_t n)
    { return Map_alloc_type::allocate(n); }
    
    void _M_deallocate_map(Tp** p, size_t n)
    { Map_alloc_type::deallocate(p, n); }

    Tp** M_map;
    size_t M_map_size;
};




template <class Tp, class Alloc>
class Deque_base : public Deque_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instalceless>
{
public:
    typedef Deque_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instalceless>    Base;
    typedef typename Base::allocator_type                                           allocator_type;
    typedef Deque_iterator<Tp, Tp&, Tp*>                                            iterator;
    typedef Deque_iterator<Tp, const Tp&, const Tp*>                                const_iterator;


    Deque_base(const allocator_type& a, size_t num_elements) : Base(a), M_start(), M_finish()
    { M_initialize_map(num_elements); }
    Deque_base(const allocator_type& a) : Base(a), M_start(), M_finish() {}
    ~Deque_base();

protected:
    void M_initialize_map(size_t);
    void M_create_nodes(Tp** nstart, Tp** nfinish);
    void M_destroy_nodes(Tp** nstart, Tp** nfinish);
    enum { S_initial_map_size = 8 };

protected:
    iterator M_start;
    iterator M_finish;

};

template <class Tp, class Alloc>
Deque_base<Tp, Alloc>::~Deque_base()
{
    if(M_map)
    {
        M_destroy_nodes(M_start.M_node, M_finish.M_node + 1);
        M_deallocate_map(M_map, M_map_size);
    }
}

template <class Tp, class Alloc>
void Deque_base<Tp, Alloc>::M_initialize_map(size_t num_elements)
{
    size_t num_nodes = num_elements / deque_buf_size(sizeof(Tp)) + 1;
    M_map_size = max((size_t)S_initial_map_size, num_nodes + 2);
    M_map = M_allocate_map(M_map_size);

    Tp** nstart = M_map + (M_map_size - num_nodes) / 2;
    Tp** nfinish = nstart + num_nodes;
    try
    {
        M_create_nodes(nstart, nfinish);
    }
    catch (...)
    {
        M_deallocate_map(M_map, M_map_size), M_map = 0, M_map_size = 0;
        throw;
    }

    M_start = M_set_node(nstart);
    M_finish = M_set_node(nfinish - 1);
    M_start.M_cur = M_start.M_first;
    M_finish.M_cur = M_finish.M_first + num_elements % deque_buf_size(sizeof(Tp));
}

template <class Tp, class Alloc>
void Deque_base<Tp, Alloc>::M_create_nodes(Tp **nstart, Tp **nfinish)
{
    Tp** cur;
    try
    {
        for(cur = nstart; cur < nfinish; ++cur)
            *cur = M_allocate_node();
    }
    catch (...)
    {
        M_destroy_nodes(nstart, cur);
        throw;
    }
}

template <class Tp, class Alloc>
void Deque_base<Tp, Alloc>::M_destroy_nodes(Tp **nstart, Tp **nfinish)
{
    for(Tp** n = nstart; n < nfinish; ++n)
        M_deallocate_node(*n);
}


















}























#endif //MYSTL_STL_DEQUE_H
