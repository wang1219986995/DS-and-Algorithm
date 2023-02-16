#include "type_traits.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"
#include "stl_algo.h"


#define stature(p) ((p) ? p->M_height : -1)
#define updateHeight(p) p->M_height = 1 + max(stature(p->M_left), stature(p->M_right));
#define BalFac(x) stature(x->M_left) - stature(x->M_right)
#define AvlBalanced(x) (-2 < BalFac(x) && (BalFac(x) < 2))

#ifndef MYSTL_STL_AVL_TREE_H
#define MYSTL_STL_AVL_TREE_H
namespace MySTL
{



struct Avl_tree_node_base
{
    typedef Avl_tree_node_base* Base_ptr;

    Base_ptr M_parent;
    Base_ptr M_left;
    Base_ptr M_right;
    int M_height;

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
struct Avl_tree_node : public Avl_tree_node_base
{
    typedef Avl_tree_node<Value>* Link_type;
    Value M_value_field;
};



struct Avl_tree_base_iterator
{
    typedef Avl_tree_node_base::Base_ptr Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
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
                y = M_node->M_parent;
            }
            if(M_node->M_right != y)
                M_node = y;
        }
    }

    void M_decrement()
    {
        if(M_node->M_left != 0)
        {
            M_node = M_node->M_left;
            while(M_node->M_right != 0)
                M_node = M_node->M_right;
        }
        else
        {
            Base_ptr y = M_node->M_parent;
            while(y == M_node->M_left)
            {
                M_node = y;
                y = M_node->M_parent;
            }
            M_node = y;
        }
    }
};

template <class Value, class Ref, class Ptr>
struct Avl_tree_iterator : public Avl_tree_base_iterator
{
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef Avl_tree_iterator<Value, Value&, Value*> iterator;
    typedef Avl_tree_iterator<Value, const Value&, const Value*> const_iterator;
    typedef Avl_tree_iterator<Value, Ref, Ptr> Self;
    typedef Avl_tree_node<Value>* Link_type;

    Avl_tree_iterator() {}
    Avl_tree_iterator(Link_type x) { M_node = x; }
    Avl_tree_iterator(const iterator& it) { M_node = it.M_node; }

    reference operator*() const { return Link_type(M_node)->M_value_field; }

    Self& operator++() { M_increment(); return *this; }
    Self operator++(int) {
        Self tmp = *this;
        M_increment();
        return tmp;
    }
    Self& operator--() { M_decrement(); return *this; }
    Self operator--(int) {
        Self tmp = *this;
        M_decrement();
        return tmp;
    }
};

inline bool operator==(const Avl_tree_base_iterator& x,
                       const Avl_tree_base_iterator& y)
{
    return x.M_node == y.M_node;
}

inline bool operator!=(const Avl_tree_base_iterator& x,
                       const Avl_tree_base_iterator& y)
{
    return x.M_node != y.M_node;
}

inline bidirectional_iterator_tag iterator_category(const Avl_tree_base_iterator&)
{
    return bidirectional_iterator_tag();
}

inline Avl_tree_base_iterator::difference_type* distance_type(const Avl_tree_base_iterator)
{
    return (Avl_tree_base_iterator::difference_type*) 0;
}

template <class Value, class Ref, class Ptr>
inline Value* value_type(const Avl_tree_iterator<Value, Ref, Ptr>&)
{
    return (Value*) 0;
}




void rotateL(Avl_tree_node_base* p, Avl_tree_node_base*& root)
{
    Avl_tree_node_base* childR = p->M_right;
    Avl_tree_node_base* childRL = childR->M_left;
    Avl_tree_node_base* g = p->M_parent; // g 可能为M_header

    p->M_right = childRL;
    if(childRL) childRL->M_parent = p;

    childR->M_parent = p->M_parent;

    if(p == root)
        root = childR;
    else if(p == p->M_parent->M_left)
        p->M_parent->M_left = p;
    else
        p->M_parent->M_right = p;

    childR->M_left = p;
    p->M_parent = childR;

    updateHeight(p);
    updateHeight(childR);
    updateHeight(g);
}

