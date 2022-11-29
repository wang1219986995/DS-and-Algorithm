#ifndef MYSTL_STL_MULTIMAP_H
#define MYSTL_STL_MULTIMAP_H

#include "stl_pair.h"
#include "stl_alloc.h"
#include "stl_rb_tree.h"
#include <functional>

namespace MySTL
{
template <class Key, class Tp, class Compare = std::less<Key>,
        class Alloc = allocator<Tp>>
class multimap;


template <class Key, class Tp, class Compare, class Alloc>
inline bool operator==(const multimap<Key, Tp, Compare, Alloc>& x,
                       const multimap<Key, Tp, Compare, Alloc>& y);


template <class Key, class Tp, class Compare, class Alloc>
inline bool operator<(const multimap<Key, Tp, Compare, Alloc>& x,
                      const multimap<Key, Tp, Compare, Alloc>& y);

template <class Key, class Tp, class Compare, class Alloc>
class multimap
{
public:
    typedef Key key_type;
    typedef Tp data_type;
    typedef Tp mapped_type;
    typedef pair<const Key, Tp> value_type;
    typedef Compare key_compare;

class value_compare : public std::binary_function<value_type, value_type, bool>
{
friend class multimap<Key, Tp, Compare, Alloc>;
protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}
public:
    bool operator()(const value_type& x, const value_type& y) const
    { return comp(x.first, y.first); }
};

private:
    typedef Rb_tree<key_type, value_type, std::_Select1st<value_type>,
            key_compare, Alloc> Rep_type;
    Rep_type M_t;

public:
    typedef typename Rep_type::pointer pointer;
    typedef typename Rep_type::const_pointer const_pointer;
    typedef typename Rep_type::reference reference;
    typedef typename Rep_type::const_reference const_reference;
    typedef typename Rep_type::iterator iterator;
    typedef typename Rep_type::const_iterator const_iterator;
    typedef typename Rep_type::reverse_iterator reverse_iterator;
    typedef typename Rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename Rep_type::size_type size_type;
    typedef typename Rep_type::difference_type difference_type;
    typedef typename Rep_type::allocator_type allocator_type;

    multimap() : M_t(Compare(), allocator_type()) {}
    explicit multimap(const Compare& comp, const allocator_type& a = allocator_type())
    : M_t(comp, a) {}

    template<class InputIterator>
    multimap(InputIterator first, InputIterator last)
    :M_t(Compare(), allocator_type())
    { M_t.insert_equal(first, last); }

    template<class InputIterator>
    multimap(InputIterator first, InputIterator last,
    const Compare& comp, const allocator_type& a = allocator_type())
    :M_t(comp, a) { M_t.insert_equal(first, last); }

    multimap(const multimap<Key, Tp, Compare, Alloc>& x) : M_t(x.M_t) {}

    multimap<Key, Tp, Compare, Alloc>&
    operator=(const multimap<Key, Tp, Compare, Alloc>& x)
    {
        M_t = x.M_t;
        return *this;
    }

    // accessors:
    key_compare key_comp() const { return M_t.key_comp(); }
    value_compare value_comp() const { return value_compare(M_t.key_comp()); }
    allocator_type getAllocator() const { return M_t.getAllocator(); }

    iterator begin() { return M_t.begin(); }
    const_iterator begin() const { return M_t.begin(); }
    iterator end() { return M_t.end(); }
    const_iterator end() const { return M_t.end(); }
    reverse_iterator rbegin() { return M_t.rbegin(); }
    const_reverse_iterator rbegin() const { return M_t.rbegin(); }
    reverse_iterator rend() { return M_t.rend(); }
    const_reverse_iterator rend() const { return M_t.rend(); }
    bool empty() const { return M_t.empty(); }
    size_type size() const { return M_t.size(); }
    size_type max_size() const { return M_t.max_size(); }
    void swap(multimap<Key,Tp,Compare,Alloc>& x) { M_t.swap(x.M_t); }

    // insert/erase
    iterator insert(const value_type& x) { return M_t.insert_equal(x); }
    iterator insert(iterator position, const value_type& x)
    { return M_t.insert_equal(position, x); }

    template <class _InputIterator>
    void insert(_InputIterator first, _InputIterator last)
    { M_t.insert_equal(first, last); }

    void erase(iterator position) { M_t.erase(position); }
    size_type erase(const key_type& x) { return M_t.erase(x); }
    void erase(iterator first, iterator last)
    { M_t.erase(first, last); }
    void clear() { M_t.clear(); }

    // multimap operations:
    iterator find(const key_type& x) { return M_t.find(x); }
    const_iterator find(const key_type& x) const { return M_t.find(x); }
    size_type count(const key_type& x) const { return M_t.count(x); }
    iterator lower_bound(const key_type& x) {return M_t.lower_bound(x); }
    const_iterator lower_bound(const key_type& x) const 
    { return M_t.lower_bound(x); }
    iterator upper_bound(const key_type& x) {return M_t.upper_bound(x); }
    const_iterator upper_bound(const key_type& x) const 
    { return M_t.upper_bound(x); }
    pair<iterator,iterator> equal_range(const key_type& x) 
    { return M_t.equal_range(x); }
    pair<const_iterator,const_iterator> equal_range(const key_type& x) const 
    { return M_t.equal_range(x); }

    template <class K1, class T1, class C1, class A1>
    friend bool operator==(const multimap<K1, T1, C1, A1>&,
                            const multimap<K1, T1, C1, A1>&);
    template <class K1, class T1, class C1, class A1>
    friend bool operator<(const multimap<K1, T1, C1, A1>&,
                           const multimap<K1, T1, C1, A1>&);
};


template <class Key, class Tp, class Compare, class Alloc>
inline bool operator==(const multimap<Key,Tp,Compare,Alloc>& x,
                       const multimap<Key,Tp,Compare,Alloc>& y)
{ return x.M_t == y.M_t; }

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator<(const multimap<Key,Tp,Compare,Alloc>& x,
                      const multimap<Key,Tp,Compare,Alloc>& y)
{ return x.M_t < y.M_t; }


template <class Key, class Tp, class Compare, class Alloc>
inline bool operator!=(const multimap<Key,Tp,Compare,Alloc>& x, 
                       const multimap<Key,Tp,Compare,Alloc>& y)
{ return !(x == y); }

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator>(const multimap<Key,Tp,Compare,Alloc>& x, 
                      const multimap<Key,Tp,Compare,Alloc>& y)
{ return y < x; }

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator<=(const multimap<Key,Tp,Compare,Alloc>& x, 
                       const multimap<Key,Tp,Compare,Alloc>& y)
{ return !(y < x); }

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator>=(const multimap<Key,Tp,Compare,Alloc>& x, 
                       const multimap<Key,Tp,Compare,Alloc>& y)
{ return !(x < y); }

template <class Key, class Tp, class Compare, class Alloc>
inline void swap(multimap<Key,Tp,Compare,Alloc>& x, 
                 multimap<Key,Tp,Compare,Alloc>& y)
{ x.swap(y); }


}



#endif //MYSTL_STL_MULTIMAP_H
