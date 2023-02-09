#ifndef MYSTL_STL_RB_TREE_H
#define MYSTL_STL_RB_TREE_H
#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_pair.h"
#include "stl_algo.h"
namespace MySTL
{

typedef bool Rb_tree_Color_type;
const Rb_tree_Color_type S_rb_tree_red = false;
const Rb_tree_Color_type S_rb_tree_black = true;

struct Rb_tree_node_base
{
    typedef Rb_tree_Color_type Color_type;
    typedef Rb_tree_node_base* Base_ptr;

    Color_type  M_color;
    Base_ptr M_parent;
    Base_ptr M_left;
    Base_ptr M_right;

    static Base_ptr S_minimum(Base_ptr x)
    {
        while(x->M_left != 0) x = x->M_left;
        return x;
    }

    static Base_ptr S_maximum(Base_ptr x)
    {
        while(x->M_right != 0) x = x->M_right;
        return x;
    }
};

template <class Value>
struct Rb_tree_node : public Rb_tree_node_base
{
    typedef Rb_tree_node<Value>* Link_type;
    Value M_value_field;
};

struct Rb_tree_base_iterator
{
    typedef Rb_tree_node_base::Base_ptr Base_ptr;
    typedef bidirectional_iterator_tag iterator_tag;
    typedef ptrdiff_t difference_type;
    Base_ptr M_node;

    void M_increment()
    {
        if(M_node->M_right != 0)
        {
            M_node = M_node->M_right;
            while(M_node->M_left != 0)
                M_node = M_node->M_left;
        }
        else
        {
            Base_ptr y = M_node->M_parent;
            while(M_node == y->M_right)
            {
                M_node = y;
                y = y->M_parent;
            }
            //TODO 这里没看懂
            if(M_node->M_right != y)
                M_node = y;
        }
    }

    void M_decrement()
    {
        if(M_node->M_color == S_rb_tree_red &&
        M_node->M_parent->M_parent == M_node)
            M_node = M_node->M_right;
        else if(M_node ->M_left != 0)
        {
            Base_ptr y = M_node->M_left;
            while(y->M_right != 0)
                y = y->M_right;
            M_node = y;
        }
        else
        {
            Base_ptr y = M_node->M_parent;
            while(M_node == y->M_left)
            {
                M_node = y;
                y = y->M_parent;
            }
            M_node = y;
        }
    }
};

template <class Value, class Ref, class Ptr>
struct Rb_tree_iterator : public Rb_tree_base_iterator
{
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef Rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef Rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
    typedef Rb_tree_iterator<Value, Ref, Ptr> Self;
    typedef Rb_tree_node<Value>* Link_type;

    Rb_tree_iterator() {}
    Rb_tree_iterator(Link_type x) { M_node = x; }
    Rb_tree_iterator(const iterator& it) { M_node = it.M_node; }

    reference operator*() const { return Link_type(M_node)->M_value_field; }

    Self& operator++() { M_increment(); return *this; }
    Self operator++(int)
    {
        Self tmp = *this;
        M_increment();
        return tmp;
    }

