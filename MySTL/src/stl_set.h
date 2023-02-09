
#ifndef MYSTL_STL_SET_H
#define MYSTL_STL_SET_H
#include "stl_pair.h"
#include "stl_alloc.h"
#include "stl_rb_tree.h"
#include <functional>


namespace MySTL
{
template <class Key, class Compare = std::less<Key>,
        class Alloc = allocator<Key>>
class set;


template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y);

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y);


template <class Key, class Compare, class Alloc>
class set
{
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef Rb_tree<key_type, value_type,
    std::_Identity<value_type>, key_compare, Alloc> Rep_type;
    Rep_type M_t;

public:
    typedef typename Rep_type::const_pointer pointer;
    typedef typename Rep_type::const_pointer const_pointer;
    typedef typename Rep_type::const_reference reference;
    typedef typename Rep_type::const_reference const_reference;
    typedef typename Rep_type::const_iterator iterator;
    typedef typename Rep_type::const_iterator const_iterator;
    typedef typename Rep_type::const_reverse_iterator reverse_iterator;
    typedef typename Rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename Rep_type::size_type size_type;
    typedef typename Rep_type::difference_type difference_type;
    typedef typename Rep_type::allocator_type allocator_type;


    set() : M_t(Compare(), allocator_type()) {}
    explicit set(const Compare& comp, const allocator_type& a = allocator_type())
    : M_t(comp, a) {}

    template <class InputIterator>
    set(InputIterator first, InputIterator last)
    :M_t(Compare(), allocator_type())
    { M_t.insert_unique(first, last); }

    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp,
    const allocator_type& a = allocator_type()) : M_t(comp, a)
    { M_t.insert_unique(first, last); }

    set(const set<Key, Compare, Alloc>& x) : M_t(x.M_t) {}
    set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x)
    {
        M_t = x.M_t;
        return *this;
    }


    key_compare key_comp() const { return M_t.key_comp(); }
    value_compare value_comp() const { return M_t.key_comp(); }
    allocator_type get_allocator() const { return M_t.get_allocator(); }

    iterator begin() const { return M_t.begin(); }
    iterator end() const { return M_t.end(); }
    reverse_iterator rbegin() const { return M_t.rbegin(); }
    reverse_iterator rend() const { return M_t.rend(); }
    bool empty() const { return M_t.empry(); }
    size_type size() const { return M_t.size(); }
    size_type max_size() const { return M_t.max_size(); }
    void swap(set<Key, Compare, Alloc>& x) { M_t.swap(x.M_t); }


    // insert / erase
    pair<iterator, bool> insert(const value_type& x)
    {
        pair<typename Rep_type::iterator, bool> p = M_t.insert_unique(x);
        return pair<iterator, bool>(p.first, p.second);
    }

    iterator insert(iterator position, const value_type& x)
    {
        typedef typename Rep_type::iterator Rep_iterator;
        return M_t.insert_unique((Rep_iterator&)position, x);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    { M_t.insert_unique(first, last); }

    void erase(iterator position)
    {
        typedef typename Rep_type::iterator Rep_iterator;
        M_t.erase((Rep_iterator&) position);
    }

    size_type erase(const key_type& x)
    { return M_t.erase(x); }

    void erase(iterator first, iterator last)
    {
        typedef typename Rep_type::iterator Rep_iterator;
        M_t.erase((Rep_iterator&)first, (Rep_iterator&)last);
    }

    void clear() { M_t.clear(); }



    iterator find(const key_type& x) const { return M_t.find(x); }
    size_type count(const key_type& x) const
    { return M_t.find(x) == M_t.end() ? 0 : 1; }

    iterator lower_bound(const key_type& x) const
    { return M_t.lower_bound(x); }

    iterator upper_bound(const key_type& x) const
    { return M_t.upper_bound(x); }

    pair<iterator, iterator> equal_range(const key_type& x) const
    { return M_t.equal_range(x); }

    template <class K1, class C1, class A1>
    friend bool operator==(const set<K1, C1, A1>&, const set<K1,C1, A1>&);

    template <class K1, class C1, class A1>
    friend bool operator<(const set<K1, C1, A1>&, const set<K1,C1, A1>&);


};


template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y)
{ return x.M_t == y.M_t; }

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y)
{ return x.M_t < y.M_t; }

template <class Key, class Compare, class Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y)
{ return !(x == y); }

template <class Key, class Compare, class Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y)
{ return y < x; }

template <class Key, class Compare, class Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y)
{ return !(y < x); }

template <class Key, class Compare, class Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y)
{ return !(x < y); }


template <class Key, class Compare, class Alloc>
inline void swap(set<Key, Compare, Alloc>& x,
                 set<Key, Compare, Alloc>& y)
{ return x.swap(y); }


} //namespace





















#endif //MYSTL_STL_SET_H
