//
// Created by GW00243464 on 2022/8/19.
//

#ifndef MYSTL_STL_VECTOR_H
#define MYSTL_STL_VECTOR_H

#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

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




template <class Tp, class Alloc>
class Vector_base : public Vector_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instalceless>
{
    typedef Vector_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instalceless> Base;
    typedef typename Base::allocator_type allocator_type;
public:
    Vector_base(const allocator_type& a) : Base(a) {}
    Vector_base(size_t n, const allocator_type& a) : Base(a){
        M_start = M_allocate(n);
        M_finish = M_start;
        M_end_of_storage = M_start + n;
    }
    ~Vector_base() { M_deallocate(M_start, M_end_of_storage - M_start); }

};



template <class Tp, class Alloc=default_alloc(Tp)>
class vector : protected Vector_base<Tp, Alloc>
{
private:
    typedef Vector_base<Tp, Alloc> Base;
public:
    typedef Tp                  value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef typename Base::allocate_type allocate_type;
    allocate_type get_allocator() const { return Base::get_allocator(); }


    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator > reverse_iterator;


protected:
    void M_insert_aux(iterator position, const Tp& x);
    void M_insert_aux(iterator position);

public:
    iterator begin() { return M_start; }
    const iterator begin() const { return M_start; }
    iterator end() { return M_finish; }
    const iterator end() const { return M_finish; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(end()); }
    const reverse_iterator rend() const { return const_reverse_iterator(end()); }

    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1) / sizeof(Tp); }
    size_type capacity() const { return M_end_of_storage - begin(); }
    bool empty() const { return begin() == end(); }

    reference operator[](size_type n) { return *(begin() + n); }
    const_reference operator[](size_type n) const { return *(begin() + n); }


    vector(const Tp* first, const Tp* last, const allocator_type& a = allocate_type())
            : Base(lase - first, a)
    { M_finish = uninitialized_copy(first, last, M_start); }

    ~vector() { destroy(M_start, M_finish); }

    vector<Tp, Alloc>& operator=(const vector<Tp, Alloc>& x);

    void reserve(size_type n)
    {
        if(capacity() < n)
        {
            const size_type old_size = size();
            iterator tmp = M_allocate_and_copy(n, M_start, M_finish);
            //TODO
        }
    }

    void assign(size_type n, const Tp& val) { M_fill_assign(n, val); }
    void M_fill_assign(size_type n, const Tp& val);


    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *begin(); }
    const_reference back() const { return *begin(); }

    void push_back(const Tp& x)
    {
        if(M_finish != M_end_of_storage)
        {
            construct(M_finish, x);
            ++M_finish;
        }
        else
            M_insert_aux(end(),x);
    }
    void push_back()
    {
        if(M_finish != M_end_of_storage)
        {
            construct(M_finish);
            ++M_finish;
        }
        else
            M_insert_aux(end());
    }

    void swap(vector<Tp, Alloc>& x)
    {
        std::swap(M_start, x.M_start);
        std::swap(M_finish, x.M_finish);
        std::swap(M_end_of_storage, x.M_end_of_storage);

    }

    iterator insert(iterator position, const Tp& x)
    {
        size_type n = position - begin();
        if(M_finish != M_end_of_stoage && position == end())
        {
            construct(M_finish, x);
            ++M_finish;
        }
        else
            M_insert_aux(position, x);
        return begin() + n;
    }

    iterator insert(iterator position)
    {
        size_type n = position - begin();
        if(M_finish != M_end_of_stoage && position == end())
        {
            construct(M_finish);
            ++M_finish;
        }
        else
            M_insert_aux(position);
        return begin() + n;
    }

    void insert(iterator position, const_iterator first, const_iterator last);
    void insert(iterator pos, size_type n, const Tp& x)
    { M_fill_insert(pos, n, x); }
    void M_fill_insert(iterator pos, size_type n, const Tp& x);

    void pop_back()
    {
        --M_finish;
        destroy(M_finish);
    }
    iterator erase(iterator position)
    {
        if(position + 1 != end())
            std::copy(position+1, M_finish, position);
        --M_finish;
        destroy(M_finish);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, M_finish, first);
        destroy(i ,M_finish);
        M_finish = M_finish - (last - first);
        return first;
    }

    void resize(size_type new_size, const Tp& x)
    {
        if(new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void resize(size_type new_size)    { resize(new_size, Tp()); }
    void clear() { erase(begin(), end()); }


protected:
    iterator M_allocate_and_copy(size_type n, const_iterator first, const_iterator last)
    {
        iterator result = M_allocate(n);
        try
        {
            uninitialized_copy(first, last, result);
            return result;
        }
        catch (...)
        {
            M_deallocate(result, n);
            throw;
        }
    }
};


template <class Tp, class Alloc>
inline bool operator==(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y)
{
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <class Tp, class Alloc>
inline bool operator<(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y)
{
    return std::lexicographical_compare(x.begin(), x.end(),
                                   y.begin(), y.end());
}



template <class Tp, class Alloc>
inline void swap(vector<Tp, Alloc>& x, vector<Tp, Alloc>& y)
{
    x.swap(y);
}

template <class Tp, class Alloc>
inline bool
operator!=(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y) {
    return !(x == y);
}

template <class Tp, class Alloc>
inline bool
operator>(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y) {
    return y < x;
}

template <class Tp, class Alloc>
inline bool
operator<=(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y) {
    return !(y < x);
}

template <class Tp, class Alloc>
inline bool
operator>=(const vector<Tp, Alloc>& x, const vector<Tp, Alloc>& y) {
    return !(x < y);
}


template <class Tp, class Alloc>
vector<Tp, Alloc>& vector<Tp, Alloc>::operator=(const vector<Tp, Alloc> &x)
{
    if(&x != this)
    {
        const size_type xlen = x.size();
        if(xlen > capacity())
        {
            iterator tmp = M_allocate_and_copy(xlen, begin(), end());
            destroy(M_start, M_finish);
            M_deallocate(M_start, M_end_of_stoage - M_start);
            M_start = tmp;
            M_end_of_storate = tmp + xlen;
        }
        else if (size() >= xlen)
        {
            iterator i = std::copy(x.begin(), x.end(), begin());
            destroy(i, M_finish);
        }
        else
        {
            std::copy(x.begin(), x.begin() + size(), M_start);
            uninitialized_copy(x.begin() + size(), x.end(), M_finish);
        }
        M_finish = M_start + xlen;
    }
    return *this;
}


template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_fill_assign(MySTL::vector<Tp, Alloc>::size_type n, const Tp &val)
{
    if(n > capacity())
    {
        vector<Tp, Alloc> tmp(n, val, get_allocator());
        tmp.swap(*this);
    }
    else if(n > size())
    {
        std::fill(begin(), end(), val);
        M_finish = uninitialized_fill_n(M_finish, n - size(), val);
    }
    else
        erase(std::fill_n(begin(), n, val), end());
}



template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_insert_aux(iterator position, const Tp& x)
{
    if(M_finish != M_end_of_storage)
    {
        construct(M_finish, *(M_finish - 1));
        ++M_finish;
        Tp x_copy = x;
        std::copy_backward(position, M_finish - 2, M_finish - 1);
        *position = x_copy;
    }
    else
    {
        const size_type old_size = size();
        const size_type len = (old_size != 0) ? 2 * old_size : 1;
        iterator new_start = M_allocate(len);
        iterator new_finish = new_start;
        try
        {
            new_finish = uninitialized_copy(M_start, position, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = uninitialized_copy(position, M_finish, new_finish);
        }
        catch (...)
        {
            destroy(new_start, new_finish), M_deallocate(new_start, len);
            throw;
        }
        destroy(begin(), end());
        M_start = new_start;
        M_finish = new_finish;
        M_end_of_storage = new_start + len;
    }
}



template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_insert_aux(iterator position)
{
    if(M_finish != M_end_of_storage)
    {
        construct(M_finish, *(M_finish - 1));
        ++M_finish;
        Tp x_copy = x;
        std::copy_backward(position, M_finish - 2, M_finish - 1);
        *position = Tp();
    }
    else
    {
        const size_type old_size = size();
        const size_type len = (old_size != 0) ? 2 * old_size : 1;
        iterator new_start = M_allocate(len);
        iterator new_finish = new_start;
        try
        {
            new_finish = uninitialized_copy(M_start, position, new_start);
            construct(new_finish);
            ++new_finish;
            new_finish = uninitialized_copy(position, M_finish, new_finish);
        }
        catch (...)
        {
            destroy(new_start, new_finish), M_deallocate(new_start, len);
            throw;
        }
        destroy(begin(), end());
        M_start = new_start;
        M_finish = new_finish;
        M_end_of_storage = new_start + len;
    }
}

template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_fill_insert(iterator position, size_type n, const Tp &x)
{
    if(n != 0)
    {
        if(size_type(M_end_of_storge - M_finish) >= n)
        {
            Tp x_copy = x;
            const size_type elems_after = M_finish - position;
            iterator old_finish = M_finish;
            if(elems_after > n)
            {
                uninitialized_copy(M_finish - n, M_finish, M_finish);
                M_finish = M_finish + n;
                std::copy_backward(position, old_finish - n, old_finish);
                std::fill(position, position + n, x_copy);
            }
            else
            {
                uninitialized_fill_n(M_finish, n - elems_after, x_copy);
                M_finish += n - elems_after;
                uninitialized_copy(position, old_finish, M_finish);
                M_finish += elems_after;
                std::fill(position, old_finish, x_copy);
            }
        }
        else
        {
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, n);
            iterator new_start = M_allocate(len);
            iterator new_finish = new_start;
            try {
                    __new_finish = uninitialized_copy(_M_start, __position, __new_start);
                    __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
                    __new_finish
                    = uninitialized_copy(__position, _M_finish, __new_finish);
            }
            catch (...)
            {
                destroy(new_start,new_finish);
                _M_deallocate(new_start,len);
                throw;
            }
            destroy(M_start, M_finish);
            M_deallocate(M_start, M_end_of_storage - M_start);
            M_start = new_start;
            M_finish = new_finish;
            M_end_of_storage = new_start + len;
        }
    }
}


template <class Tp, class Alloc>
void vector<Tp, Alloc>::insert(iterator position, const_iterator first, const_iterator last)
{
    if(first != last)
    {
        size_type n = 0;
        distance(first, last, n);
        if( size_type(M_end_of_storage - M_finish) >= n)
        {
            const size_type elems_after = M_finish - position;
            iterator old_finish = M_finish;
            if(elems_after > n)
            {
                uninitialized_copy(M_finish - n, M_finish, M_finish);
                M_finish += n;
                std::copy_backward(position, old_finish - n, old_finish);
                std::copy(first, last, position);
            }
            else
            {
                uninitialized_copy(first + elems_after, last, M_finish);
                M_finish += n - elems_after;
                uninitialized_copy(position, old_finish, M_finish);
                M_finish += elems_after;
                std::copy(first, first + elems_after, position);
            }

        }
        else
        {
            const size_type old_size = size();
            const size_type len = old_size + st::max(old_size, n);
            iterator new_start = M_allocate(len);
            iterator new_finish = new_start;
            try {
                    new_finish = uninitialized_copy(M_start, position, new_start);
                    new_finish = uninitialized_copy(first, last, new_finish);
                    new_finish
                    = uninitialized_copy(position, M_finish, new_finish);
            }
            catch(...)
            {
                destroy(new_start,new_finish);
                M_deallocate(new_start,len);
            }
            destroy(M_start, M_finish);
            M_deallocate(M_start, M_end_of_storage - M_start);
            M_start = new_start;
            M_finish = new_finish;
            M_end_of_storage = new_start + len;
        }

    }
}














}











#endif //MYSTL_STL_VECTOR_H
