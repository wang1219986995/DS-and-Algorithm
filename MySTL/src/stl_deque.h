//

#ifndef MYSTL_STL_DEQUE_H
#define MYSTL_STL_DEQUE_H

#include <stdlib.h>
// #include "stl_algo.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

namespace MySTL
{

inline size_t deque_buf_size(size_t size)
{ return size < 512 ? size_t(512 / size) : size_t(1); }

template <class Tp, class Ref, class Ptr>
struct Deque_iterator
{
public:
    typedef Deque_iterator<Tp, Tp&, Tp*>                iterator;
    typedef Deque_iterator<Tp, const Tp&, const Tp*>    const_iterator;
    static size_t S_buffer_size() { return deque_buf_size(sizeof(Tp)); }

    typedef random_access_iterator_tag  iterator_category;
    typedef Tp                          value_type;
    typedef Ptr                         pointer;
    typedef Ref                         reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef Tp**                        Map_pointer;
    typedef Deque_iterator              Self;

    Tp* M_cur;
    Tp* M_first;
    Tp* M_last;
    Map_pointer M_node;

    Deque_iterator(Tp* x, Map_pointer y) : M_cur(x), M_first(*y),
    M_last(*y + S_buffer_size()), M_node(y) {}

    Deque_iterator():M_cur(0), M_first(0), M_last(0), M_node(0) {}

    Deque_iterator(const iterator& x) : M_cur(x.M_cur), M_first(x.M_first),
    M_last(x.M_last), M_node(x.M_node) {}

    void M_set_node(Map_pointer new_node)
    {
        M_node = new_node;
        M_first = *new_node;
        M_last = M_first + difference_type(S_buffer_size());
    }

    reference operator*() const { return *M_cur; }
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

template <class Tp, class Ref, class Ptr>
inline Deque_iterator<Tp, Ref, Ptr>
operator+(ptrdiff_t n, const Deque_iterator<Tp, Ref, Ptr>& x)
{
    return x + n;
}

template <class Tp, class Ref, class Ptr>
inline random_access_iterator_tag
iterator_category(const Deque_iterator<Tp, Ref, Ptr>& )
{
    return random_access_iterator_tag();
}

template <class Tp, class Ref, class Ptr>
inline Tp* value_type(const Deque_iterator<Tp,Ref,Ptr>&) { return 0; }

template <class Tp, class Ref, class Ptr>
inline ptrdiff_t* distance_type(const Deque_iterator<Tp,Ref,Ptr>&) { return 0;}




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

    Tp* M_allocate_node()
    {  return Node_alloc_type::allocate(deque_buf_size(sizeof(Tp)));  }
    
    void M_deallocate_node(Tp* p)
    {  Node_alloc_type::deallocate(p, deque_buf_size(sizeof(Tp)));  }
    
    Tp** M_allocate_map(size_t n)
    { return Map_alloc_type::allocate(n); }
    
    void M_deallocate_map(Tp** p, size_t n)
    { Map_alloc_type::deallocate(p, n); }

    Tp** M_map;
    size_t M_map_size;
};


template <class Tp, class Alloc>
class Deque_base : public Deque_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>
{
public:
    typedef Deque_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>    Base;
    typedef typename Base::allocator_type                                           allocator_type;
    typedef Deque_iterator<Tp, Tp&, Tp*>                                            iterator;
    typedef Deque_iterator<Tp, const Tp&, const Tp*>                                const_iterator;


    Deque_base(const allocator_type& a, size_t num_elements) : Base(a), M_start(), M_finish()
    { M_initialize_map(num_elements); }
    Deque_base(const allocator_type& a) : Base(a), M_start(), M_finish() {}
    Deque_base() : Base(allocator_type()), M_start(), M_finish() {}
    ~Deque_base();

protected:
    using Base::M_map;
    using Base::M_map_size;
    using Base::M_allocate_map;
    using Base::M_deallocate_map;
    using Base::M_allocate_node;
    using Base::M_deallocate_node;

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
    M_map_size = std::max((size_t)S_initial_map_size, num_nodes + 2);
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

