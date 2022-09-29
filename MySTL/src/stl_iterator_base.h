//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_ITERATOR_BASE_H
#define MYSTL_STL_ITERATOR_BASE_H

#include <stdio.h>

namespace MySTL {


struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<class Tp, class Distance>
struct input_iterator {
    typedef input_iterator_tag  iterator_category;
    typedef Tp                  value_type;
    typedef Distance            difference_type;
    typedef Tp*                 pointer;
    typedef Tp &                reference;
};

struct output_iterator {
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;
};

template<class Tp, class Distance>
struct forward_iterator {
    typedef forward_iterator_tag    iterator_category;
    typedef Tp                      value_type;
    typedef Distance                difference_type;
    typedef Tp *                    pointer;
    typedef Tp &                    reference;
};


template<class _Tp, class _Distance>
struct bidirectional_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};

template<class _Tp, class _Distance>
struct random_access_iterator {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};


// iterator_traits
template<class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};
template <class _Tp>
struct iterator_traits<_Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

template <class _Tp>
struct iterator_traits<const _Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp*                  pointer;
    typedef const _Tp&                  reference;
};

template <class Iter>
inline typename iterator_traits<Iter>::iterator_category __iterator_category(const Iter&)
{
    typedef typename iterator_traits<Iter>::iterator_category Category;
    return Category();
}

template <class Iter>
inline typename iterator_traits<Iter>::iterator_category iterator_category(const Iter& i)
{ return MySTL::__iterator_category(i); }

template <class Iter>
inline typename iterator_traits<Iter>::difference_type* __distance_type(const Iter&)
{ return static_cast<typename iterator_traits<Iter>::difference_type*>(0); }

template <class Iter>
inline typename iterator_traits<Iter>::difference_type* distance_type(const Iter& __i)
{ return __distance_type(__i); }

template <class Iter>
inline typename iterator_traits<Iter>::value_type* __value_type(const Iter&)
{ return static_cast<typename iterator_traits<Iter>::value_type*>(0); }

template <class Iter>
inline typename iterator_traits<Iter>::value_type* value_type(const Iter& __i)
{ return __value_type(__i); }

#define ITERATOR_CATEGORY(iter) MySTL::__iterator_category(iter)
#define DISTANCE_TYPE(iter) MySTL::__distance_type(iter)
#define VALUE_TYPE(iter) MySTL::__value_type(iter)


template <class InputIterator, class Distance>
inline void __distance(InputIterator first, InputIterator last,
                       Distance& n, input_iterator_tag)
{ while(first != last) { ++first; ++n;} }

template <class RandomAccessIterator, class Distance>
inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
                       Distance& n, random_access_iterator_tag)
{ n += last - first; }

template <class InputIterator, class Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n)
{ __distance(first, last, n, iterator_category(first)); }

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while(first != last)
    {
        ++first; ++n;
    }
    return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator)
{ return last - first; }

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category Category;
    return __distance(first, last, Category());
}

template <class BidirectionIterator, class Distance>
inline void __advance(BidirectionIterator& i, Distance n, bidirectional_iterator_tag)
{
    if(n > 0)   while(n--) ++i;
    else        while(n++) --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
{ i += n; }

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n)
{ __advance(i, n, iterator_category(i)); }



} // namespace




#endif //MYSTL_STL_ITERATOR_BASE_H