    Self& operator--() { M_decrement(); return *this; }
    Self& operator--(int)
    {
        Self tmp = *this;
        M_decrement();
        return tmp;
    }
};


inline bool operator==(const Rb_tree_base_iterator& x,
                       const Rb_tree_base_iterator& y)
{
    return x.M_node == y.M_node;
}

inline bool operator!=(const Rb_tree_base_iterator& x,
                       const Rb_tree_base_iterator& y)
{
    return x.M_node != y.M_node;
}

inline bidirectional_iterator_tag iterator_category(const Rb_tree_base_iterator&)
{
    return bidirectional_iterator_tag();
}

inline Rb_tree_base_iterator::difference_type* distance_type(const Rb_tree_base_iterator)
{
    return (Rb_tree_base_iterator::difference_type*) 0;
}

template <class Value, class Ref, class Ptr>
inline Value* value_type(const Rb_tree_iterator<Value, Ref, Ptr>&)
{
    return (Value*) 0;
}

inline void Rb_tree_rotate_left(Rb_tree_node_base* x, Rb_tree_node_base*& root)
{
    Rb_tree_node_base* y = x->M_right;
    x->M_right = y->M_left;
    if(y->M_left != 0)
        y->M_left->M_parent = x;
    y->M_parent = x->M_parent;

    if(x == root)
        root = y;
    else if(x == x->M_parent->M_left)
        x->M_parent->M_left = y;
    else
        x->M_parent->M_right = y;
    y->M_left = x;
    x->M_parent = y;

}

inline void Rb_tree_rotate_right(Rb_tree_node_base* x, Rb_tree_node_base*& root)
{
    Rb_tree_node_base* y = x->M_left;
    x->M_left = y->M_right;
    if(y->M_right != 0)
        y->M_right->M_parent = x;
    y->M_parent = x->M_parent;

    if(x == root)
        root = y;
    else if(x == x->M_parent->M_right)
        x->M_parent->M_right = y;
    else
        x->M_parent->M_left = y;

    y->M_left = x;
    x->M_parent = y;
}

inline void Rb_tree_rebalance(Rb_tree_node_base* x, Rb_tree_node_base*& root)
{
    x->M_color = S_rb_tree_red;
    while(x != root && x->M_parent->M_color == S_rb_tree_red)
    {
        if(x->M_parent == x->M_parent->M_parent->M_left)
        {
            Rb_tree_node_base* y = x->M_parent->M_parent->M_right;
            if(y && y->M_color == S_rb_tree_red)
            {
                x->M_parent->M_color = S_rb_tree_black;
                y->M_color = S_rb_tree_black;
                x->M_parent->M_parent->M_color = S_rb_tree_red;
                x = x->M_parent->M_parent;
            }
            else
            {
                if(x == x->M_parent->M_right)
                {
                    x = x->M_parent;
                    Rb_tree_rotate_left(x, root);
                }
                x->M_parent->M_color = S_rb_tree_black;
                x->M_parent->M_parent->M_color = S_rb_tree_red;
                Rb_tree_rotate_right(x->M_parent->M_parent, root);
            }
        }
        else
        {
            Rb_tree_node_base* y = x->M_parent->M_parent->M_left;
            if(y && y->M_color == S_rb_tree_red)
            {
                x->M_parent->M_color = S_rb_tree_black;
                y->M_color = S_rb_tree_black;
                x->M_parent->M_parent->M_color = S_rb_tree_red;
                x = x->M_parent->M_parent;
            }
            else
            {
                if(x == x->M_parent->M_left)
                {
                    x = x->M_parent;
                    Rb_tree_rotate_right(x, root);
                }
                x->M_parent->M_color = S_rb_tree_black;
                x->M_parent->M_parent->M_color = S_rb_tree_red;
                Rb_tree_rotate_left(x->M_parent->M_parent, root);
            }
        }
    }
    root->M_color = S_rb_tree_red;
}

inline Rb_tree_node_base* Rb_tree_rebalance_for_erase(Rb_tree_node_base* z,
                                                           Rb_tree_node_base*& root,
                                                           Rb_tree_node_base*& leftmost,
                                                           Rb_tree_node_base*& rightmost)
{
    Rb_tree_node_base* y = z;
    Rb_tree_node_base* x = 0;
    Rb_tree_node_base* x_parent = 0;

    if(y->M_left == 0)
        x = y->M_right;
    else if(y->M_right == 0)
        x = y->M_left;
    else
    {
        y = y->M_right;
        while(y->M_left != 0)
            y = y->M_left;
        x = y->M_right;
    }

    if(y != z)  // 左右孩子均存在
    {
        z->M_left->M_parent = y;
        y->M_left = z->M_left;
        if(y != z->M_right)
        {
            x_parent = y->M_right;
            if(x) x->M_parent = y->M_parent;
            y->M_parent->M_left = x;
            y->M_right = z->M_right;
            z->M_right->M_parent = y;
        }
        else x_parent = y;

        if(root == z)
            root = y;
        else if(z->M_parent->M_left == z)
            z->M_parent->M_left == y;
        else
            z->M_parent->M_right == y;
        y->M_parent = z->M_parent;
        swap(y->M_color, z->M_color);
        y = z;
    }
    else
    {
        x_parent = y->M_parent;
        if(x) x->M_parent = y->M_parent;
        if(root = z)
            root = x;
        else
            if(z->M_parent->M_left == z)
                z->M_parent->M_left = x;
            else
                z->M_parent->M_right = x;

        if(leftmost == z)
            if(z->M_right == 0)
                leftmost = z->M_parent;
            else
                leftmost = Rb_tree_node_base::S_minimum(x);
        if(rightmost == z)
            if(z->M_left == 0)
                rightmost = z->M_parent;
            else
                rightmost = Rb_tree_node_base::S_maximum(x);
    }


    if(y->M_color != S_rb_tree_red)  // rebalance
    {

    }



    return z;
}




template <class Tp, class Alloc, bool S_instanceless>
class Rb_tree_alloc_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return M_node_allocator; }

    Rb_tree_alloc_base(const allocator_type& a)
    : M_node_allocator(a), M_header(0) {}

