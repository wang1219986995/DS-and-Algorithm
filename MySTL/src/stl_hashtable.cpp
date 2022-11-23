
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_algo.h"
#include "stl_vector.h"
namespace MySTL
{

template <class Val>
struct Hashtable_node
{
    Hashtable_node* M_next;
    Val M_val;
};

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class hashtable;

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class Hashtable_iterator;

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class Hashtable_const_iterator;


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey,
        class Alloc>
struct Hashtable_iterator
{
    typedef hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> Hashtable;
    typedef Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
    typedef Hashtable_node<Val> Node;
    typedef forward_iterator_tag iterator_category;
    typedef Val value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Val& reference;
    typedef Val* pointer;

    Node* M_cur;
    Hashtable* M_ht;

    Hashtable_iterator(Node* n, Hashtable* tab) : M_cur(n), M_ht(tab) {}
    Hashtable_iterator() {}

    reference operator*() { return M_cur->M_val; }
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& it) const
    { return M_cur == it.M_cur; }
    bool operator!=(const iterator& it) const
    { return M_cur != it.M_cur; }
};

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey,
        class Alloc>
struct Hashtable_const_iterator
{
    typedef hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> Hashtable;
    typedef Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
    typedef Hashtable_node<Val> Node;
    typedef forward_iterator_tag iterator_category;
    typedef Val value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef const Val& reference;
    typedef const Val* pointer;

    Node* M_cur;
    Hashtable* M_ht;

    Hashtable_const_iterator(const Node* n, const Hashtable* tab) : M_cur(n), M_ht(tab) {}
    Hashtable_const_iterator() {}
    Hashtable_const_iterator(const iterator& it) : M_cur(it.M_cur), M_ht(it.M_ht) {}

    reference operator*() { return M_cur->M_val; }
    const_iterator& operator++();
    const_iterator operator++(int);
    bool operator==(const const_iterator& it) const
    { return M_cur == it.M_cur; }
    bool operator!=(const const_iterator& it) const
    { return M_cur != it.M_cur; }
};


enum { stl_num_primes = 28};

static const unsigned long stl_prime_list[stl_num_primes]=
{
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long stl_next_prime(unsigned long n)
{
    const unsigned long* first = stl_prime_list;
    const unsigned long* last = stl_prime_list + (int)stl_num_primes;
    const unsigned long* pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}



template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable;

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
bool operator==(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht1,
                const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht2);



template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable
{
public:
    typedef Key key_type;
    typedef Val value_type;
    typedef HashFcn hasher;
    typedef EqualKey key_equal;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    hasher hash_funct() const { return M_hash; }
    key_equal key_eq() const { return M_equals; }
private:
    typedef Hashtable_node<Val> Node;

public:
    typedef typename Alloc_traits<Val, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return M_node_allocator; }
private:
    typename Alloc_traits<Node, Alloc>::allocator_type M_node_allocator;
    Node* M_get_node() { return M_node_allocator.allocate(1); }
    void M_put_node(Node* p) { M_node_allocator.deallocate(p, 1); }

private:
    hasher                  M_hash;
    key_equal               M_equals;
    ExtractKey              M_get_key;
    vector<Node*, Alloc>    M_buckets;
    size_type               M_num_elements;


public:
    typedef Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

    friend struct Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
    friend struct Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

public:
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql,
              const ExtractKey& ext, const allocator_type& a = allocator_type())
              : M_node_allocator(a), M_hash(hf), M_equals(eql), M_get_key(ext),
              M_buckets(a), M_num_elements(0)
    {
        M_initialize_buckets(n);
    }

    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql,
              const allocator_type& a = allocator_type())
            : M_node_allocator(a), M_hash(hf), M_equals(eql), M_get_key(ExtractKey()),
              M_buckets(a), M_num_elements(0)
    {
        M_initialize_buckets(n);
    }

    hashtable(const hashtable& ht)
            : M_node_allocator(ht.get_allocator()), M_hash(ht.M_hash),
            M_equals(ht.M_equals), M_get_key(ht.M_get_key()),
            M_buckets(ht.get_allocator()), M_num_elements(0)
    {
        M_copy_from(ht);
    }

    hashtable& operator=(const hashtable& ht)
    {
        if(&ht != this)
        {
            clear();
            M_hash = ht.M_hash;
            M_equals = ht.M_equals;
            M_get_key = ht.M_get_key;
            M_copy_from(ht);
        }
        return *this;
    }

    ~hashtable() { clear(); }

    size_type size() const { return M_num_elements; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return size() == 0; }

    void swap(hashtable& ht)
    {
        MySTL::swap(M_hash, ht.M_hash);
        MySTL::swap(M_equals, ht.M_equals);
        MySTL::swap(M_get_key, ht.M_get_key);
        M_buckets.swap(ht.M_buckets);
        MySTL::swap(M_num_elements, ht.M_num_elements);
    }

    iterator begin()
    {
        for(size_type n = 0; n < M_buckets.size(); ++n)
            if(M_buckets[n])
                return iterator(M_buckets[n], this);
        return end();
    }

    iterator end() { return iterator(0, this); }
    const_iterator begin() const
    {
        for(size_type n = 0; n < M_buckets.size(); ++n)
            if(M_buckets[n])
                return const_iterator(M_buckets[n], this);
        return end();
    }
    const_iterator end() const { return const_iterator(0, this); }

    //TODO  这个地方怎么理解呢？
    template<class Vl, class Ky, class HF, class Ex, class Eq, class Al>
    friend bool operator==(const hashtable<Vl, Ky, HF, Ex, Eq, Al>&,
                           const hashtable<Vl, Ky, HF, Ex, Eq, Al>&);