    M_start.M_set_node(nstart);
    M_finish.M_set_node(nfinish - 1);
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






template <class Tp, class Alloc=allocator<Tp>>
class deque : protected Deque_base<Tp, Alloc>
{
public:
    typedef Deque_base<Tp, Alloc> Base;
    typedef Tp                  value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef typename Base::iterator         iterator;
    typedef typename Base::const_iterator   const_iterator;

    typedef typename Base::allocator_type  allocator_type;
    allocator_type get_allocator() const { return Base::get_allocator(); }

    typedef reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef reverse_iterator<iterator>          reverse_iterator;

protected:
    typedef pointer* Map_pointer;
    static size_t S_buffer_size() { return deque_buf_size(sizeof(Tp)); }

protected:
//    using Base::M_initialize_map;
//    using Base::M_create_nodes;
    using Base::M_destroy_nodes;
    using Base::M_deallocate_node;
    using Base::M_allocate_node;
    using Base::M_allocate_map;
    using Base::M_deallocate_map;

    using Base::M_map;
    using Base::M_map_size;
    using Base::M_start;
    using Base::M_finish;

public:
    iterator begin() { return M_start; }
    iterator end() { return M_finish; }
    const_iterator begin() const { return M_start; }
    const_iterator end() const { return M_finish; }

    reverse_iterator rbegin() { return reverse_iterator(M_finish); }
    reverse_iterator rend() { return reverse_iterator(M_start); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(M_finish); }
    const_reverse_iterator rend() const { return const_reverse_iterator(M_start); }

    reference operator[](size_type n)
    {  return M_start[difference_type(n)];  }

    const_reference operator[](size_type n) const
    {  return M_start[difference_type(n)];  }

    reference front() { return *M_start; }
    reference back()
    {
        iterator tmp = M_finish;
        --tmp;
        return *tmp;
    }

    const_reference front() const { return *M_start; }
    reference back() const
    {
        iterator tmp = M_finish;
        --tmp;
        return *tmp;
    }

    size_type size() const {  return M_finish - M_start; }
    size_type max_size() const {  return size_type(-1); }
    bool empty() const { return M_finish == M_start; }

public:
    explicit deque(const allocator_type& a = allocator_type()) : Base(a, 0) {}

    deque(const deque& x) : Base(x.get_allocator(), x.size())
    { MySTL::uninitialized_copy(x.begin(), x.end(), M_start); }

    deque(size_type n, const value_type& value, const allocator_type& a = allocator_type()) : Base(a, n)
    { M_fill_initialize(value); }

    explicit deque(size_type n) : Base(allocator_type(), n)
    {  M_fill_initialize(value_type()); }

    template <class InputIterator>
    deque(InputIterator first, InputIterator last, const allocator_type& a = allocator_type())
    : Base(a)
    {
        typedef typename Is_integer<InputIterator>::Integral Integral;
        M_initialize_dispatch(first, last, Integral());
    }

    template<class Integer>
    void M_initialize_dispatch(Integer n, Integer x, __true_type)
    {
        M_initialize_map(n);
        M_fill_initialize(x);
    }

    template<class InputIter>
    void M_initialize_dispatch(InputIter first, InputIter last, __false_type)
    {
        M_range_initialize(first, last, ITERATOR_CATEGORY(first));
    }


    ~deque() { MySTL::destroy(M_start, M_finish); }

    deque& operator= (const deque& x)
    {
        const size_type len = size();
        if(&x != this)
        {
            if(len >= x.size())
                erase(copy(x.begin(), x.end(), M_start), M_finish);
            else
            {
                const_iterator mid = x.begin() + difference_type(len);
                std::copy(x.begin(), mid, M_start);
                insert(M_finish, mid, x.end());
            }
        }
        return *this;
    }