protected:
    typename Alloc_traits<Rb_tree_node<Tp>, Alloc>::allocator_type M_node_allocator;
    Rb_tree_node<Tp>* M_header;

    Rb_tree_node<Tp>* M_get_node()
    { return M_node_allocator.allocate(1); }

    void M_put_node(Rb_tree_node<Tp>* p)
    { M_node_allocator.deallocate(p, 1); }
};


template <class Tp, class Alloc>
class Rb_tree_alloc_base<Tp, Alloc, true>
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    Rb_tree_alloc_base(const allocator_type& a) : M_header(0) {}

protected:
    Rb_tree_node<Tp>* M_header;

    typedef typename Alloc_traits<Rb_tree_node<Tp>, Alloc>::Alloc_type Alloc_type;

    Rb_tree_node<Tp>* M_get_node()
    { return Alloc_type::allocate(1); }

    void M_put_node(Rb_tree_node<Tp>* p)
    { Alloc_type::deallocate(p, 1); }
};





template <class Tp, class Alloc>
struct Rb_tree_base : public Rb_tree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>
{
    typedef Rb_tree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless> Base;
    typedef typename Base::allocator_type allocator_type;

    Rb_tree_base(const allocator_type& a):Base(a) { Base::M_header = Base::M_get_node(); }
    ~Rb_tree_base() { M_put_node(Base::M_header); }
};


template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc = allocator<Value>>
class Rb_tree : protected Rb_tree_base<Value, Alloc>
{
    typedef Rb_tree_base<Value, Alloc> Base;
protected:
    typedef Rb_tree_node_base* Base_ptr;
    typedef Rb_tree_node<Value> Rb_tree_node;
    typedef Rb_tree_Color_type Color_type;
public:
    typedef Key                 key_type;
    typedef Value               value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef Rb_tree_node*       Link_type;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef typename Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return Base::get_allocator(); }

protected:
    using Base::M_get_node;
    using Base::M_put_node;
    using Base::M_header;

protected:
    Link_type M_create_node(const value_type& x)
    {
        Link_type tmp = M_get_node();
        try
        {
            construct(&tmp->M_value_field, x);
        }
        catch (...)
        {
            M_put_node(tmp);
            throw;
        }
        return tmp;
    }

    Link_type M_clone_node(Link_type x)
    {
        Link_type tmp = M_create_node(x->M_value_field);
        tmp->M_color = x->M_color;
        tmp->M_left = 0;
        tmp->M_right = 0;
        return tmp;
    }

    void destroy_node(Link_type p)
    {
        destroy(&p->M_value_field);
        M_put_node(p);
    }

