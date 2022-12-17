#ifndef MYSTL_STL_BTREE_H
#define MYSTL_STL_BTREE_H

#include <stddef.h>
#include <stdint.h>

#include "type_traits.h"
#include "stl_alloc.h"
#include "stl_pair.h"
namespace MySTL
{

template <bool Condition, class A, class B>
class node_count_if
{
    typedef A type;
};

template <class A, class B>
class node_count_if<false, A, B>
{
    typedef B type;
};



// 在map 和 set 中决定 Value是pair 还是 Tp 类型
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc, int TargetNodeSize>
class btree_node
{
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef Alloc allocator_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    enum
    {
        kValueSize = sizeof(Value),
        kNodeValueSpace = TargetNodeSize - 2 * sizeof(void*),
        kTargetNodeSize = TargetNodeSize,
    };

    struct base_fields {
        typedef typename node_count_if<
                (kNodeValueSpace / kValueSize) >= 256, uint16_t, uint8_t>
        ::type field_type;
        bool leaf;
        field_type position;
        field_type max_count;
        field_type count;
        btree_node* parent;
    };

    enum
    {
        kNodeTargetValues = (kTargetNodeSize - sizeof(base_fields))/kValueSize,
        kNodeValues = kNodeTargetValues >= 3 ? kNodeTargetValues : 3,

        //TODO 这两个数字什么意思？
        kExactMatch = 1 << 30,
        kMatchMask = kExactMatch - 1,
    };

    struct leaf_fields : public base_fields
    {
        value_type values[kNodeValues];
    };

    struct internal_fields : public leaf_fields
    {
        btree_node* children[kNodeValues + 1];
    };

    struct root_fields : public internal_fields
    {
        btree_node* rightmost;
        size_type size;
    };

public:
    bool leaf() const { return M_fields.leaf; }
    int position() const { return M_fields.position; }
    void set_position(int v) { M_fields.postion = v;}

    int count() const { return M_fields.count; }
    void set_count(int v) const { M_fields.count = v; }
    int max_count() const { return M_fields.max_count; }

    btree_node* parent() const { return M_fields.parent; }
    bool is_root() const { return parent()->leaf; }
    void make_root()
    {
        //TODO 这个函数是干嘛用的？
        M_fields.parent = M_fields.parent->parent();
    }

    //TODO leftmost 不用管么？
    btree_node* rightmost() const { return M_fields.rightmost; }
    //TODO mutable 类型的变量和方法是干嘛用的？
    btree_node* mutable_rightmost() { return &M_fields.rightmost; }

    size_type size() const { return M_fields.size; }
    //TODO mutable 类型的变量和方法是干嘛用的？
    size_type mutable_size() { return &M_fields.size; }

    btree_node* child(int i) const { return M_fields.children[i]; }
    btree_node** mutable_child(int i) { return &M_fields.children[i]; }
    void set_child(int i, btree_node* c)
    {
        *mutable_child(i) = c;
        c->M_fields.parent = this;
        c->M_fields.position = i;
    }

    template<class Cmp>
    int lower_bound(const value_type& k, const Cmp& c) const
    {
        //TODO
    }

    template<class Cmp>
    int upper_bound(const value_type& k, const Cmp& comp) const
    {
        //TODO
    }



    void insert_value(int i, const value_type& x);
    void remove_value(int i);
    void rebalance_right_to_left(btree_node* sibling, int to_move);
    void rebalance_right_to_right(btree_node* sibling, int to_move);

    void split(btree_node* sibling, int insert_position);
    void merge(btree_node* sibling);
    void swap(btree_node* src);

    //TODO 这三个init函数的实现是什么意思？
    static btree_node* init_leaf()
    {
        //TODO
    }
    static btree_node* init_internal(internal_fields* f, btree_node* parent)
    {
        //TODO
    }
    static btree_node* init_root(root_fields* f, btree_node* parent)
    {
        //TODO
    }

private:
    void value_init(int i)
    { new (&M_fields.values[i]) value_type; }

    void value_init(int i, const value_type &x)
    { new (&M_fields.values[i]) value_type(x); }

    void value_destroy(int i)
    { M_fields.values[i].~value_type(); }

private:
    root_fields M_fields;

private:
    btree_node(const btree_node&);
    void operator=(const btree_node&);
};



template <class Node, class Reference, class Pointer>
class btree_iterator
{
public:
    typedef typename Node::key_type key_type;
    typedef typename Node::value_type value_type;
    typedef typename Node::size_type size_type;
    typedef typename Node::difference_type difference_type;