void rotateR(Avl_tree_node_base* p, Avl_tree_node_base*& root)
{
    Avl_tree_node_base* childL = p->M_left;
    Avl_tree_node_base* childLR = childL->M_right;
    Avl_tree_node_base* g = p->M_parent; // g 可能为M_header


    p->M_right = childLR;
    if(childLR) childLR->M_parent = p;

    childL->M_right = p;
    p ->M_parent = childL;

    childL->M_parent = p->M_parent;
    if(p == root)
        root = childL;
    else if( p == p->M_parent->M_left)
        p->M_parent->M_left = p;
    else
        p->M_parent->M_right = p;
    updateHeight(p);
    updateHeight(childL);
    updateHeight(g);
}

// p 为插入节点父节点，g 应该没用
void Avl_tree_rebalance_for_insert(Avl_tree_node_base* p, Avl_tree_node_base*& root)
{
    while(p != root->M_parent)
    {
        Avl_tree_node_base* p_next = p->M_parent;
        updateHeight(p);
        if(BalFac(p) <= -2 || BalFac(p) >= 2)
        {
            if(BalFac(p) == 2)
            {
                if(BalFac(p->M_left) == -1)
                {
                    rotateL(p->M_left, root);
                    rotateR(p, root);
                }
                else
                    rotateR(p, root);
            }
            else if(BalFac(p) == -2)
            {
                if(BalFac(p->M_right) == 1)
                {
                    rotateR(p->M_right, root);
                    rotateL(p, root);
                }
                else
                    rotateL(p, root);
            }
        }
        p = p_next;
    }
}


inline Avl_tree_node_base*
Avl_tree_rebalance_for_erase(Avl_tree_node_base* z,
                             Avl_tree_node_base*& root,
                             Avl_tree_node_base*& leftmost,
                             Avl_tree_node_base*& rightmost)
{
    Avl_tree_node_base* y = z;   // 实际被删除的节点(z 或者 z的后继)
    Avl_tree_node_base* x = 0;   // y 的孩子节点
    Avl_tree_node_base* x_parent = 0;

    if(y->M_left == 0)       // 左孩子为null
        x = y->M_right;      // 此时 x 可能为空，可能非空
    else if(y->M_right == 0) // 右孩子为null，左孩子非null
        x = y->M_left;
    else                    // 两个孩子都非null
    {
        y = y->M_right;
        while(y->M_left != 0)
            y = y->M_left;
        x = y->M_right;
    }

    if(y != z) // y 是 z 的后继
    {
        z->M_left->M_parent = y;
        y->M_left = z->M_left;
        if(y != z->M_right)
        {
            x_parent = y->M_parent;
            if(x) x->M_parent = y->M_parent;
            y->M_parent->M_left = x;
            y->M_right = z->M_right;
            z->M_right->M_parent = y;
        }
        else
            x_parent = y;

        if(root == z)
            root = y;
        else if(z->M_parent->M_left == z)
            z->M_parent->M_left = y;
        else
            z->M_parent->M_right = y;
        y->M_parent = z->M_parent;
        y = z;  // y is deleted pointer
    }
    else   // y == z
    {
        x_parent = y->M_parent;
        if(x) x->M_parent = y->M_parent;
        if(root == z)
            root = x;
        else
        if(z->M_parent->M_left == z)
            z->M_parent->M_left = x;
        else
            z->M_parent->M_right = x;

        // leftmost、 rightmost 处理
        if(leftmost == z)
            if(z->M_right == 0)
                leftmost = z->M_parent;
            else
                leftmost = Avl_tree_node_base::S_minimum(x);
        if(rightmost == z)
            if(z->M_left == 0)
                rightmost = z->M_parent;
            else
                rightmost = Avl_tree_node_base::S_maximum(x);
    }

    // 从 x 节点开始向上遍历修复
    while(x_parent != root->M_parent)
    {
        Avl_tree_node_base* x_next = x_parent->M_parent;
        updateHeight(x_parent);
        if(BalFac(x_parent) <= -2 || BalFac(x_parent) >= 2)
        {
            if(BalFac(x_parent) == 2)
            {
                if(BalFac(x_parent->M_left) == -1)
                {
                    rotateL(x_parent->M_left, root);
                    rotateR(x_parent, root);
                }
                else
                    rotateR(x_parent, root);
            }
            else if(BalFac(x_parent) == -2)
            {
                if(BalFac(x_parent->M_right) == 1)
                {
                    rotateR(x_parent->M_right, root);
                    rotateL(x_parent, root);
                }
                else
                    rotateL(x_parent, root);
            }
        }
        x_parent = x_next;
    }
}


