#ifndef MYSTL_STL_BTREE_H
#define MYSTL_STL_BTREE_H

#include <stddef.h>
#include <stdint.h>

#include "type_traits.h"
#include "stl_alloc.h"
#include "stl_pair.h"
#include "stl_iterator.h"
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

    //TODO
    key_type& key();



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

// 在map 和 set 中决定 Value是pair 还是 Tp 类型
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc, int TargetNodeSize>
class btree_node
{
public:
    typedef btree_node<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize> self_type;
    typedef Key key_type;
    typedef Value value_type;
    typedef Value mutable_value_type;
    typedef Compare key_compare;
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
    //TODO 初始化时怎么做？
    bool is_root() const { return parent()->leaf; }
    void make_root()
    {
        //TODO 这个函数是干嘛用的？
        M_fields.parent = M_fields.parent->parent();
    }

    //TODO leftmost 不用管么？
    btree_node* rightmost() const { return M_fields.rightmost; }
    btree_node** mutable_rightmost() { return &M_fields.rightmost; }

    size_type size() const { return M_fields.size; }
    size_type* mutable_size() { return &M_fields.size; }

    const key_type& key(int i) const
    { return KeyOfValue()(M_fields.values[i]); }

    reference value(int i)
    { return M_fields.values[i]; }

    const_reference value(int i) const
    {
        return M_fields.values[i];
    }

    mutable_value_type* mutable_value(int i)
    { return &M_fields.values[i]; }

    void value_swap(int i, btree_node* x, int j)
    {
        // TODO
    }

    btree_node* child(int i) const { return M_fields.children[i]; }
    btree_node** mutable_child(int i) { return &M_fields.children[i]; }
    void set_child(int i, btree_node* c)
    {
        *mutable_child(i) = c;
        c->M_fields.parent = this;
        c->M_fields.position = i;
    }

    template<class Cmp>
    int lower_bound(const value_type& k) const
    {
        size_type begin = 0, end = count();
        while (begin != end)
        {
            int mid = (begin + end) / 2;
            if (key_compare()(KeyOfValue(M_fields.values[mid]), k))
                begin = mid + 1;
            else
                end = mid;
        }
        return begin;
    }

    template<class Cmp>
    int upper_bound(const value_type& k, const Cmp& comp) const
    {
        size_type begin = 0, end = count();
        while (begin != end)
        {
            int mid = (begin + end) / 2;
            if (!key_compare()(KeyOfValue(M_fields.values[mid]), k))
                begin = mid + 1;
            else
                end = mid;
        }
        return begin;
    }




    void insert_value(int i, const value_type& x);
    void remove_value(int i)
    {
        //TODO 这个if 里面的没看懂
        if(!leaf())
        {
            for(int j = i + 1; j < count(); ++j)
            {
                *mutable_child(j) = child(j+1);
                child(j)->set_position(j);
            }
            *mutable_child(count()) == NULL;
        }

        set_count(count() - 1);
        for(; i < count() - 1; ++i)
            value_swap(i, this, i+1);
        value_destroy(i);
    }

    void rebalance_right_to_left(btree_node* sibling, int to_move);
    void rebalance_right_to_right(btree_node* sibling, int to_move);

    void split(btree_node* sibling, int insert_position);
    void merge(btree_node* sibling);
    void swap(btree_node* src);

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
    // allocator_type get_allocator() const { return M_node_allocator; }


protected:
    //TODO
    // typename Alloc_traits<btree_node<>, Alloc>::allocator_type M_node_allocator;
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
    typedef Key key_type;
    typedef Value value_type;
    typedef Compare key_compare;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef btree_iterator<node_type, reference, pointer> iterator;
    typedef btree_iterator<node_type, const reference, const pointer> const_iterator;
    typedef const reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;
    typedef typename Base::allocator_type allocator_type;

    typedef typename node_type::base_fields base_fields;
    typedef typename node_type::leaf_fields leaf_fields;
    typedef typename node_type::internal_fields internal_fields;
    typedef typename node_type::root_fields root_fields;
    //TODO get_allocator