    typedef Node node_type;
    typedef const Node const_node;

    typedef Reference reference;
    typedef Pointer pointer;
    typedef btree_iterator<Node, reference, pointer> iterator;
    typedef btree_iterator<Node, const reference, const pointer> const_iterator;
    typedef btree_iterator<Node, Reference, Pointer> self_type;

public:
    btree_iterator():node(NULL), position(-1) {}
    btree_iterator(Node *n, int p):node(n), position(p) {}
    btree_iterator(const btree_iterator& x):node(x.node), position(x.position) {}

    void increment()
    {
        if(node->leaf() && ++position < node->count())
            return;
        increment_slow();
    }
    void increment_by(int couont);
    void increment_slow();

    void decrement()
    {
        if(node->leaf() && --position >= 0)
            return;
        decrement_slow();
    }
    void decrement_slow();



    reference operator*() const
    {
        //TODO
    }

    self_type& operator++() { increment(); return *this; }
    self_type operator++(int)
    {
        self_type tmp = *this;
        ++*this;
        return tmp;
    }

    self_type& operator--() { decrement(); return *this; }
    self_type operator--(int)
    {
        self_type tmp = *this;
        --*this;
        return tmp;
    }

private:
    Node* node;
    int position;
};

template <class Node, class Reference, class Pointer>
inline bool operator==(const btree_iterator<Node, Reference, Pointer>& x,
                       const btree_iterator<Node, Reference, Pointer>& y)
{ return x.node == y.node && x.position == y.position; }


template <class Node, class Reference, class Pointer>
inline bool operator!=(const btree_iterator<Node, Reference, Pointer>& x,
                       const btree_iterator<Node, Reference, Pointer>& y)
{ return !(x==y); }




template <class Tp, class Alloc, bool S_instanceless>
class btree_alloc_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return M_node_allocator; }


protected:
    //TODO
//    typename Alloc_traits<btree_node<Tp>, Alloc>::allocator_type M_node_allocator;
//    btree_node<Tp>* M_header;
//
//    btree_node<Tp>* M_get_node()
//    { return M_node_allocator.allocate(1); }
//    void M_put_node(btree_node<Tp>* p)
//    { M_node_allocator.deallocate(p, 1); }
};

template <class Tp, class Alloc>
class btree_base : public btree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless>
{
    typedef btree_alloc_base<Tp, Alloc, Alloc_traits<Tp, Alloc>::S_instanceless> Base;
    typedef typename Base::allocator_type allocator_type;

    //TODO

};




template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc=allocator<Value>, int TargetNodeSize=256>
class btree : protected btree_base<Value, Alloc>
{
public:
    typedef btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize> self_type;
    typedef btree_base<Value, Alloc> Base;
    typedef btree_node<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize> node_type;
protected:
//    typedef _Rb_tree_node_base* _Base_ptr;
//    typedef _Rb_tree_node<_Value> _Rb_tree_node;
    typedef btree_node<Key, Value, Compare, Alloc, TargetNodeSize> node_type;
//    typedef typename node_type::base_fields base_fields;
//    typedef typename node_type::root_fields root_fields;


    enum {
        kNodeValues = node_type::kNodeValues,
        kMinNodeValues = node_type::kNodeValues / 2,
        kValueSize = node_type::kValueSize,
        kExactMatch = node_type::kExactMatch,
        kMatchMatch = node_type::kMatchMask,
    };
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    //TODO Link_type 待定
    typedef node_type* Link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;




protected:
    Link_type M_create_node(const value_type& x)
    {

    }

    Link_type M_clone_node(Link_type x)
    {

    }

    void destroy_node(Link_type p)
    {

    }

protected:


public:
    typedef btree_iterator<value_type, reference, pointer> iterator;
    typedef btree_iterator<value_type, const_reference, const_pointer> const_iterator;
private:
    iterator M_insert(iterator iter, const value_type& v)
    {
        if(!iter.node->leaf())
        {
            --iter;
            ++iter.position;
        }

    }


    Link_type M_copy();
    void M_erase(Link_type x);


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

    }
};











}
#endif //MYSTL_STL_BTREE_H