template <class Tp, class Alloc, bool S_instanceless>
class Avl_tree_alloc_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return M_node_allocator; }

    Avl_tree_alloc_base(const allocator_type& a)
    : M_node_allocator(a), M_header(0) {}

protected:
    typename Alloc_traits<Avl_tree_node<Tp>, Alloc>::allocator_type M_node_allocator;
    Avl_tree_node<Tp>* M_header;

    Avl_tree_node<Tp>* M_get_node()
    {  return M_node_allocator.allocate(1); }
    void M_put_node(Avl_tree_node<Tp>* p)
    { M_node_allocator.deallocate(p, 1); }
};

template <class Tp, class Alloc>
class Avl_tree_alloc_base<Tp, Alloc, true>
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    Avl_tree_alloc_base(const allocator_type&) : M_header(0) {}
protected:
    Avl_tree_node<Tp>* M_header;

    typedef typename Alloc_traits<Avl_tree_node<Tp>, Alloc>::Alloc_type Alloc_type;
    Avl_tree_node<Tp>* M_get_node() { return Alloc_type::allocate(1); }
    void M_put_node(Avl_tree_node<Tp>* p)
    { Alloc_type::deallocate(p, 1); }
};



template <class Tp, class Alloc>
struct Avl_tree_base : public Avl_tree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>
{
    typedef Avl_tree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless> Base;
    typedef typename Base::allocator_type allocator_type;

    Avl_tree_base(const allocator_type& a)
    : Base(a) { Base::M_header = Base::M_get_node(); }
    ~Avl_tree_base() { Base::M_put_node(Base::M_header); }
};


template <class Key, class Value, class KeyOfValue,
        class Compare, class Alloc=allocator<Value>>
class Avl_tree : protected Avl_tree_base<Value, Alloc>
{
    typedef Avl_tree_base<Value, Alloc> Base;
protected:
    typedef Avl_tree_node_base* Base_ptr;
    typedef Avl_tree_node<Value> Avl_tree_node;
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef Avl_tree_node* Link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

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
        tmp->M_left = 0;
        tmp->M_right = 0;
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
    { return (Link_type&) M_header->M_parent; }
    Link_type& M_leftmost() const
    {  return (Link_type&) M_header->M_left;}
    Link_type& M_rightmost() const
    { return (Link_type&) M_header->M_right; }

    static Link_type& S_left(Link_type x)
    { return (Link_type&)(x->M_left); }
    static Link_type& S_right(Link_type x)
    { return (Link_type&)(x->M_right); }
    static Link_type& S_parent(Link_type x)
    { return (Link_type&)(x->M_parent); }
    static reference S_value(Link_type x)
    { return x->M_value_field; }
    static const Key& S_key(Link_type x)
    { return KeyOfValue()(S_value(x)); }

    static Link_type& S_left(Base_ptr x)
    { return (Link_type&)(x->M_left); }
    static Link_type& S_right(Base_ptr x)
    { return (Link_type&)(x->M_right); }
    static Link_type& S_parent(Base_ptr x)
    { return (Link_type&)(x->M_parent); }
    static reference S_value(Base_ptr x)
    { return ((Link_type) x)->M_value_field; }
    static const Key& S_key(Base_ptr x)
    { return KeyOfValue()(S_value(Link_type(x))); }