    void swap(deque& x)
    {
        swap(M_start, x.M_start);
        swap(M_finish, x.M_finish);
        swap(M_map, x.M_map);
        swap(M_map_size, x.M_map_size);
    }

public:
    void M_fill_assign(size_type n, const Tp& val)
    {
        if(n > size())
        {
            fill(begin(), end(), val);
            insert(end(), n - size(), val);
        }
        else
        {
            erase(begin() + n, end());
            fill(begin(), end(), val);
        }
    }

    void assign(size_type n, const Tp& val)
    { M_fill_assign(n, val); }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        typedef typename Is_integer<InputIterator>::Integral Integral;
        M_assign_dispatch(first, last, Integral());
    }

private:
    template<class Integer>
    void M_assign_dispatch(Integer n, Integer val, __true_type)
    { M_fill_assign((size_type) n, (Tp) val); }

    template<class InputIterator>
    void M_assign_dispatch(InputIterator first, InputIterator last, __false_type)
    {
        M_assign_aux(first, last, ITERATOR_CATEGORY(first));
    }

    template<class InputIterator>
    void M_assign_aux(InputIterator first, InputIterator last, input_iterator_tag);

    template<class ForwardIterator>
    void M_assign_aux(ForwardIterator first, ForwardIterator last,
                      forward_iterator_tag)
    {
        size_type len = 0;
        MySTL::distance(first, last, len);
        if(len > size())
        {
            ForwardIterator mid = first;
            advance(mid, size());
            std::copy(first, mid, begin());
            insert(end(), mid, last);
        }
        else
            erase(copy(first, last, begin()), end());
    }

public:
    void push_back(const value_type& t)
    {
        if(M_finish.M_cur != M_finish.M_last - 1)
        {
            construct(M_finish.M_cur, t);
            ++M_finish.M_cur;
        }
        else
            M_push_back_aux(t);
    }

    void push_back()
    {
        if(M_finish.M_cur != M_finish.M_last - 1)
        {
            construct(M_finish.M_cur);
            ++M_finish.M_cur;
        }
        else
            M_push_back_aux();
    }

    void push_front(const value_type& t)
    {
        if(M_start.M_cur != M_start.M_first)
        {
            construct(M_start.M_cur - 1, t);
            --M_start.M_cur;
        }
        else
            M_push_front_aux(t);
    }

    void push_front()
    {
        if(M_start.M_cur != M_start.M_first)
        {
            construct(M_start.M_cur - 1);
            --M_start.M_cur;
        }
        else
            M_push_front_aux();
    }

    void pop_back()
    {
        if(M_finish.M_cur!= M_finish.M_first)
        {
            --M_finish.M_cur;
            destroy(M_finish.M_cur);
        }
        else
            M_pop_back_aux();
    }

    void pop_front()
    {
        if(M_start.M_cur != M_start.M_last - 1)
        {
            destroy(M_start.M_cur);
            ++M_start.M_cur;
        }
        else
            M_pop_front_aux();
    }


public:         //insert
    iterator insert(iterator position, const value_type& x)
    {
        if(position.M_cur == M_start.M_cur)
        {
            push_front(x);
            return M_start;
        }
        else if(position.M_cur == M_finish.M_cur)
        {
            push_back(x);
            iterator tmp = M_finish;
            --tmp;
            return tmp;
        }
        else
            return M_insert_aux(position, x);
    }

    iterator insert(iterator position)
    {  return insert(position, value_type());  }

    void insert(iterator position, size_type n, const value_type& x)
    {  M_fill_insert(position, n, x);  }

    void M_fill_insert(iterator position, size_type n, const value_type& x);

    template<class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last)
    {
        typedef typename Is_integer<InputIterator>::Integral Integral;
        M_insert_dispatch(pos, first, last, Integral());
    }

    template<class Integer>
    void M_insert_dispatch(iterator pos, Integer n, Integer x, __true_type)
    {
        M_fill_insert(pos, (size_type) n, (value_type) x);
    }

