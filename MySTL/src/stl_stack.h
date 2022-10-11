//
#ifndef MYSTL_STL_STACK_H
#define MYSTL_STL_STACK_H

#include "stl_deque.h"

namespace MySTL
{

template <class Tp, class Sequence = deque<Tp>>
class stack;

template <class Tp, class Seq>
bool operator==(const stack<Tp, Seq>& x, const stack<Tp, Seq>& y);

template <class Tp, class Seq>
bool operator<(const stack<Tp, Seq>& x, const stack<Tp, Seq>& y);


template <class Tp, class Sequence>
class stack
{
    template<class Tp1, class Seq1>
    friend bool operator==(const stack<Tp1, Seq1>&, const stack<Tp1, Seq1>&);

    template<class Tp1, class Seq1>
    friend bool operator<(const stack<Tp1, Seq1>&, const stack<Tp1, Seq1>&);

public:
    typedef typename Sequence::value_type       value_type;
    typedef typename Sequence::size_type        size_type;
    typedef Sequence                            container_type;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::const_reference  const_reference;
protected:
    Sequence c;
public:
    stack() : c() {}
    explicit stack(const Sequence& s) : c(s) {}

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};

template <class Tp, class Seq>
bool operator==(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
    return x.c == y.c;
}

template <class Tp, class Seq>
bool operator<(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
    return x.c < y.c;
}

template <class Tp, class Seq>
bool operator!=(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
  return !(x == y);
}

template <class Tp, class Seq>
bool operator>(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
  return y < x;
}

template <class Tp, class Seq>
bool operator<=(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
  return !(y < x);
}

template <class Tp, class Seq>
bool operator>=(const stack<Tp,Seq>& x, const stack<Tp,Seq>& y)
{
  return !(x < y);
}





} // namespace
#endif //MYSTL_STL_STACK_H
