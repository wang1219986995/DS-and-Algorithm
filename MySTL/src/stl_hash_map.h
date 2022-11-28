#ifndef MYSTL_STL_HASH_MAP_H
#define MYSTL_STL_HASH_MAP_H
#include "stl_hashtable.h"
#include "stl_pair.h"
#include <functional>
namespace MySTL
{
    // TODO
template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class hash_map;

template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_map<Key, Tp, HashFcn, EqualKey, Alloc>&,
                       const hash_map<Key, Tp, HashFcn, EqualKey, Alloc>&);



template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class hash_map
{
private:
    //TODO
    typedef hashtable<pair<const Key, Tp>, Key, HashFcn, std::_Select1st<pair<const Key, Tp>>, EqualKey, Alloc> Ht;
    Ht M_ht;

public:
    typedef typename Ht::key_type key_type;
    typedef Tp data_type;
    typedef Tp mapped_type;
    typedef typename Ht::value_type value_type;
    typedef typename Ht::hasher hasher;
    typedef typename Ht::key_equal key_equal;

    typedef typename Ht::size_type size_type;
    typedef typename Ht::difference_type difference_type;
    typedef typename Ht::pointer pointer;
    typedef typename Ht::const_pointer const_pointer;
    typedef typename Ht::reference reference;
    typedef typename Ht::const_reference const_reference;
    typedef typename Ht::iterator iterator;
    typedef typename Ht::const_iterator const_iterator;
    typedef typename Ht::allocator_type allocator_type;

    hasher hash_funct() const { return M_ht.hash_funct(); }
    key_equal key_eq() const { return M_ht.key_eq(); }
    allocator_type get_allocator() const { return M_ht.get_allocator(); }


public:
    hash_map():M_ht(100, hasher(), key_equal(), allocator_type()) {}
    explicit hash_map(size_type n):M_ht(n, hasher(), key_equal(), allocator_type()) {}
    hash_map(size_type n, const hasher& hf):M_ht(n, hf, key_equal(), allocator_type()) {}
    hash_map(size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
    :M_ht(n, hf, eql, a) {}

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l)
    :M_ht(100, hasher(), key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template<class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n)
    :M_ht(n, hasher(), key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf)
    :M_ht(n, hf, key_equal(), allocator_type())
    { M_ht.insert_unique(f, l); }

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf, const key_equal& eql,
             const allocator_type& a = allocator_type()):M_ht(n, hf, eql, a)
    { M_ht.insert_unique(f, l); }


public:
    size_type size() const { return M_ht.size(); }
    size_type max_size() const { return M_ht.max_size(); }
    bool empty() const { return M_ht.empty(); }
    void swap(hash_map& hs) { M_ht.swap(hs.M_ht); }


    template<class K1, class T1, class HF, class EqK, class Al>
    friend bool operator==(const hash_map<K1, T1, HF, EqK, Al>&,
                           const hash_map<K1, T1, HF, EqK, Al>&);

    iterator begin() { return M_ht.begin(); }
    iterator end() { return M_ht.end(); }
    const_iterator begin() const { return M_ht.begin(); }
    const_iterator end() const { return M_ht.end(); }

public:
    pair<iterator, bool> insert(const value_type& obj)
    { return M_ht.insert_unique(obj); }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l)
    { M_ht.insert_unique(f, l); }

    pair<iterator, bool> insert_noresize(const value_type& obj)
    { return M_ht.insert_unique_noresize(obj); }

    iterator find(const key_type& key) { return M_ht.find(key); }
    const_iterator find(const key_type& key) const { return M_ht.find(key); }

    Tp& operator[](const key_type& key)
    { return M_ht.find_or_insert(value_type(key, Tp())).second; }

    size_type count(const key_type& key) const { return M_ht.count(key); }

    pair<iterator, iterator> equal_range(const key_type& key)
    { return M_ht.equal_range(key); }

    pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const
    { return M_ht.equal_range(key); }

    size_type erase(const key_type& key) { return M_ht.erase(key); }
    void erase(iterator it) { M_ht.erase(it); }
    void erase(iterator f, iterator l) { return M_ht.erase(f, l); }
    void clear() { M_ht.clear(); }

    void resize(size_type hint) { M_ht.resize(hint); }
    size_type bucket_count() const { return M_ht.bucket_count(); }
    size_type max_bucket_count() const { return M_ht.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    { return M_ht.elems_in_bucket(n); }

};

template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm1,
                       hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm2)
{ return hm1.M_ht == hm2.M_ht; }

template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm1,
                       const hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm2)
{ return !(hm1 == hm2); }

template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
inline void swap(hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm1,
                 hash_map<Key, Tp, HashFcn, EqualKey, Alloc>& hm2)
{ hm1.swap(hm2); }


//TODO
template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class hash_multimap;

template <class Key, class Tp, class HF, class EqKey, class Alloc>
inline bool operator==(const hash_multimap<Key, Tp, HF, EqKey, Alloc>& hm1,
                       const hash_multimap<Key, Tp, HF, EqKey, Alloc>& hm2);


