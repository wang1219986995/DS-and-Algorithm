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
        M_fields.parent = M_fields.parent->parent();
    }

    btree_node* rightmost() const { return M_fields.rightmost; }
    btree_node** mutable_rightmost() { return &M_fields.rightmost; }

    size_type size() const { return M_fields.size; }
    size_type* mutable_size() { return &M_fields.size; }

    const key_type& key(int i) const
    { return KeyOfValue()(M_fields.values[i]); }

    reference value(int i)
    { return M_fields.values[i]; }

    const_reference value(int i) const
    { return M_fields.values[i]; }

    mutable_value_type* mutable_value(int i)
    { return &M_fields.values[i]; }

    void value_swap(int i, btree_node* x, int j)
    {
        std::swap(mutable_value(i), x->mutable_value(j));
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

    void insert_value(int i, const value_type& x)
    {
        value_init(count(), x);
        value_init(count(), x);
        for(int j = count(); j > i; --j)
            value_swap(j, this, j - 1);

        set_count(count() + 1);

        if(!leaf())
        {
            ++i;
            for(int j = count(); j > i; --j)
            {
                *mutable_child(j) = child(j - 1);
                child(j)->set_position(j);
            }
            *mutable_child(i) = NULL;
        }
    }

    void remove_value(int i)
    {
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

    void rebalance_right_to_left(btree_node* src, int to_move)
    {
        // make room in the left
        for(int i = 0; i < to_move; ++i)
            value_init(i + count());

        value_swap(count(), parent(), position());
        parent()->value_swap(position(), src, to_move - 1);

        // move values from right to left
        for(int i = 1; i < to_move; ++i)
            value_swap(count() + i, src, i - 1);

        // shift values in the right
        for(int i = to_move; i < src->count(); ++i)
            src->value_swap(i - to_move, src, i);

        for(int i = 1; i <= to_move; ++i)
            src->value_destroy(src->count() - i);

        if(!leaf())
        {
            for(int i = 0; i < to_move; ++i)
                set_child(1 + count() + i, src->child(i));

            for(int i = 0; i <= src->count() - to_move; ++i)
            {
                src->set_child(i, src->child(i + to_move));
                *src->mutable_child(i + to_move) = NULL;
            }
        }
    }

    void rebalance_left_to_right(btree_node* dest, int to_move)
    {
        //make room in the right
        for(int i = 0; i < to_move; ++i)
            dest->value_init(i + dest->count());
        for(int i = dest->count() - 1; i >= 0; --i)
            dest->value_swap(i, dest, i + to_move);

        dest->value_swap(to_move - 1, parent(), position());
        parent()->value_swap(position(), this, count() - to_move);
        value_destroy(count() - to_move);

        // move values from left to right
        for(int i = 1; i < to_move; ++i)
        {
            value_swap(count() - to_move + i, dest, i - 1);
            value_destroy(count() - to_move + i);
        }

        if(!leaf())
        {
            for(int i = dest->count(); i >=0; --i)
            {
                dest->set_child(i + to_move, dest->child(i));
                *dest->mutable_child(i) = NULL;
            }
            for(int i = 1; i < to_move; ++i)
            {
                dest->set_child(i - 1, child(count() - to_move + i));
                *mutable_child(count() - to_move + i) = NULL;
            }
        }
    }

    void split(btree_node* sibling, int insert_position);

    void merge(btree_node* src)
    {
        value_init(count());
        value_swap(count(), parent(), position());

        // move values from the right to left
        for(int i = 0; i < src->count(); ++i)
        {
            value_init(1 + count() + i);
            value_swap(1 + count() + i, src, i);
            src->value_destroy(i);
        }

        if(!leaf())
        {
            for(int i = 0; i <= src->count(); ++i)
            {
                set_child(1 + count() + i, src->child(i));
                *src->mutable_child(i) = NULL;
            }
        }

        set_count(1 + count() + src->count());
        src->set_count(0);
        parent()->remove_value(position());
    }

    void swap(btree_node* x)
    {
        assert(leaf() == x->leaf());

        // Swap the values.
        for (int i = count(); i < x->count(); ++i) {
            value_init(i);
        }
        for (int i = x->count(); i < count(); ++i) {
            x->value_init(i);
        }
        int n = std::max(count(), x->count());
        for (int i = 0; i < n; ++i) {
            value_swap(i, x, i);
        }
        for (int i = count(); i < x->count(); ++i) {
            x->value_destroy(i);
        }
        for (int i = x->count(); i < count(); ++i) {
            value_destroy(i);
        }

        if (!leaf()) {
            // Swap the child pointers.
            for (int i = 0; i <= n; ++i) {
                btree_swap_helper(*mutable_child(i), *x->mutable_child(i));
            }
            for (int i = 0; i <= count(); ++i) {
                x->child(i)->fields_.parent = x;
            }
            for (int i = 0; i <= x->count(); ++i) {
                child(i)->fields_.parent = this;
            }
        }

        // Swap the counts.
        std::swap(M_fields.count, x->M_fields.count);
    }

    static btree_node* init_leaf(leaf_fields* f, btree_node* parent, int max_count)
    {
        btree_node* n = reinterpret_cast<btree_node*>(f);
        f->leaf = 1;
        f->position = 0;
        f->max_count = max_count;
        f->count = 0;
        f->parent = parent;
        return n;
    }
    static btree_node* init_internal(internal_fields* f, btree_node* parent)
    {
        btree_node* n = init_leaf(f, parent, kNodeValues);
        f->leaf = 0;
        return n;
    }
    static btree_node* init_root(root_fields* f, btree_node* parent)
    {
        btree_node* n = init_internal(f, parent);
        f->rightmost = parent;
        f->size = parent->count();
        return n;
    }

    void destroy()
    {
        for(int i = 0; i < count(); ++i)
            value_destroy(i);
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
    void increment_by(int count)
    {
        while(count > 0)
        {
            if(node->leaf())
            {
                int rest = node->count() - position;
                position += std::min(rest, count);
                count = count - rest;
                if(position < node->count())
                    return;
            }
            else
                --count;
            increment_slow();
        }
    }

    void increment_slow()
    {
        if(node->leaf())
        {
            self_type save(*this);
            while(position == node->count() && !node->is_root())
            {
                position = node->position();
                node = node->parent();
            }
            if(position == node->count())
                *this = save;
        }
        else
        {
            node = node->child(position + 1);
            while(!node->leaf())
                node = node->child(0);
            position;
        }
    }

    void decrement()
    {
        if(node->leaf() && --position >= 0)
            return;
        decrement_slow();
    }

    void decrement_slow()
    {
        if(node->leaf())
        {
            self_type save(*this);
            while(position < 0 && !node->is_root())
            {
                position = node->position() - 1;
                node = node->parent();
            }
            if(position < 0)
                *this = save;
        }
        else
        {
            node = node->child(position);
            while(!node->leaf())
                node = node->child(node->count());
        }
        position = node->count() - 1;
    }

    key_type& key()
    { return node->key(); }

    reference operator*() const
    {
        return node->value(position);
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
    node_type* node;
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
class btree_base
{
public:
    typedef typename Alloc_traits<Tp, Alloc>::allocator_type::template rebind<char>::other allocator_type;

    btree_base() : M_node_allocator(allocator_type()) {}
    btree_base(const allocator_type& a) : M_node_allocator(a) {}
    allocator_type get_allocator() const { return M_node_allocator; }

public:
    allocator_type M_node_allocator;
};





template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc=allocator<Value>, int TargetNodeSize=256>
class btree : protected btree_base<Value, Alloc, Alloc_traits<Value, Alloc>::S_instanceless>
{
public:
    typedef btree<Key, Value, KeyOfValue, Compare, Alloc, TargetNodeSize> self_type;
    typedef btree_base<Value, Alloc, Alloc_traits<Value, Alloc>::S_instanceless> Base;
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

    allocator_type get_allocator() const { return Base::get_allocator(); }

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
                int to_move = (right->count() - iter->node->count()) / 2;
                to_move = std::min(to_move, right->count() - 1);
                iter->node->rebalance_right_to_left(right, to_move);
                return false;
            }
        }

        if(iter->node->position() > 0)
        {
            node_type* left = parent->child(iter->node->position() - 1);
            if((left->count() > kMinNodeValues) &&
                    (iter->node->count() == 0 || iter->position < iter->node->count()))
            {
                int to_move = (left->count() - iter->node->count()) / 2;
                to_move = std::min(to_move, left->count() - 1);
                left->rebalance_right_to_right(iter->node, to_move);
                iter->position += to_move;
                return false;
            }
        }
        return true;
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
    else if(!root()->leaf()) //TODO 这里是为什么？
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


    if(res.position == res.node->count())
    {
        res.position = res.node->count() - 1;
        ++res;
    }

    if(internal_delete) ++res;
    return res;
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