    static Link_type S_minimum(Link_type x)
    { return (Link_type) Avl_tree_node_base::S_minimum(x); }
    static Link_type S_maximum(Link_type x)
    { return (Link_type) Avl_tree_node_base::S_maximum(x); }

public:
    typedef Avl_tree_iterator<value_type, reference, pointer> iterator;
    typedef Avl_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

private:
    iterator M_insert(Base_ptr x, Base_ptr y, const value_type& v);
    Link_type M_copy(Link_type x, Link_type p);
    void M_erase(Link_type x);

public:
    Avl_tree() : Base(allocator_type()), M_node_count(0), M_key_compare()
    { M_empty_initialzie(); }

    Avl_tree(const Compare& comp)
    : Base(allocator_type()), M_node_count(0), M_key_compare(comp)
    { M_empty_initialzie(); }

    Avl_tree(const Compare& comp, const allocator_type& a)
    : Base(a), M_node_count(0), M_key_compare(comp)
    { M_empty_initialzie(); }

    Avl_tree(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
    : Base(x.get_allocator()), M_node_count(0), M_key_compare(x.M_key_compare)
    {
        if(x.M_root() == 0)
            M_empty_initialzie();
        else
        {
            M_root() = M_copy(x.M_root(), M_header);
            M_leftmost() = S_minimum(M_root());
            M_rightmost() = S_maximum(M_root());
        }
        M_node_count = x.M_node_count;
    }
    ~Avl_tree() { clear(); }
    Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>&
    operator=(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
private:
    void M_empty_initialzie()
    {
        M_root() = 0;
        M_leftmost() = M_header;
        M_rightmost() = M_header;
    }

public:
    Compare key_comp() const { return M_key_compare; }
    iterator begin() { return M_leftmost(); }
    const_iterator begin() const { return M_leftmost(); }
    iterator end() { return M_rightmost(); }
    const_iterator end() const { return M_rightmost(); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    bool empty() const { return M_node_count == 0; }
    size_type size() const { return M_node_count; }
    size_type max_size() const { return size_type(-1); }

    void swap(Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& t)
    {
        swap(M_header, t.M_header);
        swap(M_node_count, t.M_node_count);
        swap(M_key_compare, t.M_key_compare);
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
    iterator upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    pair<iterator, iterator> equal_range(const key_type& x);
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const;

    bool avl_verify() const;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator==(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return x.size() == y.size() &&
            equal(x.begin(), x.end(), y.begin());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator!=(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(x==y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return y < x;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<=(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                      const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(y < x);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>=(const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                       const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    return !(x < y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void swap(Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                 Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    x.swap(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>&
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(
const Avl_tree<Key, Value, KeyOfValue, Compare, Alloc> &x)
{
    if(this != &x)
    {
        clear();
        M_node_count = 0;
        M_key_compare = x.M_key_compare;
        if(x.M_root() == 0)
        {
            M_root() == 0;
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


// 调用此函数时，插入位置为 y 的孩子节点，且该节点一定为空
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_insert
(Base_ptr _x, Base_ptr _y, const Value& v)
{
    Link_type x = (Link_type) _x;
    Link_type y = (Link_type) _y;
    Link_type z;

    if(y == M_header || x != 0 || M_key_compare(KeyOfValue()(v), S_key(y))) // 这三种条件都是插入为左孩子
    {
        z = M_create_node(v);
        z->M_height = 0;
        S_left(y) = z;  // 树为空时， 此处执行后 M_header->M_left = z; M_leftmost() == z;

        if(y == M_header)
        {
            M_root() = z; // M_header->parent = z;
            M_rightmost() = z; // M_header->M_right = z;
        }
        else if(y = M_leftmost())
            M_leftmost() = z;
    }
    else
    {
        z = M_create_node(v);
        z->M_height = 0;
        S_right(y) = z;
        if(y == M_rightmost())
            M_rightmost() = z;
    }
    S_parent(z) = y;
    S_left(z) = 0;
    S_right(z) = 0;
    Avl_tree_rebalance_for_insert(y, M_header->M_parent);
    ++M_node_count;
    return iterator(z);
}



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
{
    Link_type y = M_header;
    Link_type x = M_root();
    while(x != 0)
    {
        y = x;
        x = M_key_compare(KeyOfValue()(v), S_key(x)) ?
            S_left(x) : S_right(x);
    }
    return M_insert(x, y, v);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
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
    if(comp)  // x < y , x 为 v 插入的地方
        if(j == begin())
            return pair<iterator, bool>(M_insert(x, y, v), true);
        else
            --j;
    if(M_key_compare(S_key(j.M_node), KeyOfValue()(v)))
        return pair<iterator, bool>(M_insert(x, y, v), true);
    return pair<iterator, bool>(j, false); // 已存在元素v
}


template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Val& v)
{
    if(position.M_node == M_header->M_left) // begin()
    {
        if(size() > 0 && M_key_compare(KeyOfValue()(v), S_key(position.M_node)))
            return M_insert(position.M_node, position.M_node, v);
        else
            return insert_unique(v).first;
    }
    else if(position.M_node == M_header)   // end()
    {
        if(M_key_compare(S_key(M_rightmost()), KeyOfValue()(v)))
            return M_insert(0, M_rightmost(), v);
        else
            return insert_unique(v).first;
    }
    else
    {
        iterator before = position;
        --before;
        if(M_key_compare(S_key(before.M_node), KeyOfValue()(v)) &&
        M_key_compare(KeyOfValue()(v), S_key(position.M_node)))    // 前驱 < v < position
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


template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_equal(iterator position, const Val& v)
{
    if(position.M_node == M_header->M_left) // begin()
    {
        if(size() > 0 && !M_key_compare(S_key(position.M_node), KeyOfValue()(v)))
            return M_insert(position.M_node, position.M_node, v);
        else
            return insert_equal(v);
    }
    else if(position.M_node == M_header) // end()
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
        !M_key_compare(S_key(position.M_node), KeyOfValue()(v)))  // before <= v <= position
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


template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
template <class Iterator>
void Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_equal(Iterator first, Iterator last)
{
    for(; first != last; ++first)
        insert_equal(*first);
}

template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
template <class Iterator>
void Avl_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_unique(Iterator first, Iterator last)
{
    for(; first != last; ++first)
        insert_unique(*first);
}



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
    Link_type y = (Link_type) Avl_tree_rebalance_for_erase(position.M_node, M_header->M_parent,
                                                           M_header->M_left, M_header->M_right);
    destroy_node(y);
    --M_node_count;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x)
{
    pair<iterator, iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::Link_type
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_copy(Link_type x, Link_type p)
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
        throw;
    }
    return top;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::M_erase(Link_type x)
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
void Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::erase(iterator first, iterator last)
{
    if(first == begin() && last == end())
        clear();
    else
        while (first != last) erase(first++);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::erase(const Key* first, const Key* last)
{
    while(first != last) erase(*first++);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find(const Key& k)
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(!M_key_compare(S_key(x), k))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    iterator j = iterator(y);
    return (j == end() || M_key_compare(k, S_key(j.M_node))) ?
            end() : j;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find(const Key& k) const
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(!M_key_compare(S_key(x), k))
            y = x, x = S_left(x);
        else
            x = S_right(x);

    const_iterator j = const_iterator(y);
    return (j == end() || M_key_compare(k, S_key(j.M_node))) ?
            end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::count(const Key& k) const
{
    pair<const_iterator, const_iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::lower_bound(const key_type& k)
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
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::lower_bound(const key_type& k) const
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
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::upper_bound(const key_type &k)
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
typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
Avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::upper_bound(const key_type &k) const
{
    Link_type y = M_header;
    Link_type x = M_root();

    while(x != 0)
        if(M_key_compare(k, S_key(x)))
            y = x, x = S_left(x);
        else
            x = S_right(x);
    return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k)
{
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
        typename Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k) const
{
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

//template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
//bool Avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::avl_verift() const
//{
//
//}
//


} // namespace


#endif //MYSTL_STL_AVL_TREE_H