public:
    size_type bucket_count() const { return M_buckets.size(); }
    size_type max_bucket_count() const
    { return stl_prime_list[(int)stl_num_primes - 1]; }

    size_type elems_in_bucket(size_type bucket) const
    {
        size_type result = 0;
        for(Node* cur = M_buckets[bucket]; cur; cur = cur->M_next)
            result += 1;
        return result;
    }

    pair<iterator, bool> insert_unique(const value_type& obj)
    {
        resize(M_num_elements + 1);
        return insert_unique_noresize(obj);
    }

    iterator insert_equal(const value_type& obj)
    {
        resize(M_num_elements + 1);
        return insert_equal_noresize(obj);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& obj);
    iterator insert_equal_noresize(const value_type& obj);


    template<class InputIterator, InputIterator>
    void insert_unique(InputIterator f, InputIterator l)
    {
        insert_unique(f, l, ITERATOR_CATEGORY(f));
    }

    template<class InputIterator>
    void insert_equal(InputIterator f, InputIterator l)
    {
        insert_equal(f, l, ITERATOR_CATEGORY(f));
    }

    template<class InputIterator>
    void insert_unique(InputIterator f, InputIterator l, input_iterator_tag)
    {
        for(; f != l; ++f)
            insert_unique(*f);
    }

    template<class InputIterator>
    void insert_equal(InputIterator f, InputIterator l, input_iterator_tag)
    {
        for(; f != l; ++f)
            insert_equal(*f);
    }

    template<class ForwardIterator>
    void insert_unique(ForwardIterator f, ForwardIterator l, forward_iterator_tag)
    {
        size_type n = 0;
        distance(f, l, n);
        resize(M_num_elements + n);
        for(; n > 0; --n, ++f)
            insert_unique_noresize(*f);
    }

    template<class ForwardIterator>
    void insert_equal(ForwardIterator f, ForwardIterator l, forward_iterator_tag)
    {
        size_type n = 0;
        distance(f, l, n);
        resize(M_num_elements + n);
        for(; n > 0; --n, ++f)
            insert_equal_noresize(*f);
    }

    reference find_or_insert(const value_type& obj);

    iterator find(const key_type& key)
    {
        size_type n = M_bkt_num_key(key);
        Node* first;
        for(first = M_buckets[n];
        first && !M_equals(M_get_key(first->M_val), key);
        first = first->M_next) {}
        return iterator(first, this);
    }

    const_iterator find(const key_type& key) const
    {
        size_type n = M_bkt_num_key(key);
        Node* first;
        //TODO 这里 M_equals 的作用是什么呢？
        for(first = M_buckets[n];
            first && !M_equals(M_get_key(first->M_val), key);
            first = first->M_next) {}
        return const_iterator(first, this);
    }

    size_type count(const key_type& key) const
    {
        const size_type n = M_bkt_num_key(key);
        size_type result = 0;
        for(const Node* cur = M_buckets[n]; cur; cur = cur->M_next)
            if(M_equals(M_get_key(cur->M_val), key))
                ++ result;
        return result;
    }

    pair<iterator, iterator>
    equal_range(const key_type& key);

    pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const;

    size_type erase(const key_type& key);
    void erase(const iterator& it);
    void erase(iterator first, iterator last);

    void erase(const const_iterator& it);
    void erase(const_iterator first, const_iterator last);

    void resize(size_type num_elements_hint);
    void clear();