protected:
    size_type M_node_count;
    Compare M_key_compare;

    Link_type& M_root() const
    {  return (Link_type&) M_header->M_parent; }
    Link_type& M_leftmost() const
    {  return (Link_type&) M_header->M_left; }
    Link_type& M_rightmost() const
    {  return (Link_type&) M_header->M_right; }

    static Link_type& S_left(Link_type x)
    { return (Link_type&) x->M_left; }
    static Link_type& S_right(Link_type x)
    { return (Link_type&) x->M_right; }
    static Link_type& S_parent(Link_type x)
    { return (Link_type&)(x->parent); }
    static reference S_value(Link_type x)
    { return x->M_value_field; }
    static const Key& S_key(Link_type x)
    {  return KeyOfValue()(S_value(x)); }
    static Color_type& S_color(Link_type x)
    { return (Color_type&)(x->M_color); }

    static Link_type& S_left(Base_ptr x)
    { return (Link_type&)(x->M_left); }
    static Link_type& S_right(Base_ptr x)
    { return (Link_type&)(x->M_right); }
    static Link_type& S_parent(Base_ptr x)
    { return (Link_type&)(x->M_parent); }
    static reference S_value(Base_ptr x)
    { return ((Link_type)x)->M_value_field; }
    static const Key& S_key(Base_ptr x)
    { return KeyOfValue()(S_value(Link_type(x)));}
    static Color_type& S_color(Base_ptr x)
    { return (Color_type&)(Link_type(x)->M_color); }

    static Link_type S_minimum(Link_type x)
    { return (Link_type) Rb_tree_node_base::S_minimum(x); }

    static Link_type S_maximum(Link_type x)
    { return (Link_type) Rb_tree_node_base::S_maximum(x); }

public:
    typedef Rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef Rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

private:
    iterator M_insert(Base_ptr x, Base_ptr y, const value_type& v);
    Link_type M_copy(Link_type x, Link_type p);
    void M_erase(Link_type x);

public:
    Rb_tree():Base(allocator_type()), M_node_count(0), M_key_compare()
    { M_empty_initialize(); }

    Rb_tree(const Compare& comp) : Base(allocator_type()), M_node_count(0), M_key_compare(comp)
    { M_empty_initialize(); }

    Rb_tree(const Compare& comp, const allocator_type& a): Base(a), M_node_count(0), M_key_compare(comp)
    { M_empty_initialize(); }

    Rb_tree(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
    : Base(x.get_allocator()), M_node_count(0), M_key_compare(x.M_key_compare)
    {
        if(x.M_root() == 0)
            M_empty_initialize();
        else
        {
            S_color(M_header) = S_rb_tree_red;
            M_root() = M_copy(x.M_root(), M_header);
            M_leftmost() = S_minimum(M_root());
            M_rightmost() = S_maximum(M_root());
        }
        M_node_count = x.M_node_count;
    }
    ~Rb_tree() { clear(); }

    Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
    operator=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
private:
    void M_empty_initialize()
    {
        S_color(M_header) = S_rb_tree_red;
        M_root() = 0;
        M_leftmost() = M_header;
        M_rightmost() = M_header;
    }

public:
    Compare key_comp() const { return M_key_compare; }
    iterator begin() { return M_leftmost(); }
    const_iterator begin() const { return M_leftmost(); }
    iterator end() { return M_header; }
    const_iterator end() const { return M_header; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(begin()); }
    bool empty() const { return M_node_count == 0; }
    size_type size() const { return M_node_count; }
    size_type max_size() const { return size_type(-1); }

    void swap(Rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& t)
    {
        swap(M_header, t._M_header);
        swap(M_node_count, t._M_node_count);
        swap(M_key_compare, t._M_key_compare);
    }
public:
    pair<iterator, bool> insert_unique(const value_type& x);
    iterator insert_equal(const value_type& x);

    iterator insert_unique(iterator position, const value_type& x);
    iterator insert_equal(iterator position, const value_type& x);

    template<class InputIterator>
    void insert_unique(InputIterator first, InputIterator last);

    template<class InputIterator>
    void insert_equal(InputIterator first, InputIterator last);

    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void erase(const key_type* first, const key_type* last);
    void clear()
    {
        if(M_node_count != 0)
        {
            M_erase(M_root());
            M_leftmost() = M_header;
            M_root() = 0;
            M_rightmost() = M_header;
            M_node_count = 0;
        }
    }
public:
    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x) ;
    const_iterator upper_bound(const key_type& x) const;
    pair<iterator, iterator> equal_range(const key_type& x);
    pair<const_iterator , const_iterator> equal_range(const key_type& x) const;