template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class hash_multimap
{
private:
    typedef hashtable<pair<const Key, Tp>, Key, HashFcn, std::_Select1st<pair<const Key, Tp>>, EqualKey, Alloc> Ht;
    Ht M_ht;

public:
    typedef typename Ht::key_type key_type;
    typedef Tp data_type;
    typedef Tp mapped_type;
    typedef typename Ht::value_type value_type;
    typedef typename Ht::hasher hasher;
    typedef typename Ht::key_equal key_equal;

    typedef typename Ht::size_type size_type;
    typedef typename Ht::difference_type difference_type;
    typedef typename Ht::pointer pointer;
    typedef typename Ht::const_pointer const_pointer;
    typedef typename Ht::reference reference;
    typedef typename Ht::const_reference const_reference;
    typedef typename Ht::iterator iterator;
    typedef typename Ht::const_iterator const_iterator;
    typedef typename Ht::allocator_type allocator_type;

    hasher hash_funct() const { return M_ht.hash_funct(); }
    key_equal key_eq() const { return M_ht.key_eq(); }
    allocator_type get_allocator() const { return M_ht.get_allocator(); }

public:
    hash_multimap():M_ht(100, hasher(), key_equal(), allocator_type()) {}
    explicit hash_multimap(size_type n):M_ht(n, hasher(), key_equal(), allocator_type()) {}
    hash_multimap(size_type n, const hasher& hf) : M_ht(n, hf, key_equal(), allocator_type()) {}
    hash_multimap(size_type n, const hasher& hf, const key_equal& eql,
                  const allocator_type& a = allocator_type()) : M_ht(n, hf, eql, a) {}

    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l)
    :M_ht(100, hasher(), key_equal(), allocator_type()) { M_ht.insert_equal(f, l); }

    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n)
    :M_ht(n, hasher(), key_equal(), allocator_type())
    { M_ht.insert_equal(f, l); }

    template<class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n, const hasher& hf)
    :M_ht(n, hf, key_equal(), allocator_type())
    { M_ht.insert_equal(f, l); }

    template<class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n, const hasher& hf,const key_equal& eql,
                  const allocator_type& a = allocator_type()) : M_ht(n, hf, eql, a)
    { M_ht.insert_equal(f, l); }


public:
    size_type size() const { return M_ht.size(); }
    size_type max_size() const { return M_ht.max_size(); }
    bool empty() const { return M_ht.empty(); }
    void swap(hash_multimap& hs) { M_ht.swap(hs.M_ht); }

    template<class K1, class T1, class HF, class EqK, class Al>
    friend bool operator==(const hash_multimap<K1, T1, HF, EqK, Al>&,
                           const hash_multimap<K1, T1, HF, EqK, Al>&);

    iterator begin() { return M_ht.begin(); }
    iterator end() { return M_ht.end(); }
    const_iterator begin() const { return M_ht.begin(); }
    const_iterator end() const { return M_ht.end(); }

public:
    iterator insert(const value_type& obj)
    { return M_ht.insert_equal(obj); }

    template<class InputIterator>
    void insert(InputIterator f, InputIterator l)
    { M_ht.insert_equal(f, l); }

    iterator insert_noresize(const value_type& obj)
    { return M_ht.insert_equal_noresize(obj); }

    iterator find(const key_type& key) { return M_ht.find(key); }
    const_iterator find(const key_type& key) const
    { return M_ht.find(key); }

    size_type count(const key_type& key) const { return M_ht.count(key); }

    pair<iterator, iterator> equal_range(const key_type& key)
    { return M_ht.equal_range(key); }

    pair<const_iterator, const_iterator> equal_range(const key_type& key) const
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


template <class Key, class Tp, class HF, class EqKey, class Alloc>
inline bool
operator==(const hash_multimap<Key,Tp,HF,EqKey,Alloc>& hm1,
           const hash_multimap<Key,Tp,HF,EqKey,Alloc>& hm2)
{ return hm1._M_ht == hm2._M_ht; }


template <class Key, class Tp, class HF, class EqKey, class Alloc>
inline bool
operator!=(const hash_multimap<Key,Tp,HF,EqKey,Alloc>& hm1,
           const hash_multimap<Key,Tp,HF,EqKey,Alloc>& hm2)
{ return !(hm1 == hm2); }

template <class Key, class Tp, class HashFcn, class EqlKey, class Alloc>
inline void
swap(hash_multimap<Key,Tp,HashFcn,EqlKey,Alloc>& hm1,
     hash_multimap<Key,Tp,HashFcn,EqlKey,Alloc>& hm2)
{ hm1.swap(hm2); }





template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<hash_map<Key, Tp, HashFcn, EqualKey, Alloc>>
{
protected:
    typedef hash_map<Key, Tp, HashFcn, EqualKey, Alloc> Container;
    Container* container;

public:
    typedef Container container_type;
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;


    insert_iterator(Container& x):container(&x) {}
    insert_iterator(Container& x, typename Container::iterator):container(&x) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->insert(value);
        return *this;
    }
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};

template <class Key, class Tp, class HashFcn, class EqualKey, class Alloc>
class insert_iterator<hash_multimap<Key, Tp, HashFcn, EqualKey, Alloc>>
{
protected:
    typedef hash_multimap<Key, Tp, HashFcn, EqualKey, Alloc> Container;
    Container* container;

public:
    typedef Container               container_type;
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;

    insert_iterator(Container& x):container(&x) {}
    insert_iterator(Container& x, typename Container::iterator):container(&x) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->insert(value);
        return *this;
    }
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};














} // namespace




#endif //MYSTL_STL_HASH_MAP_H