private:
    size_type M_next_size(size_type n) const
    { return stl_next_prime(n); }

    void M_initialize_buckets(size_type n)
    {
        const size_type n_buckets = M_next_size(n);
        M_buckets.reserve(n_buckets);
        M_buckets.insert(M_buckets.end(), n_buckets, (Node*) 0);
        M_num_elements = 0;
    }

    size_type M_bkt_num_key(const key_type& key) const
    {
        return M_bkt_num_key(key, M_buckets.size());
    }

    size_type M_bkt_num(const value_type& obj) const
    {
        return M_bkt_num_key(M_get_key(obj));
    }




    size_type M_bkt_num_key(const key_type& key, size_t n) const
    {
        return M_hash(key) % n;
    }

    size_type M_bkt_num(const value_type& obj, size_t n) const
    {
        return M_bkt_num_key(M_get_key(obj), n);
    }


    Node* M_new_node(const value_type& obj)
    {
        Node* n = M_get_node();
        n->M_next = 0;
        try
        {
            construct(&n->M_val, obj);
            return n;
        }
        catch (...)
        {
            M_put_node(n);
            throw;
        }
    }

    void M_delete_node(Node* n)
    {
        destroy(&n->M_val);
        M_put_node(n);
    }

    void M_erase_bucket(const size_type n, Node* first, Node* last);
    void M_erase_bucket(const size_type n, Node* last);

    void M_copy_from(const hashtable& ht);
};


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
    const Node*  old = M_cur;
    M_cur = M_cur->M_next;
    if(!M_cur)
    {
        size_type bucket = M_ht->M_bkt_num(old->M_val);
        while(!M_cur && ++ bucket < M_ht->M_buckets.size())
            M_cur = M_ht->M_buckets[bucket];
    }
    return *this;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
    const Node*  old = M_cur;
    M_cur = M_cur->M_next;
    if(!M_cur)
    {
        size_type bucket = M_ht->M_bkt_num(old->M_val);
        while(!M_cur && ++ bucket < M_ht->M_buckets.size())
            M_cur = M_ht->M_buckets[bucket];
    }
    return *this;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline forward_iterator_tag