public:
    bool rb_verify() const;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator==(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator!=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(x==y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return y < x;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(y < x);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(x < y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool swap(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                 const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    x.swap(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        operator=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
{
    if(this != &x)
    {
        clear();
        M_node_count = 0;
        M_key_compare = x.M_key_compare;
        if(x.M_root() == 0)
        {
            M_root() = 0;
            M_leftmost() = M_header;
            M_rightmost() = M_header;
        }
        else
        {
            M_root() = M_copy(x.M_root(), M_header);
            M_leftmost() = S_minimum(M_root());
            M_rightmost() = S_maximum(M_root());
            M_node_count = x.M_node_count;
        }
    }
    return *this;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_insert(Base_ptr x, Base_ptr y, const value_type &v)
{
    Link_type _x = (Link_type) x;
    Link_type _y = (Link_type) y;
    Link_type _z;
    if(y == M_header || x != 0 || M_key_compare(KeyOfValue()(v), S_key(_y)))
    {
        _z = M_create_node(v);
        S_left(y) = _z;

        if(_y == M_header)
        {
            M_root() = _z;
            M_rightmost() = _z;
        }
        else if(y == M_leftmost())
            M_leftmost() = _z;
    }
    else
    {
        _z = M_create_node(v);
        S_right(_y) = _z;
        if(y == M_rightmost())
            M_rightmost() = _z;
    }
    S_parent(_z) = _y;
    S_left(_z) = 0;
    S_right(_z) = 0;
    Rb_tree_rebalance(_z, M_header->M_parent);
    ++M_node_count;
    return iterator(_z);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &v)
{
    Link_type _y = M_header;
    Link_type _x = M_root();
    while(_x != 0)
    {
        _y = _x;
        _x = M_key_compare(KeyOfValue()(v), S_key(_x)) ?
             S_left(_x) : S_right(_x);
    }
    return M_insert(_x, _y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
{
    Link_type y = M_header;
    Link_type x = M_root();
    bool comp = true;
    while(x != 0)
    {
        y = x;
        comp = M_key_compare(KeyOfValue()(v), S_key(x));
        x = comp ? S_left(x) : S_right(x);
    }
    iterator j = iterator(y);
    if(comp)
    {
        if(j == begin())
            return pair<iterator, bool>(M_insert(x, y, v), true);
        else
            --j;
        if(M_key_compare(S_key(j.M_node), KeyOfValue()(v)))
            return pair<iterator, bool>(M_insert(x,y,v), true);
        return pair<iterator, bool>(j, false);
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Value& v)
{
    if(position.M_node == M_header->M_left)
    {
        if(size() > 0 && M_key_compare(KeyOfValue()(v), S_key(position.M_node)))
            return M_insert(position.M_node, position.M_node, v);
        else
            return insert_unique(v).first;
    }
    else if(position.M_node == M_header)
    {
        if(M_key_compare(S_key(M_rightmost())), KeyOfValue()(v))
            return M_insert(0, M_rightmost(), v);
        else
            return insert_unique(v).first;
    }
    else
    {
        iterator before = position;
        --before;
        if(M_key_compare(S_key(before.M_node), KeyOfValue()(v)) &&
        M_key_compare(KeyOfValue()(v), S_key(position.M_node)))
        {
            if(S_right(before.M_node) == 0)
                return M_insert(0, before.M_node, v);
            else
                return M_insert(position.M_node, position.M_node, v);
        }
        else
            return insert_unique(v).first;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator position, const Value& v)
{
    if(position.M_node == M_header->M_left)
    {
        if(size() > 0 && !M_key_compare(S_key(position.M_node), KeyOfValue()(v)))
            return M_insert(position.M_node, position.M_node, v);
        else
            return insert_equal(v);
    }
    else if(position.M_node == M_header)
    {
        if(!M_key_compare(KeyOfValue()(v), S_key(M_rightmost())))
            return M_insert(0, M_rightmost(), v);
        else
            return insert_equal(v);
    }
    else
    {
        iterator before = position;
        --before;
        if(!M_key_compare(KeyOfValue()(v), S_key(before.M_node)) &&
        !M_key_compare(S_key(position.M_node), KeyOfValue()(v)))
        {
            if(S_right(before.M_node) == 0)
                return M_insert(0, before.M_node, v);
            else
                return M_insert(position.M_node, position.M_node, v);
        }
        else
            return insert_equal(v);
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class II>
void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(II first, II last)
{
    for(; first != last; ++ first)
        insert_equal(*first);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class II>
void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(II first, II last)
{
    for(; first != last; ++ first)
        insert_unique(*first);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
    Link_type y = (Link_type) Rb_tree_rebalance_for_erase(position.M_node,
                                                          M_header->M_parent,
                                                          M_header->M_left,
                                                          M_header->M_right);
    destroy_node(y);
    --M_node_count;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x)
{
    pair<iterator, iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::Link_type
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_copy(Link_type x, Link_type p)
{
    Link_type top = M_clone_node(x);
    top->M_parent = p;

    try
    {
        if(x->M_right)
            top->M_right = M_copy(S_right(x), top);
        p = top;
        x = S_left(x);

        while(x != 0)
        {
            Link_type y = M_clone_node(x);
            p->M_left = y;
            y->M_parent = p;
            if(x->M_right)
                y->M_right = M_copy(S_right(x), y);
            p = y;
            x = S_left(x);
        }
    }
    catch (...)
    {
        M_erase(top);
    }
    return top;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_erase(Link_type x)
{
    while(x != 0)
    {
        M_erase(S_right(x));
        Link_type y = S_left(x);
        destroy_node(x);
        x = y;
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last)
{
    if(first == begin() && last == end())
        clear();
    else
        while(first != last) erase(first ++);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key* first, const Key* last)
{
    while(first != last) erase(*first ++);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k)
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(!M_key_compare(S_key(x), k))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    iterator j = iterator(y);
    return (j == end() || M_key_compare(k, S_key(j.M_node))) ? end() : j;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k) const
{
    pair<const_iterator, const_iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k)
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(!M_key_compare(S_key(x), k))
            y = x, x = S_left(x);
        else
            x = S_right(x);
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) const
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(!M_key_compare(S_key(x), k))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    return const_iterator(y);
}



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k)
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(M_key_compare(k, S_key(x)))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    return iterator(y);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) const
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(M_key_compare(k, S_key(x)))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    return iterator(y);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline
pair<typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
     typename Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) const
{
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

inline int
__black_count(Rb_tree_node_base* node, Rb_tree_node_base* root)
{
    if(node == 0)
        return 0;
    else
    {
        int bc = node->M_color == S_rb_tree_black ? 1 : 0;
        if(node == root)
            return bc;
        else
            return bc + __black_count(node->M_parent, root);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_verify() const
{
    if(M_node_count == 0 || begin() == end())
        return M_node_count == 0 && begin() == end() &&
        M_header->M_left == M_header && M_header->M_right == M_header;

    int len = __black_count(M_leftmost(), M_root());
    for(const_iterator it = begin(); it != end(); ++it)
    {
        Link_type x = (Link_type) it.M_node;
        Link_type L = S_left(x);
        Link_type R = S_right(x);

        if(x->M_color == S_rb_tree_red)
            if((L && L->M_color == S_rb_tree_red) || R && R->M_color == S_rb_tree_red)
                return false;

        if(L && M_key_compare(S_key(x), S_key(L)))
            return false;
        if(R && M_key_compare(S_key(R), S_key(x)))
            return false;

        if(!L && !R && __black_count(x, M_root()) != len)
            return false;
    }

    if(M_leftmost() != Rb_tree_node_base::S_minimum(M_root()))
        return false;
    if(M_rightmost() != Rb_tree_node_base::S_maximum(M_root()))
        return false;

    return true;
}




}
#endif //MYSTL_STL_RB_TREE_H