    template<class InputIterator>
    void M_insert_dispatch(iterator pos, InputIterator first, InputIterator last, __false_type)
    {
        insert(pos, first, last, ITERATOR_CATEGORY(first));
    }

    void resize(size_type new_size, const value_type& x)
    {
        const size_type len = size();
        if(new_size < len)
            erase(M_start + new_size, M_finish);
        else
            insert(M_finish, new_size - len, x);
    }

    void resize(size_type new_size) {  resize(new_size, value_type());  }

public:     // erase
    iterator erase(iterator pos)
    {
        iterator next = pos;
        ++next;
        difference_type index = pos - M_start;
        if(size_type(index) < (this->size() >> 1))
        {
            copy_backward(M_start, pos, next);
            pop_front();
        }
        else
        {
            copy(next, M_finish, pos);
            pop_back();
        }
    }

    iterator erase(iterator first, iterator last);
    void clear();

protected:
    void M_fill_initialize(const value_type& value);

    template<class InputIterator>
    void M_range_initialize(InputIterator first, InputIterator last, input_iterator_tag);

    template<class ForwardIterator>
    void M_range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

protected:
    void M_push_back_aux(const value_type&);
    void M_push_back_aux();
    void M_push_front_aux(const value_type&);
    void M_push_front_aux();
    void M_pop_back_aux();
    void M_pop_front_aux();

protected:
    template<class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, input_iterator_tag);

    template<class ForwardIterator>
    void insert(iterator pos, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

    iterator M_insert_aux(iterator pos, const value_type& x);
    iterator M_insert_aux(iterator pos);
    void M_insert_aux(iterator pos, size_type n, const value_type& x);

    template<class ForwardIterator>
    void M_insert_aux(iterator pos, ForwardIterator first, ForwardIterator last, size_type n);

    iterator M_reserve_elements_at_front(size_type n)
    {
        size_type vacancies = M_start.M_cur - M_start.M_first;
        if(n > vacancies)
            M_new_elements_at_front(n - vacancies);
        return M_start - difference_type(n);
    }

    iterator M_reserve_elements_at_back(size_type n)
    {
        size_type vacancies = (M_finish.M_last - M_finish.M_cur) - 1;
        if(n > vacancies)
            M_new_elements_at_back(n - vacancies);
        return M_finish + difference_type(n);
    }

    void M_new_elements_at_front(size_type new_elements);
    void M_new_elements_at_back(size_type new_elements);

protected:    // allocation of M_map and nodes
    void M_reserve_map_at_back(size_type nodes_to_add = 1)
    {
        if(nodes_to_add + 1 > M_map_size - (M_finish.M_node - M_map))
            M_reallocate_map(nodes_to_add, false);
    }

    void M_reserve_map_at_front(size_type nodes_to_add = 1)
    {
        if(nodes_to_add > size_type(M_start.M_node - M_map))
            M_reallocate_map(nodes_to_add, true);
    }

    void M_reallocate_map(size_type nodes_to_add, bool add_at_front);
};

template <class Tp, class Alloc>
template <class InputIterator>
void deque<Tp, Alloc>::M_assign_aux(InputIterator first, InputIterator last, MySTL::input_iterator_tag)
{
    iterator cur = begin();
    for(; first != last && cur != end(); ++cur, ++first)
        *cur = *first;
    if(first == last)
        erase(cur, end());
    else
        insert(end(), first, last);
}

template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_fill_insert(iterator pos, size_type n, const value_type& x)
{
    if(pos.M_cur == M_start.M_cur)
    {
        iterator new_start = M_reserve_elements_at_front(n);
        try
        {
            MySTL::uninitialized_fill(new_start, M_start, x);
            M_start = new_start;
        }
        catch (...)
        {
            M_destroy_nodes(new_start.M_node, M_start.M_node);
            throw;
        }
    }
    else if(pos.M_cur == M_finish.M_cur)
    {
        iterator new_finish = M_reserve_elements_at_back(n);
        try
        {
            MySTL::uninitialized_fill(M_finish, new_finish, x);
            M_finish = new_finish;
        }
        catch (...)
        {
            M_destroy_nodes(M_finish.M_node + 1, new_finish.M_node + 1);
            throw;
        }
    }
}


