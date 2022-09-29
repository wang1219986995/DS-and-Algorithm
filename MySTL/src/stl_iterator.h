//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_ITERATOR_H
#define MYSTL_STL_ITERATOR_H

#include "stl_iterator_base.h"


namespace MySTL{

template <class Container>
class back_insert_iterator{
protected:
    Container* container;
public:
    typedef Container               container_type;
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;

    explicit back_insert_iterator(Container& x) : container(&x){}
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
            { container->push_back(value);};
    back_insert_iterator<Container>& operator*() {return *this;}
    back_insert_iterator<Container>& operator++() {return *this;}
    back_insert_iterator<Container>& operator++(int) {return *this;}
};


template <class Container>
inline output_iterator_tag iterator_category(const back_insert_iterator<Container>&)
{   return output_iterator_tag();   }

template <class Container>
inline back_insert_iterator<Container> back_insert(Container& x)
{   return back_insert_iterator<Container>(x);  }


template <class Container>
class front_insert_iterator{
protected:
    Container* container;
public:
    typedef Container           container_type;
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;


    explicit front_insert_iterator(Container& x) : container(&x) {}
    front_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->push_front(value);
        return *this;
    }

    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container>& operator++(int) { return *this; }
};

template <class Container>
inline output_iterator_tag iterator_category(const front_insert_iterator<Container>&)
{ return output_iterator_tag(); }

template <class Container>
inline front_insert_iterator<Container> front_inserter(Container& x) {
    return front_insert_iterator<Container>(x);
}


template <class Container>
class insert_iterator{
protected:
        Container* container;
        typename Container::iterator iter;
public:
    typedef Container           container_type;
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    insert_iterator(Container& x, typename Container::iterator i)
    : container(&x), iter(i) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};

template <class Container>
inline output_iterator_tag iterator_category(const insert_iterator<Container>&)
{
    return output_iterator_tag();
}


template <class Container, class Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator i)
{
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(x, iter(i));
}


template <class BidirectionalIterator, class Tp, class Reference = Tp&,
            class Distance = ptrdiff_t>
class reverse_bidirectional_iterator{
        typedef reverse_bidirectional_iterator<BidirectionalIterator,
                                        Tp, Reference, Distance> Self;

protected:
        BidirectionalIterator current;
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef Tp                         value_type;
    typedef Distance                   difference_type;
    typedef Tp*                        pointer;
    typedef Reference                  reference;

    reverse_bidirectional_iterator() {}
    explicit reverse_bidirectional_iterator(BidirectionalIterator x)
    : current(x) {}
    BidirectionalIterator base() const { return current; }
    Reference operator*() const
    {
        BidirectionalIterator tmp = current;
        return *--tmp;
    }

    Self& operator++()
    {
        --current;
        return *this;
    }

    Self operator++(int)
    {
        Self tmp = *this;
        --current;
        return tmp;
    }

    Self& operator--()
    {
        ++current;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp = *this;
        ++current;
        return tmp;
    }
 };

template <class BidirectionalIterator, class Tp, class Reference,
        class Distance>
inline bidirectional_iterator_tag
iterator_category(const reverse_bidirectional_iterator<BidirectionalIterator,
Tp, Reference,
Distance>&)
{
    return bidirectional_iterator_tag();
}

template <class BidirectionalIterator, class Tp, class Reference,
        class Distance>
inline Tp*
value_type(const reverse_bidirectional_iterator<BidirectionalIterator, Tp,
Reference, Distance>&)
{
    return (Tp*) 0;
}

template <class BidirectionalIterator, class Tp, class Reference,
        class Distance>
inline Distance*
distance_type(const reverse_bidirectional_iterator<BidirectionalIterator,
Tp,
Reference, Distance>&)
{
    return (Distance*) 0;
}

template <class BiIter, class Tp, class Ref, class Distance>
inline bool operator==(
        const reverse_bidirectional_iterator<BiIter, Tp, Ref, Distance>& x,
        const reverse_bidirectional_iterator<BiIter, Tp, Ref, Distance>& y)
{
    return x.base() == y.base();
}

template <class BiIter, class Tp, class Ref, class Distance>
inline bool operator!=(
        const reverse_bidirectional_iterator<BiIter, Tp, Ref, Distance>& x,
        const reverse_bidirectional_iterator<BiIter, Tp, Ref, Distance>& y)
{
    return !(x == y);
}




template <class Iterator>
class reverse_iterator
{
protected:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::reference           reference;

    typedef Iterator                        iterator_type;
    typedef reverse_iterator<Iterator>      Self;

public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    reverse_iterator(const Self& x) : current(x.current) {}

    template<class Iter>
    reverse_iterator(const reverse_iterator<Iter>& x) : current(x.base()) {}

    iterator_type base() const { return current; }

    reference operator*() const
    {
        Iterator tmp = current;
        return *--tmp;
    }

    Self& operator++()
    {
        --current;
        return *this;
    }

    Self& operator++(int)
    {
        Self tmp = *this;
        --current;
        return tmp;
    }

    Self& operator--()
    {
        ++current;
        return *this;
    }

    Self& operator--(int)
    {
        Self tmp = *this;
        ++current;
        return tmp;
    }

    Self operator+(difference_type n) const
    {
        return Self(current - n);
    }

    Self& operator+=(difference_type n)
    {
        current -= n;
        return *this;
    }

    Self operator-(difference_type n) const
    {
        return Self(current + n);
    }

    Self& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    reference operator[](difference_type n) const { return *(*this + n); }
};

template <class Iterator>
inline bool operator==(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return x.base() == y.base();
}


template <class Iterator>
inline bool operator<(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return x.base() < y.base();
}


template <class Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return !(x == y);
}


template <class Iterator>
inline bool operator>(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return y < x;
}

template <class Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return !(y < x);
}


template <class Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return !(x < y);
}

template <class Iterator>
inline typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
{
    return y.base() - x.base();
}

template <class Iterator>
inline reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n,
          const reverse_iterator<Iterator>& x)
{
    return reverse_iterator<Iterator>(x.base() -n );
}




//TODO:  iostream iterator



}
#endif //MYSTL_STL_ITERATOR_H
