//

#ifndef MYSTL_STL_QUEUE_H
#define MYSTL_STL_QUEUE_H
#include "stl_deque.h"
#include "stl_vector.h"
#include "stl_heap.h"

#include <functional>

namespace MySTL
{

template <class Tp, class Sequence = deque<Tp>>
class queue;

template <class Tp, class Seq>
inline bool operator==(const queue<Tp, Seq>&, const queue<Tp, Seq>&);

template <class Tp, class Seq>
inline bool operator<(const queue<Tp, Seq>&, const queue<Tp, Seq>&);

template <class Tp, class Sequence>
class queue
{
    typedef typename Sequence::value_type Sequence_value_type;

    template<class Tp1, class Seq1>
    friend bool operator==(const queue<Tp1, Seq1>&, const queue<Tp1, Seq1>&);

    template<class Tp1, class Seq1>
    friend bool operator<(const queue<Tp1, Seq1>&, const queue<Tp1, Seq1>&);

public:
    typedef typename Sequence::value_type           value_type;
    typedef typename Sequence::size_type            size_type;
    typedef Sequence                                container_type;
    typedef typename Sequence::reference            reference;
    typedef typename Sequence::const_reference      const_reference;

protected:
    Sequence c;

public:
    queue() : c() {}
    explicit queue(const Sequence& s) : c(s) {}

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

template <class Tp, class Sequence>
bool operator==(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return x.c == y.c; }

template <class Tp, class Sequence>
bool operator<(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return x.c_y.c; }

template <class Tp, class Sequence>
bool operator!=(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return !(x == y); }

template <class Tp, class Sequence>
bool operator>(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return y < x; }

template <class Tp, class Sequence>
bool operator<=(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return !(y < x); }

template <class Tp, class Sequence>
bool operator>=(const queue<Tp, Sequence>& x, const queue<Tp, Sequence>& y)
{ return !(x < y); }



// priority_queue
template <class Tp, class Sequence = vector<Tp>,
        class Compare = std::less<typename Sequence::value_type>>
class priority_queue
{
    typedef typename Sequence::value_type Sequence_value_type;
public:
    typedef typename Sequence::value_type           value_type;
    typedef typename Sequence::size_type            size_type;
    typedef Sequence                                container_type;
    typedef typename Sequence::reference            reference;
    typedef typename Sequence::const_reference      const_reference;

protected:
    Sequence c;
    Compare comp;

public:
    priority_queue() : c() {}

    explicit priority_queue(const Compare& x) : c(), comp(x) {}

    priority_queue(const Compare& x, const Sequence& s) : c(s), comp(x)
    { make_heap(c.begin(), c.end(), comp); }

    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& x)
    : c(first, last), comp(x)
    { make_heap(c.begin(), c.end(), comp); }

    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& x, const Sequence& s)
    : c(s), comp(x)
    {
        c.insert(c.end(), first, last);
        make_heap(c.begin(), c.end(), comp);
    }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type& x)
    {
        try
        {
            c.push_back(x);
            push_heap(c.begin(), c.end(), comp);
        }
        catch (...)
        {
            c.clear();
            throw;
        }
    }

    void pop()
    {
        try
        {
            pop_heap(c.begin(), c.end());
            c.pop_back();
        }
        catch (...)
        {
            c.clear();
            throw;
        }
    }

};



} // namespace
#endif //MYSTL_STL_QUEUE_H