template <class Tp, class Alloc>
typename deque<Tp, Alloc>::iterator deque<Tp, Alloc>::erase(iterator first, iterator last)
{
    if(first == M_start && last == M_finish)
    {
        clear();
        return M_finish;
    }
    else
    {
        difference_type n = last - first;
        difference_type elems_before = first - M_start;
        if(elems_before < difference_type((this->size() - n) / 2))
        {
            copy_backward(M_start, first, last);
            iterator new_start = M_start + n;
            MySTL::destroy(M_start, new_start);
            M_destroy_nodes(M_start.M_node, new_start.M_node);
            M_start = new_start;
        }
        else
        {
            std::copy(last, M_finish, first);
            iterator new_finish = M_finish -n;
            MySTL::destroy(new_finish, M_finish);
            M_destroy_nodes(new_finish.M_node + 1, M_finish.M_node + 1);
            M_finish = new_finish;
        }
        return M_start + elems_before;
    }
}

template  <class Tp, class Alloc>
void deque<Tp, Alloc>::clear()
{
    for(Map_pointer node = M_start.M_node + 1; node < M_finish.M_node; ++node)
    {
        MySTL::destroy(*node, *node + S_buffer_size());
        M_deallocate_node(*node);
    }

    if(M_start.M_node != M_finish.M_node)
    {
        MySTL::destroy(M_start.M_cur, M_start.M_last);
        MySTL::destroy(M_finish.M_first, M_finish.M_cur);
        M_deallocate_node(M_finish.M_first);
    }
    else
        MySTL::destroy(M_start.M_cur, M_finish.M_cur);

    M_finish = M_start;
}

template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_fill_initialize(const value_type &value)
{
    Map_pointer cur;
    try
    {
        for (cur = M_start.M_node; cur < M_finish.M_node; ++cur)
            MySTL::uninitialized_fill(*cur, *cur + S_buffer_size(), value);
        MySTL::uninitialized_fill(M_finish.M_first, M_finish.M_cur, value);
    }
    catch (...)
    {
        MySTL::destroy(M_start, iterator(*cur, cur));
        throw;
    }
}

template <class Tp, class Alloc>
template <class InputIterator>
void deque<Tp, Alloc>::M_range_initialize(InputIterator first, InputIterator last, MySTL::input_iterator_tag)
{
    Base::M_initialize_map(0);
    try
    {
        for(; first != last; ++first)
            push_back(*first);
    }
    catch (...)
    {
        clear();
        throw;
    }
}

template <class Tp, class Alloc>
template <class ForwardIterator>
void deque<Tp, Alloc>::M_range_initialize(ForwardIterator first, ForwardIterator last, MySTL::forward_iterator_tag)
{
    size_type n = 0;
    distance(first, last, n);
    Base::M_initialize_map(n);
    Map_pointer cur_node;
    try
    {
        for(cur_node = M_start.M_node; cur_node < M_finish.M_node; ++cur_node)
        {
            ForwardIterator mid = first;
            MySTL::advance(mid, S_buffer_size());
            MySTL::uninitialized_copy(first, mid, *cur_node);
            first = mid;
        }
        MySTL::uninitialized_copy(first, last, M_finish.M_first);
    }
    catch (...)
    {
        MySTL::destroy(M_start, iterator(*cur_node, cur_node));
        throw;
    }
}

// called only if M_finish.M_cur == M_finish.M_last - 1
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_push_back_aux(const value_type& t)
{
    value_type t_copy = t;
    M_reserve_map_at_back();
    *(M_finish.M_node + 1) = M_allocate_node();
    try
    {
        construct(M_finish.M_cur, t_copy);
        M_finish.M_set_node(M_finish.M_node + 1);
        M_finish.M_cur = M_finish.M_first;
    }
    catch (...)
    {
        M_deallocate_node(*(M_finish.M_node + 1));
        throw;
    }
}

