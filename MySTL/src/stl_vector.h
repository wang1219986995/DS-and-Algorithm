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


template <class Tp, class Allocator>
class Vector_alloc_base<Tp, Allocator, true>{
public:
    typedef typename Alloc_traits<Tp, Allocator>::allocator_type allocator_type;
    allocator_type get_allocator() const {return allocator_type();}
    Vector_alloc_base(const allocator_type&)
    : M_start(0), M_finish(0), M_end_of_storage(0) {}

protected:
    Tp* M_start;
    Tp* M_finish;
    Tp* M_end_of_storage;

    typedef typename Alloc_traits<Tp, Allocator>::Alloc_type Alloc_type;
    Tp* M_allocate(size_t n)
    {  return Alloc_type::allocate(n); }

    void M_deallocate(Tp* p, size_t n)
    {  if(p)  Alloc_type::deallocate(p, n);  }
};


template <class Tp, class Alloc>
class Vector_base : public Vector_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>
{
public:
    typedef Vector_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless> Base;
    typedef typename Base::allocator_type allocator_type;
public:
    Vector_base(const allocator_type& a) : Base(a) {}
    Vector_base(size_t n, const allocator_type& a) : Base(a){
        this->M_start = this->M_allocate(n);
        this->M_finish = this->M_start;
        this->M_end_of_storage = this->M_start + n;
    }
    ~Vector_base() { this->M_deallocate(this->M_start, this->M_end_of_storage - this->M_start); }
};







template <class Tp, class Alloc=allocator<Tp>>
class vector : protected Vector_base<Tp, Alloc>
{
public:
    typedef Vector_base<Tp, Alloc> Base;

    typedef Tp                  value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef typename Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return Base::get_allocator(); }


    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator > reverse_iterator;


protected:
    void M_insert_aux(iterator position, const Tp& x);
    void M_insert_aux(iterator position);

public:
    iterator begin() { return this->M_start; }
    const iterator begin() const { return this->M_start; }
    iterator end() { return this->M_finish; }
    const iterator end() const { return this->M_finish; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(end()); }
    const reverse_iterator rend() const { return const_reverse_iterator(end()); }

    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1) / sizeof(Tp); }
    size_type capacity() const { return this->M_end_of_storage - begin(); }
    bool empty() const { return begin() == end(); }

    reference operator[](size_type n) { return *(begin() + n); }
    const_reference operator[](size_type n) const { return *(begin() + n); }


    explicit vector(const typename Base::allocator_type& a = allocator_type()) : Base(a) {}

    vector(size_type n, const Tp& value, const typename Base::allocator_type& a = allocator_type())
    : Base(n, a)
    { this->M_finish = uninitialized_fill_n(this->M_start, n, value); }

    explicit vector(size_type n) : Base(n, allocator_type())
    { this->M_finish = uninitialized_fill_n(this->M_start, n, Tp()); }

    vector(const vector<Tp, Alloc>& x) : Base(x.size(), x.get_allocator())
    { this->M_finish = std::uninitialized_copy(x.begin(), x.end(), this->M_start); }

    vector(const Tp* first, const Tp* last, const typename Base::allocator_type& a = allocator_type())
            : Base(last - first, a)
    { this->M_finish = std::uninitialized_copy(first, last, this->M_start); }

    ~vector() { std::destroy(this->M_start, this->M_finish); }

    vector<Tp, Alloc>& operator=(const vector<Tp, Alloc>& x);

    void reserve(size_type n)
    {
        if(capacity() < n)
        {
            const size_type old_size = size();
            iterator tmp = M_allocate_and_copy(n, this->M_start, this->M_finish);
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
        if(this->M_finish != this->M_end_of_storage)
        {
            construct(this->M_finish, x);
            ++this->M_finish;
        }
        else
            M_insert_aux(end(),x);
    }
    void push_back()
    {
        if(this->M_finish != this->M_end_of_storage)
        {
            construct(this->M_finish);
            ++this->M_finish;
        }
        else
            M_insert_aux(end());
    }

    void swap(vector<Tp, Alloc>& x)
    {
        std::swap(this->M_start, x.M_start);
        std::swap(this->M_finish, x.M_finish);
        std::swap(this->M_end_of_storage, x.M_end_of_storage);

    }

    iterator insert(iterator position, const Tp& x)
    {
        size_type n = position - begin();
        if(this->M_finish != this->M_end_of_stoage && position == end())
        {
            construct(this->M_finish, x);
            ++this->M_finish;
        }
        else
            M_insert_aux(position, x);
        return begin() + n;
    }

    iterator insert(iterator position)
    {
        size_type n = position - begin();
        if(this->M_finish != this->M_end_of_stoage && position == end())
        {
            construct(this->M_finish);
            ++this->M_finish;
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
        --this->M_finish;
        std::destroy(this->M_finish);
    }
    iterator erase(iterator position)
    {
        if(position + 1 != end())
            std::copy(position+1, this->M_finish, position);
        --this->M_finish;
        std::destroy(this->M_finish);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, this->M_finish, first);
        std::destroy(i ,this->M_finish);
        this->M_finish = this->M_finish - (last - first);
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
        iterator result = this->M_allocate(n);
        try
        {
            std::uninitialized_copy(first, last, result);
            return result;
        }
        catch (...)
        {
            this->M_deallocate(result, n);
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
            std::destroy(this->M_start, this->M_finish);
            this->M_deallocate(this->M_start, this->M_end_of_stoage - this->M_start);
            this->M_start = tmp;
            this->M_end_of_storate = tmp + xlen;
        }
        else if (size() >= xlen)
        {
            iterator i = std::copy(x.begin(), x.end(), begin());
            std::destroy(i, this->M_finish);
        }
        else
        {
            std::copy(x.begin(), x.begin() + size(), this->M_start);
            std::uninitialized_copy(x.begin() + size(), x.end(), this->M_finish);
        }
        this->M_finish = this->M_start + xlen;
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
        this->M_finish = uninitialized_fill_n(this->M_finish, n - size(), val);
    }
    else
        erase(std::fill_n(begin(), n, val), end());
}