    typedef typename allocator_type::template rebind<char>::other
            internal_allocator_type;

private:
    enum
    {
        kNodeValues = node_type::kNodeValues,
        kMinNodeValues = node_type::kNodeValues / 2,
        kValueSize = node_type::kValueSize,

        kExactMatch = node_type::kExactMatch,
        kMatchMatch = node_type::kMatchMask,
    };
    struct node_states{
        node_states(size_type l, size_type i) : leaf_nodes(l), internal_nodes(i){}

        node_states& operator+=(const node_states& x)
        {
            leaf_nodes += x.leaf_nodes;
            internal_nodes += x.internal_nodes;
            return *this;
        }

        size_type leaf_nodes;
        size_type internal_nodes;
    };


public:
    //TODO 构造函数不完全
    btree(const key_compare& comp, const allocator_type& alloc);
    btree(const self_type& x);
    ~btree(){ /*TODO  */ }




    Compare key_comp() const { return M_key_compare; }
    iterator begin() { return iterator(leftmost(), 0); }
    const_iterator begin() const { return const_iterator(leftmost(), 0); }

    iterator end() { iterator(rightmost(), rightmost() ? rightmost()->count() : 0); }

    const_iterator end() const { const_iterator(rightmost(), rightmost() ? rightmost()->count() : 0); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { reverse_iterator(begin()); }
    const_reverse_iterator rend() const { const_reverse_iterator(begin()); }


    bool empty() const { return root() == NULL; }
    size_type size() const
    {
        if(empty()) return 0;
        if(root()->leaf()) return root()->count();
        //TODO: 要不要维护一个大小的变量
        return root()->size();
    }
    size_type max_size() const { return size_type(-1); }

    void swap(self_type& x)
    {
        //TODO 如果加其他成员变量的话，这里可能也要变
        std::swap(M_root, x.M_root);
    }


    // insert and erase
    pair<iterator, bool> insert_unique(const value_type& x);

    iterator insert_unique(iterator position, const value_type& x);

    template<class InputIterator>
    void insert_unique(InputIterator first, InputIterator last);

    iterator insert_equal(const value_type& x);

    iterator insert_equal(iterator position, const value_type& x);

    template<class InputIterator>
    void insert_equal(InputIterator first, InputIterator last);


    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void erase(const key_type* first, const key_type* last);
    void clear() {}


    // find and bound
    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;

    iterator lower_bound(const key_type& key)
    {
        //TODO
    }

    const_iterator lower_bound(const key_type& key) const
    {
        //TODO
    }

    iterator upper_bound(const key_type& key)
    {
        //TODO
    }

    const_iterator upper_bound(const key_type& key) const
    {
        //TODO
    }

    pair<iterator, iterator> equal_range(const key_type& x);
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const;






private:
    node_type* root() { return M_root; }
    const node_type* root() const { return M_root; }
    node_type** mutable_root() { return &M_root; }

    node_type* rightmost() { return (!root() || root()->leaf()) ? root() : root()->rightmost(); }
    const node_type* rightmost() const { return (!root() || root()->leaf()) ? root() : root()->rightmost(); }
    node_type** mutable_rightmost() const { return root()->mutable_rightmost(); }

    //TODO leftmost 为什么设置为 root 父节点？
    node_type* leftmost() { return root() ? root()->parent() : NULL; }
    const node_type* leftmost() const { return root()? root()->parent():NULL; }

    size_type* mutable_size() { return root()->mutable_size(); }


    internal_allocator_type* mutable_internal_allocator()
    {

    }





private:
    // node creation and deletion
    node_type* new_internal_node(node_type *parent)
    {
        internal_fields *p = reinterpret_cast<internal_fields*>(
                mutable_internal_allocator()->allocate(sizeof(internal_fields)));
        return node_type::init_internal(p, parent);
    }
    node_type* new_internal_root_node()
    {
        root_fields *p = reinterpret_cast<root_fields*>(
                mutable_internal_allocator()->allocate(sizeof(root_fields)));
        return node_type::init_root(p, root()->parent());
    }
    node_type* new_leaf_node(node_type *parent)
    {
        leaf_fields *p = reinterpret_cast<leaf_fields*>(
                mutable_internal_allocator()->allocate(sizeof(leaf_fields)));
        return node_type::init_leaf(p, parent, kNodeValues);
    }
    node_type* new_leaf_root_node(int max_count)
    {
        leaf_fields *p = reinterpret_cast<leaf_fields*>(
                mutable_internal_allocator()->allocate(
                        sizeof(base_fields) + max_count * sizeof(value_type)));
        return node_type::init_leaf(p, reinterpret_cast<node_type*>(p), max_count);
    }
    void delete_internal_node(node_type *node)
    {
        node->destroy();
        assert(node != root());
        mutable_internal_allocator()->deallocate(
                reinterpret_cast<char*>(node), sizeof(internal_fields));
    }
    void delete_internal_root_node()
    {
        root()->destroy();
        mutable_internal_allocator()->deallocate(
                reinterpret_cast<char*>(root()), sizeof(root_fields));
    }
    void delete_leaf_node(node_type *node)
    {
        node->destroy();
        mutable_internal_allocator()->deallocate(
                reinterpret_cast<char*>(node),
                sizeof(base_fields) + node->max_count() * sizeof(value_type));
    }

    void rebalance_or_split(iterator* iter);

    void merge_nodes(node_type* left, node_type* right)
    {
        left->merge(right);
        if(right->leaf())
        {
            if(rightmost() == right)
                *mutable_rightmost() = left;
            delete_leaf_node(right);
        }
        else
            delete_internal_node(right);
    }

    bool try_merge_or_rebalance(iterator* iter)
    {
        node_type* parent = iter->node->parent();
        if(iter->node->position() > 0)
        {
            node_type* left = parent->child(iter->node->position() - 1);
            if((1 + left->count() + iter->node->count()) <= left->make_count())
            {
                iter->position += 1 + left->count();
                merge_nodes(left, iter->node);
                iter->node = left;
                return true;
            }
        }


        if(iter->node->position() < parent->count())
        {
            node_type* right = parent->child(iter->node->position() + 1);
            if((1 + iter->node->count() + right->count()) <= right->max_count())
            {
                merge_nodes(iter->node, right);
                return true;
            }


            if((right->count() > kMinNodeValues) &&
            (iter->node->count() == 0 || iter->position > 0))
            {

            }
        }

        if(iter->node->position() > 0)
        {
            node_type* left = parent->child(iter->node->position() - 1);





        }

    }

    void try_shrink()
    {
        if(root()->count() > 0) return;

        if(root()->leaf())
        {
            delete_leaf_node(root());
            *mutable_root() = NULL;
        }
        else
        {
            node_type* child = root()->child(0);
            if(child->leaf())
            {
                child->make_root();
                delete_internal_root_node();
                *mutable_root() = child;
            }
            else
            {
                child->swap(root());
                delete_internal_node(child);
            }
        }
    }

    iterator internal_end(iterator iter)
    { return iter.node ? iter : end(); }
    const_iterator internal_end(iterator iter) const
    { return iter.node ? iter : end(); }

    iterator internal_insert(iterator iter, const value_type& v);

    template<typename Itertype>
    static Itertype internal_last(Itertype iter);

    template<class Itertype>
    std::pair<Itertype, int> internal_locate(const key_type& key, Itertype iter) const
    {
        for(;;)
        {
            iter.position = iter.node->lower_bound(key, M_key_compare);
            if(iter.node->leaf())  break;
            iter.node = iter.node->child(iter.position);
        }
        //TODO 这里的第二个值总是返回0，能否优化掉？
        return std::make_pair(iter, 0);
    }


private:
    node_type* M_root;

    // 还用么？
    size_type M_node_count;
    Compare M_key_compare;

};


template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
pair<typename btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::iterator, bool>
btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::insert_unique(const value_type& x)
{
    if(empty())
        *mutable_root() = new_leaf_root_node(1);

     std::pair<iterator, int> res = internal_locate(KeyOfValue()(x), iterator(root(), 0));
     iterator& iter = res.first;
     if(!res.second)
     {
         iterator last = internal_last(iter);

         // 已存在该值
         if(last.node && !M_key_compare(KeyOfValue()(x), last.key()))
             return std::make_pair(last, false);
     }
     return std::make_pair(internal_insert(iter, x), true);
};


template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
typename btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::iterator
btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::insert_unique(iterator position, const value_type& x)
{
    if(!empty())
    {
        // x < poistion
        if(position == end() || M_key_compare(KeyOfValue()(x), position.key()))
        {
            iterator prev = position;
            if(position == begin() || M_key_compare((--prev).key(), KeyOfValue()(x)))
                return internal_insert(position, x);
        }
        else if(M_key_compare(position.key(), KeyOfValue()(x))) // poistion < x
        {
            iterator next = position;
            ++next;
            if(next == end() || M_key_compare(KeyOfValue()(x), next.key()))
                return internal_insert(next, x);
        }
        else // poistion == x
            return position;

    }
    return insert_unique(x).first;

}

template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
template <class InputIterator>
void btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::
insert_unique(InputIterator first, InputIterator last)
{
    for(; first != last; ++first)
        insert_unique(end(), *first);
}




template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
typename btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::iterator
btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::insert_equal(const value_type &x)
{
    if(empty())
        *mutable_root() = new_leaf_root_node(1);

    iterator iter = upper_bound(KeyOfValue()(x), iterator(root(), 0));
    if(!iter.node)
        iter=end();
    return internal_insert(iter, x);
}



template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
typename btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::iterator
btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::insert_equal(iterator position, const value_type& x)
{
    if(!empty())
    {
        if(position == end() || !M_key_compare(position.key(), KeyOfValue()(x)))
        {
            iterator prev = position;
            if(position == begin() || !M_key_compare(KeyOfValue()(x), position.key()))
            {
                return internal_insert(position, x);
            }
        }
        else
        {
            iterator next = position;
            ++next;
            if(next == end() || !M_key_compare(next.key(), KeyOfValue()(x)))
                return internal_insert(next, x);
        }
    }
    return insert_equal(x);
}

template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
template <class InputIterator>
void btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::
insert_equal(InputIterator first, InputIterator last)
{
    for(; first != last; ++first)
        insert_equal(end(), *first);
}


template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, int TargetNodeSize>
typename btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::iterator
btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::internal_insert(iterator iter, const value_type &v)
{
    if(!iter->leaf())
    {
        --iter;
        ++iter.position; // 实际是将 position 置为 count()
    }

    if(iter.node->count() == iter.node->max_count())
    {
        if(iter.node->max_count() < kNodeValues)
        {
            iter.node = new_leaf_root_node(std::min<int>(kNodeValues, 2 * iter.node->max_count()));
            iter.node->swap(root());
            delete_leaf_node(root());
            *mutable_root() = iter.node;
        }
        else
        {
            rebalance_or_split(&iter);
            ++*mutable_size();
        }

    }
    else if(!root()->leaf())
    {
        ++*mutable_size();
    }
    iter.node->insert_value(iter.position,v );
    return iter;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc, int TargetNodeSize>
void btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize>::erase(iterator poistion)
{
    bool internal_delete = false;
    if(!poistion.node->leaf())
    {
        iterator tmp_iter(poistion--);
        poistion.node->value_swap(poistion.position, tmp_iter.node, tmp_iter.position);
        internal_delete = true;
        --*mutable_size();
    }
    else if(!root()->leaf())
        --*mutable_size();

    poistion->remove_value(poistion.position);

    iterator res(poistion);
    for(;;)
    {
        if(poistion.node == root())
        {
            try_shrink();
            if(empty())
                return end();
            break;
        }
        if(poistion.node->count() >= kMinNodeValues)
            break;
        bool merged = try_merge_or_rebalance(&poistion);
        if(poistion.node->leaf())
            res = poistion;
        if(!merged)
            break;
        poistion.node = poistion.node->parent();
    }

}

//    void erase(iterator position);
//    size_type erase(const key_type& x);
//    void erase(iterator first, iterator last);
//    void erase(const key_type* first, const key_type* last);
//    void clear() {}
//
//
//    // find and bound
//    iterator find(const key_type& x);
//    const_iterator find(const key_type& x) const;
//    size_type count(const key_type& x) const;





}
#endif //MYSTL_STL_BTREE_H