// only if M_finish.M_cur == M_finish.M_last - 1
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_push_back_aux()
{
    M_reserve_map_at_back();
    *(M_finish.M_node + 1) = M_allocate_node();
    try
    {
        construct(M_finish.M_cur);
        M_finish.M_set_node(M_finish.M_node + 1);
        M_finish.M_cur = M_finish.first;
    }
    catch (...)
    {
        M_deallocate_node(*(M_finish.M_node + 1));
    }
}

// only if M_start.M_cur == M_start.M_first
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_push_front_aux(const value_type &t)
{
    value_type t_copy = t;
    M_reserve_map_at_front();
    *(M_start.M_node - 1) = M_allocate_node();
    try
    {
        M_start.M_set_node(M_start.M_node - 1);
        M_start.M_cur = M_start.M_last - 1;
        construct(M_start.M_cur, t_copy);
    }
    catch (...)
    {
        ++M_start;
        M_deallocate_node(*(M_start.M_node - 1));
    }
}

// only if M_start.M_cur == M_start.M_first
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_push_front_aux()
{
    M_reserve_map_at_front();
    *(M_start.M_node - 1) = M_allocate_node();
    try
    {
        M_start.M_set_node(M_start.M_node - 1);
        M_start.M_cur = M_start.M_last - 1;
        construct(M_start.M_cur);
    }
    catch (...)
    {
        ++M_start;
        M_deallocate_node(*(M_start.M_node - 1));
    }
}

// only if M_finish.M_cur == M_finish.M_first
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_pop_back_aux()
{
    M_deallocate_node(M_finish.M_first);
    M_finish.M_set_node(M_finish.M_node - 1);
    M_finish.M_cur = M_finish.M_last - 1;
    destroy(M_finish.M_cur);
}

// only if M_start.M_cur == M_start.M_last - 1
template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_pop_front_aux()
{
    destroy(M_start.M_cur);
    M_deallocate_node(M_start.M_first);
    M_start.M_set_node(M_start.M_node + 1);
    M_start.M_cur = M_start.M_first;
}

template <class Tp, class Alloc>
template <class InputIterator>
void deque<Tp, Alloc>::insert(iterator pos, InputIterator first, InputIterator last, input_iterator_tag)
{
    std::copy(first, last, inserter(*this, pos));
}