template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_insert_aux(iterator position, const Tp& x)
{
    if(this->M_finish != this->M_end_of_storage)
    {
        construct(this->M_finish, *(this->M_finish - 1));
        ++this->M_finish;
        Tp x_copy = x;
        std::copy_backward(position, this->M_finish - 2, this->M_finish - 1);
        *position = x_copy;
    }
    else
    {
        const size_type old_size = size();
        const size_type len = (old_size != 0) ? 2 * old_size : 1;
        iterator new_start = this->M_allocate(len);
        iterator new_finish = new_start;
        try
        {
            new_finish = std::uninitialized_copy(this->M_start, position, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = std::uninitialized_copy(position, this->M_finish, new_finish);
        }
        catch (...)
        {
            std::destroy(new_start, new_finish), this->M_deallocate(new_start, len);
            throw;
        }
        //TODO
        std::destroy(begin(), end());
        this->M_start = new_start;
        this->M_finish = new_finish;
        this->M_end_of_storage = new_start + len;
    }
}



template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_insert_aux(iterator position)
{
    if(this->M_finish != this->M_end_of_storage)
    {
        construct(this->M_finish, *(this->M_finish - 1));
        ++this->M_finish;
        std::copy_backward(position, this->M_finish - 2, this->M_finish - 1);
        *position = Tp();
    }
    else
    {
        const size_type old_size = size();
        const size_type len = (old_size != 0) ? 2 * old_size : 1;
        iterator new_start = this->M_allocate(len);
        iterator new_finish = new_start;
        try
        {
            new_finish = std::uninitialized_copy(this->M_start, position, new_start);
            construct(new_finish);
            ++new_finish;
            new_finish = std::uninitialized_copy(position, this->M_finish, new_finish);
        }
        catch (...)
        {
            std::destroy(new_start, new_finish), this->M_deallocate(new_start, len);
            throw;
        }
        std::destroy(begin(), end());
        this->M_start = new_start;
        this->M_finish = new_finish;
        this->M_end_of_storage = new_start + len;
    }
}

template <class Tp, class Alloc>
void vector<Tp, Alloc>::M_fill_insert(iterator position, size_type n, const Tp &x)
{
    if(n != 0)
    {
        if(size_type(this->M_end_of_storge - this->M_finish) >= n)
        {
            Tp x_copy = x;
            const size_type elems_after = this->M_finish - position;
            iterator old_finish = this->M_finish;
            if(elems_after > n)
            {
                std::uninitialized_copy(this->M_finish - n, this->M_finish, this->M_finish);
                this->M_finish = this->M_finish + n;
                std::copy_backward(position, old_finish - n, old_finish);
                std::fill(position, position + n, x_copy);
            }
            else
            {
                uninitialized_fill_n(this->M_finish, n - elems_after, x_copy);
                this->M_finish += n - elems_after;
                std::uninitialized_copy(position, old_finish, this->M_finish);
                this->M_finish += elems_after;
                std::fill(position, old_finish, x_copy);
            }
        }
        else
        {
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, n);
            iterator new_start = this->M_allocate(len);
            iterator new_finish = new_start;
            try {
                    new_finish = std::uninitialized_copy(this->M_start, position, new_start);
                    new_finish = uninitialized_fill_n(new_finish, n, x);
                    new_finish
                    = std::uninitialized_copy(position, this->M_finish, new_finish);
            }
            catch (...)
            {
                std::destroy(new_start,new_finish);
                this->M_deallocate(new_start,len);
                throw;
            }
            std::destroy(this->M_start, this->M_finish);
            this->M_deallocate(this->M_start, this->M_end_of_storage - this->M_start);
            this->M_start = new_start;
            this->M_finish = new_finish;
            this->M_end_of_storage = new_start + len;
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
        if( size_type(this->M_end_of_storage - this->M_finish) >= n)
        {
            const size_type elems_after = this->M_finish - position;
            iterator old_finish = this->M_finish;
            if(elems_after > n)
            {
                std::uninitialized_copy(this->M_finish - n, this->M_finish, this->M_finish);
                this->M_finish += n;
                std::copy_backward(position, old_finish - n, old_finish);
                std::copy(first, last, position);
            }
            else
            {
                std::uninitialized_copy(first + elems_after, last, this->M_finish);
                this->M_finish += n - elems_after;
                std::uninitialized_copy(position, old_finish, this->M_finish);
                this->M_finish += elems_after;
                std::copy(first, first + elems_after, position);
            }

        }
        else
        {
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, n);
            iterator new_start = this->M_allocate(len);
            iterator new_finish = new_start;
            try {
                    new_finish = std::uninitialized_copy(this->M_start, position, new_start);
                    new_finish = std::uninitialized_copy(first, last, new_finish);
                    new_finish
                    = std::uninitialized_copy(position, this->M_finish, new_finish);
            }
            catch(...)
            {
                std::destroy(new_start,new_finish);
                this->M_deallocate(new_start,len);
            }
            std::destroy(this->M_start, this->M_finish);
            this->M_deallocate(this->M_start, this->M_end_of_storage - this->M_start);
            this->M_start = new_start;
            this->M_finish = new_finish;
            this->M_end_of_storage = new_start + len;
        }

    }
}




} // namespace MySTL











#endif //MYSTL_STL_VECTOR_H