iterator_category(const Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return forward_iterator_tag();
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline Val* value_type(const Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return (Val*) 0;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::difference_type*
distance_type(const Hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return (hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::difference_type *) 0;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline forward_iterator_tag
iterator_category(const Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return forward_iterator_tag();
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline Val* value_type(const Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return (Val*) 0;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::difference_type*
distance_type(const Hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc> &)
{
    return (hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::difference_type*) 0;
}




template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
bool operator==(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht1,
                const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht2)
{
    typedef typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::Node Node;
    if(ht1.M_buckets.size() != ht2.M_buckets.size())
        return false;
    for(int n = 0; n < ht1.M_buckets.size(); ++n)
    {
        Node* cur1 = ht1.M_buckets[n];
        Node* cur2 = ht2.M_buckets[n];
        for(; cur1 && cur2 && cur1->M_val == cur2->M_val;
            cur1 = cur1->next, cur2 = cur2->next)
        { }
        if(cur1 || cur2)
            return false;
    }
    return true;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline bool operator!=(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht1,
                       const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht2)
{
    return !(ht1 == ht2);
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void swap(hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht1,
                 hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& ht2)
{
    ht1.swap(ht2);
}

// ----------------------hashtable-------------------
template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
insert_unique_noresize(const value_type &obj)
{
    const size_type n = M_bkt_num(obj);
    Node* first = M_buckets[n];

    for(Node* cur = first; cur; cur = cur->M_next)
        if(M_equals(M_get_key(cur->M_val), M_get_key(obj)))
            return pair<iterator, bool>(iterator(cur, this), false);

    Node* tmp = M_new_node(obj);
    tmp->M_next = first;
    M_buckets[n] = tmp;
    ++M_num_elements;
    return pair<iterator, bool>(iterator(tmp, this), true);
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type &obj)
{
    const size_type n = M_bkt_num(obj);
    Node* first = M_buckets[n];

    for(Node* cur = first; cur; cur = cur->M_next)
        if(M_equals(M_get_key(cur->M_val), M_get_key(obj)))
        {
            Node* tmp = M_new_node(obj);
            tmp->M_next = cur->M_next;
            cur->M_next = tmp;
            ++M_num_elements;
            return iterator(tmp, this);
        }

    Node* tmp = M_new_node(obj);
    tmp->M_next = first;
    M_buckets[n] = tmp;
    ++M_num_elements;
    return iterator(tmp, this);
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find_or_insert(const value_type &obj)
{
    resize(M_num_elements + 1);

    size_type n = M_bkt_num(obj);
    Node* first = M_buckets[n];

    for(Node* cur = first; cur; cur = cur->M_next)
        if(M_equals(M_get_key(cur->M_val), M_get_key(obj)))
            return cur->M_val;

    Node* tmp = M_new_node(obj);
    tmp->M_next = first;
    M_buckets[n] = tmp;
    return tmp->M_val;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair <typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
      typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type &key)
{
    typedef pair<iterator, iterator> Pii;
    const size_type n = M_bkt_num_key(key);

    for(Node* first = M_buckets[n]; first; first = first->M_next)
        if(M_equals(M_get_key(first->M_val), key))
        {
            for(Node* cur = first->M_val; cur; cur = cur->M_next)
                if(!M_equals(M_get_key(cur->M_val), key))
                    return Pii(iterator(first, this), iterator(cur, this));
            for(size_type m = n + 1; m < M_buckets.size(); ++m)
                if(M_buckets[m])
                    return Pii(iterator(first, this), iterator(M_buckets[m], this));
            return Pii(iterator(first, this), end());
        }
    return Pii(end(), end());
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
     typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type &key) const
{
    typedef pair<const_iterator, const_iterator> Pii;
    const size_type n = M_bkt_num_key(key);

    for(const Node* first = M_buckets[n]; first; first = first->M_next)
    {
        if(M_equals(M_get_key(first->M_val), key))
        {
            for(const Node* cur = first->M_next; cur; cur = cur->M_next)
                if(!M_equals(M_get_key(cur->M_val), key))
                    return Pii(const_iterator(first, this), const_iterator(cur, this));

            for(size_type m = n + 1; m < M_buckets.size(); ++m)
                if(M_buckets[m])
                    return Pii(const_iterator(first, this), const_iterator(M_buckets[m], this));
        }
    }
    return Pii(end(), end());
}


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type& key)
{
    const size_type n = M_bkt_num_key(key);
    Node* first = M_buckets[n];
    size_type erased = 0;

    if(first)
    {
        Node* cur = first;
        Node* next = cur->M_next;
        while(next)
        {
            if(M_equals(M_get_key(next->M_val), key))
            {
                cur->M_next = next->M_next;
                M_delete_node(next);
                next = cur->M_next;
                ++erased;
                --M_num_elements;
            }
            else
            {
                cur = next;
                next = cur->M_next;
            }
        }

        if(M_equals(M_get_key(first->M_val), key))
        {
            M_buckets[n] = first->M_next;
            M_delete_node(first);
            ++erased;
            --M_num_elements;
        }
    }
    return erased;
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it)
{
    Node* p = it.M_cur;
    if(p)
    {
        const size_type n = M_bkt_num(p->M_val);
        Node* cur = M_buckets[n];

        if(cur == p)
        {
            M_buckets[n] = cur->M_next;
            M_delete_node(cur);
            --M_num_elements;
        }
        else
        {
            Node* next = cur->M_next;
            while(next)
            {
                if(next == p)
                {
                    cur->M_next = next->M_next;
                    M_delete_node(next);
                    --M_num_elements;
                    break;
                }
                else
                {
                    cur = next;
                    next = cur->M_next;
                }
            }
        }
    }
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator first, iterator last)
{
    size_type f_bucket = first.M_cur ?
                         M_bkt_num(first.M_cur->M_val) : M_buckets.size();
    size_type l_bucket = last.M_cur ?
                         M_bkt_num(last.M_cur->M_val) : M_buckets.size();

    if(first.M_cur == last.M_cur)
        return;
    else if(f_bucket == l_bucket)
        M_erase_bucket(f_bucket, first.M_cur, last.M_cur);
    else
    {
        M_erase_bucket(f_bucket, first.M_cur, 0);
        for(size_type n = f_bucket + 1; n < l_bucket; ++n)
            M_erase_bucket(n, 0);
        if(l_bucket != M_buckets.size())
            M_erase_bucket(l_bucket, last.M_cur);
    }
}


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const_iterator first, const_iterator last)
{
    erase(iterator(const_cast<Node*>(first.M_cur), const_cast<hashtable*>(first.M_ht)),
          iterator(const_cast<Node*>(last.M_cur),const_cast<hashtable*>(last.M_ht)));
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const const_iterator& it)
{
    erase(iterator(const_cast<Node*>(it.M_cur), const_cast<hashtable*>(it.M_ht)));
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
{
    const size_type old_n = M_buckets.size();
    if(num_elements_hint > old_n)
    {
        const size_type n = M_next_size(num_elements_hint);
        if(n > old_n)
        {
            vector<Node*, Alloc> tmp(n, (Node*)(0), M_buckets.get_allocator());
            try
            {
                for(size_type bucket = 0; bucket < old_n; ++bucket)
                {
                    Node* first = M_buckets[bucket];
                    while(first)
                    {
                        size_type new_bucket = M_bkt_num(first->M_val, n);
                        M_buckets[bucket] = first->M_next;
                        first->M_next = tmp[new_bucket];
                        tmp[new_bucket] = first;
                        first = M_buckets[bucket];
                    }
                }
            }
            catch (...)
            {
                for(size_type bucket = 0; bucket < tmp.size(); ++ bucket)
                {
                    while(tmp[bucket])
                    {
                        Node* next = tmp[bucket]->M_next;
                        M_delete_node(tmp[bucket]);
                        tmp[bucket] = next;
                    }
                }
                throw;
            }

        }
    }
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::M_erase_bucket(const size_type n, Node* first, Node* last)
{
    Node* cur = M_buckets[n];
    if(cur == first)
        M_erase_bucket(n, last);
    else
    {
        Node* next;
        for(next = cur->M_next; next != first; cur = next, next = cur->M_next);
        while(next != last)
        {
            cur->M_next = next->M_next;
            M_delete_node(next);
            next = cur->M_next;
            --M_num_elements;
        }
    }
}


template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::M_erase_bucket(const size_type n, Node* last)
{
    Node* cur = M_buckets[n];
    while(cur != last)
    {
        Node* next = cur->M_next;
        M_delete_node(cur);
        cur = next;
        M_buckets[n] = cur;
        --M_num_elements;
    }
}

template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::M_copy_from(const hashtable& ht)
{
    M_buckets.clear();
    M_buckets.reserve(ht.M_buckets.size());
    M_buckets.insert(M_buckets.end(), ht.M_buckets.size(), (Node*) 0);
    try
    {
        for(size_type i = 0; i < ht.M_buckets.size(); ++i)
        {
            const Node* cur = ht.M_buckets[i];
            if(cur)
            {
                Node* copy = M_new_node(cur->M_val);
                M_buckets[i] = copy;

                for(Node* next = cur->M_next; next; cur = next, next = cur->M_next)
                {
                    copy->M_next = M_new_node(next->M_val);
                    copy = copy->M_next;
                }
            }
        }
        M_num_elements = ht.M_num_elements;
    }
    catch (...)
    {
        clear();
        throw;
    }
}





}// namespace
