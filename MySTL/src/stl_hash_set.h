#ifndef MYSTL_STL_HASH_SET_H
#define MYSTL_STL_HASH_SET_H
#include "stl_alloc.h"
#include "stl_hashtable.h"


namespace MySTL
{

    //TODO 这里的宏定义没搞明白
template <class Value, class HashFcn, class EqualKey,
          class Alloc = allocator<Value>>
class hash_set;


template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1,
                       const hash_set<Value, HashFcn, EqualKey, Alloc>& hs2);

template <class Value, class HashFcn, class EqualKey, class Alloc>
class hash_set
{
private:
    typedef hashtable<Value, Value, HashFcn, EqualKey, Alloc> Ht;
    Ht M_ht;

public:
    typedef typename Ht::key_type key_type;
    typedef typename Ht::value_type value_type;
    typedef typename Ht::hasher hasher;
    typedef typename Ht::key_equal key_equal;

    typedef typename Ht::size_type size_type;
    typedef typename Ht::difference_type difference_type;
    typedef typename Ht::const_pointer pointer;
    typedef typename Ht::const_pointer const_pointer;
    typedef typename Ht::const_reference reference;
    typedef typename Ht::const_reference const_reference;

    typedef typename Ht::const_iterator iterator;
    typedef typename Ht::const_iterator const_iterator;

    typedef typename Ht::allocator_type allocator_type;

public:
    hash_set() : M_ht(100, hasher(), key_equal(), allocator_type()) {}
    explicit hash_set(size_type n) : M_ht(n, hasher(), key_equal(), allocator_type()) {}
    hash_set(size_type n, const hasher& hf) : M_ht(n, hf, key_equal(), allocator_type()) {}
    hash_set(size_type n, const hasher& hf, const key_equal& eql,
             const allocator_type& a = allocator_type())
             : M_ht(n, hf, eql, a) {}

    template<class InputIterator>
    hash_set(InputIterator f, InputIterator l)
    :M_ht(100, hasher(), key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template<class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n)
    : M_ht(n, hasher(), key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template<class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n, const hasher& hf)
    : M_ht(n, hf, key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n, const hasher& hf,
    const key_equal& eql, const allocator_type& a = allocator_type())
    : M_ht(n, hf, eql, a)
    { M_ht.insert_unique(f, l); }


public:
    size_type size() const { return M_ht.size(); }
    size_type max_size() const { return M_ht.max_size(); }
    bool empty() const { return M_ht.empty(); }
    void swap(hash_set& hs) { M_ht.swap(hs.M_ht); }

    template<class Val, class HF, class EqK, class Al>
            friend bool operator==(const hash_set<Val, HF, EqK, Al>&,
                                   const hash_set<Val, HF, EqK, Al>&);

    iterator begin() const { return M_ht.begin(); }
    iterator end() const { return M_ht.end(); }

public:
    pair<iterator, bool> insert(const value_type& obj)
    {
        pair<typename Ht::iterator, bool> p = M_ht.insert_unique(obj);
        return pair<iterator, bool>(p.first, p.second);
    }

    template<class InputIterator>
    void insert(InputIterator f, InputIterator l)
    { M_ht.insert_unique(f, l); }

    pair<iterator, bool> insert_noresize(const value_type& obj)
    {
        pair<typename Ht::iterator, bool> p = M_ht.insert_unique_noresize(obj);
        return pair<iterator, bool>(p.first, p.second);
    }

    iterator find(const key_type& key) const { return M_ht.find(key); }

    size_type count(const key_type& key) const { return M_ht.count(key); }

    pair<iterator, iterator> equal_range(const key_type& key) const
    { return M_ht.equal_range(key); }

    size_type erase(const key_type& key) { return M_ht.erase(key); }
    void erase(iterator it) { M_ht.erase(it); }
    void erase(iterator f, iterator l) { M_ht.erase(f, l); }
    void clear() { M_ht.clear(); }

public:
    void resize(size_type hint) { M_ht.resize(hint); }
    size_type bucket_count() const { return M_ht.bucket_count(); }
    size_type max_bucket_count() const { return M_ht.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    { return M_ht.elems_in_bucket(n); }

};

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1,
                       const hash_set<Value, HashFcn, EqualKey, Alloc>& hs2)
{
    return hs1.M_ht == hs2.M_ht;
}


template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1,
                       const hash_set<Value, HashFcn, EqualKey, Alloc>& hs2)
{
    return !(hs1.M_ht == hs2.M_ht);
}

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(hash_set<Value, HashFcn, EqualKey, Alloc>& hs1,
                 hash_set<Value, HashFcn, EqualKey, Alloc>& hs2)
{ hs1.swap(hs2); }







template <class Value, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<hash_set<Value, HashFcn, EqualKey, Alloc> >
{
protected:
    typedef hash_set<Value, HashFcn, EqualKey, Alloc> Container;
    Container* container;

public:
    typedef Container container_type;
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    insert_iterator(Container& x) : container(&x) {}
    insert_iterator(Container& x, typename Container::iterator) : container(&x) {}
    insert_iterator<Container>& () operator=(const typename Container::value_type& value)
    {
        container->insert(value);
        return *this;
    }
    insert_iterator<Container >& operator*() { return *this; }
    insert_iterator<Container >& operator++() { return *this; }
    insert_iterator<Container >& operator++(int) { return *this; }

};

























}



















#endif //MYSTL_STL_HASH_SET_H