template <class Tp, class Alloc>
template <class ForwardIterator>
void deque<Tp, Alloc>::insert(iterator pos, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
{
    //TODO
    size_type n = 0;
    distance(first, last, n);
    if(pos.M_cur == M_start.M_cur)
    {
        iterator new_start = M_reserve_elements_at_front(n);
        try
        {
            uninitialized_copy(first, last, new_start);
            M_start = new_start;
        }
        catch (...)
        {
            M_destroy_nodes(new_start.M_node, M_start.M_node);
            throw;
        }
    }
    else if(pos.M_cur == M_finish.M_cur)
    {
        iterator new_finish = M_reserve_elements_at_back(n);
        try
        {
            uninitialized_copy(first, last, M_finish);
            M_finish = new_finish;
        }
        catch (...)
        {
            M_destroy_nodes(M_finish.M_node + 1, new_finish.M_node);
            throw;
        }
    }
    else
        M_insert_aux(pos, first, last, n);
}

template <class Tp, class Alloc>
typename deque<Tp, Alloc>::iterator
deque<Tp, Alloc>::M_insert_aux(iterator pos, const value_type& x)
{
    difference_type index = pos - M_start;
    value_type x_copy = x;
    if(size_type(index) < this->size() / 2)
    {
        push_front(front());
        iterator front1 = M_start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        pos = M_start + index;
        iterator pos1 = pos;
        ++pos1;
        copy(front2, pos1, front1);
    }
    else
    {
        push_back(back());
        iterator back1 = M_finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = M_start + index;
        copy_backward(pos, back2, back1);
    }
    *pos = x_copy;
    return pos;
}

template <class Tp, class Alloc>
typename deque<Tp, Alloc>::iterator deque<Tp, Alloc>::M_insert_aux(iterator pos)
{
    difference_type index = pos - M_start;
    if(size_type(index) < this->size() / 2)
    {
        push_front(front());
        iterator front1 = M_start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        pos = M_start + index;
        iterator pos1 = pos;
        ++pos1;
        copy(front2, pos1, front1);
    }
    else
    {
        push_back(back());
        iterator back1 = M_finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = M_start + index;
        copy_backward(pos, back2, back1);
    }
    *pos = value_type();
    return pos;
}

template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_insert_aux(iterator pos, size_type n, const value_type& x)
{
    const difference_type elems_before = pos - M_start;
    size_type length = this->size();
    value_type x_copy = x;
    if(elems_before < difference_type(length / 2))
    {
        iterator new_start = M_reserve_elements_at_front(n);
        iterator old_start = M_start;
        pos = M_start + elems_before;
        try
        {
            if(elems_before >= difference_type(n))
            {
                iterator start_n = M_start + difference_type(n);
                uninitialized_copy(M_start, start_n, new_start);
                M_start = new_start;
                std::copy(start_n, pos, old_start);
                std::fill(pos - difference_type(n), pos, x_copy);
            }
            else
            {
                __uninitialized_copy_fill(M_start, pos, new_start, M_start, x_copy);
                M_start = new_start;
                std::fill(old_start, pos, x_copy);
            }
        }
        catch (...)
        {
            M_destroy_nodes(new_start.M_node, M_start.M_node);
        }
    }
    else
    {
        iterator new_finish = M_reserve_elements_at_back(n);
        iterator old_finish = M_finish;
        const difference_type elems_after = difference_type(length) - elems_before;
        pos = M_finish - elems_after;
        try
        {
            if(elems_after > difference_type(n))
            {
                iterator finish_n = M_finish - difference_type(n);
                uninitialized_copy(finish_n, M_finish, M_finish);
                M_finish = new_finish;
                std::copy_backward(pos, finish_n, old_finish);
                std::fill(pos, pos + difference_type(n), x_copy);
            }
            else
            {
                __uninitialized_fill_copy(M_finish, pos + difference_type(n), x_copy, pos, M_finish);
                M_finish = new_finish;
                std::fill(pos, old_finish, x_copy);
            }
        }
        catch (...)
        {
            M_destroy_nodes(M_finish.M_node + 1, new_finish.M_node + 1);
        }
    }
}

template <class Tp, class Alloc>
template <class ForwardIterator>
void deque<Tp, Alloc>::M_insert_aux(iterator pos, ForwardIterator first, ForwardIterator last, size_type n)
{
    //TODO 需要修改
    const difference_type elemsbefore = pos - M_start;
    size_type length = size();
    if(elemsbefore < length / 2)
    {
        iterator new_start = M_reserve_map_at_front(n);
        iterator old_start = M_start;
        pos = M_start + elemsbefore;
        try
        {
            if(elemsbefore >= difference_type(n))
            {
                iterator start_n = M_start + difference_type(n);
                uninitialized_copy(M_start, start_n, new_start);
                M_start = new_start;
                std::copy(start_n, pos, old_start);
                std::copy(first, last, pos - difference_type(n));
            }
            else
            {
                ForwardIterator mid = first;
                advance(mid, difference_type(n) - elemsbefore);
                __uninitialized_copy_copy(M_start, pos, first, mid, new_start);
                M_start = new_start;
                std::copy(mid, last ,old_start);
            }
        }
        catch (...)
        {
            M_destroy_nodes(new_start.M_node, M_start.M_node);
            throw ;
        }
    }
    else
    {
        iterator new_finish = M_reserve_map_at_back(n);
        iterator old_finish = M_finish;
        const difference_type elemsafter = difference_type(length) - elemsbefore;
        pos = M_finish - elemsafter;
        try
        {
            if(elemsafter > difference_type(n))
            {
                iterator finish_n = M_finish - difference_type(n);
                uninitialized_copy(finish_n, M_finish, M_finish);
                M_finish = new_finish;
                std::copy_backward(pos, finish_n, old_finish);
                std::copy(first, last, pos);
            }
            else
            {
                ForwardIterator mid = first;
                advance(mid, elemsafter);
                __uninitialized_copy_copy(mid, last, pos, M_finish, M_finish);
                M_finish = new_finish;
                std::copy(first, mid, pos);
            }
        }
        catch (...)
        {
            M_destroy_nodes(M_finish.M_node + 1, new_finish.M_node + 1);
            throw;
        }
    }
}

template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_new_elements_at_front(size_type new_elems)
{
    size_type new_nodes = (new_elems + S_buffer_size() - 1) / S_buffer_size();
    M_reserve_map_at_front(new_nodes);
    size_type i;
    try
    {
        for(i = 1; i <= new_nodes; ++i)
            *(M_start.M_node - i) = M_allocate_node();
    }
    catch (...)
    {
        for(size_type j = 1; j < i; ++j)
            M_deallocate_node(*(M_start.M_node - j));
        throw ;
    }
}


template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_new_elements_at_back(size_type new_elems)
{
    size_type new_nodes = (new_elems + S_buffer_size() - 1) / S_buffer_size();
    M_reserve_map_at_back(new_nodes);
    size_type i;
    try
    {
        for(i = 1; i <= new_nodes; ++i)
            *(M_finish.M_node + i) = M_allocate_node();
    }
    catch (...)
    {
        for(size_type j = 1; j < i; ++j)
            M_deallocate_node(*(M_finish.M_node + j));
        throw ;
    }
}

template <class Tp, class Alloc>
void deque<Tp, Alloc>::M_reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_num_nodes = M_finish.M_node - M_start.M_node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    Map_pointer new_nstart;
    if(M_map_size > 2 * new_num_nodes)
    {
        new_nstart = M_map + (M_map_size - new_num_nodes) / 2 + (add_at_front? nodes_to_add : 0);
        if(new_nstart < M_start.M_node)
            copy(M_start.M_node, M_finish.M_node + 1, new_nstart);
        else
            copy_backward(M_start.M_node, M_finish.M_node + 1, new_nstart + old_num_nodes);
    }
    else
    {
        size_type new_map_size = M_map_size + std::max(M_map_size, nodes_to_add) + 2;

        Map_pointer new_map = M_allocate_map(new_map_size);
        new_nstart = new_map + ( new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        copy(M_start.M_node, M_finish.M_node + 1, new_nstart);
        M_deallocate_map(M_map, M_map_size);

        M_map = new_map;
        M_map_size = new_map_size;
    }

    M_start.M_set_node(new_nstart);
    M_finish.M_set_node(new_nstart + old_num_nodes - 1);
}



// Nonmember functions.

template <class Tp, class Alloc>
inline bool operator==(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}


template <class Tp, class Alloc>
inline bool operator<(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}


template <class Tp, class Alloc>
inline bool operator!=(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return !(x == y);
}

template <class Tp, class Alloc>
inline bool operator>(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return y < x;
}

template <class Tp, class Alloc>
inline bool operator<=(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return !(y < x);
}

template <class Tp, class Alloc>
inline bool operator>=(const deque<Tp, Alloc>& x, const deque<Tp, Alloc>& y)
{
    return !(x < y);
}

template <class Tp, class Alloc>
inline void swap(deque<Tp, Alloc>& x, deque<Tp, Alloc>& y)
{
    x.swap(y);
}

} // namespace























#endif //MYSTL_STL_DEQUE_H
