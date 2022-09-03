//
// Created by GW00243464 on 2022/9/3.
//

#ifndef MYSTL_STL_ALGO_H
#define MYSTL_STL_ALGO_H
#include "stl_algobase.h"
namespace MySTL
{
template <class Tp>
inline const Tp& median(const Tp& a, const Tp& b, const Tp& c)
{
    if (a < b)
        if (b < c)
            return b;
        else if (a < c)
            return c;
        else
            return a;
    else if (a < c)
        return a;
    else if (b < c)
        return c;
    else
        return b;
}


template <class Tp, class Compare>
inline const Tp& median(const Tp& a, const Tp& b, const Tp&  c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

// for each
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f)
{
    for(; first != last; ++first)
        f(*first);
    return f;
}

// find and find_if
template <class InputIter, class Tp>
inline InputIter find(InputIter first, InputIter last, const Tp& val, input_iterator_tag)
{
    while(first != last && !(*first == val))
        ++first;
    return first;
}


template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred, input_iterator_tag)
{
    while(first != last && !pred(*first))
        ++first;
    return first;
}

template <class InputIter, class Tp>
inline InputIter find(InputIter first, InputIter last, const Tp& val)
{
    return find(first, last, val, ITERATOR_CATEGORY(first));
}


template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return find_if(first, last, pred, ITERATOR_CATEGORY(first)) ;
}

// adjacent_find
template <class ForwareIter>
ForwareIter adjacent_find(ForwareIter first, ForwareIter last)
{
    if(first == last)
        return last;
    ForwareIter next = first;
    while(++next != last)
    {
        if(*first == *next)
            return first;
        first = next;
    }
    return last;
}


template <class ForwareIter, class Predicate>
ForwareIter adjacent_find(ForwareIter first, ForwareIter last, Predicate pred)
{
    if(first == last)
        return last;
    ForwareIter next = first;
    while(++next != last)
    {
        if(pred(*first, *next))
            return first;
        first = next;
    }
    return last;
}


//count and count_if
template <class InputIter, class Tp, class Size>
void count (InputIter first, InputIter last, const Tp& value, Size& n)
{
    for(; first != last; ++first)
        if(*first == value)
            ++n;
}

template <class InputIter, class Predicate, class Size>
void count_if (InputIter first, InputIter last, Predicate pred, Size& n)
{
    for(; first != last; ++first)
        if(pred(*first))
            ++n;
}

// search
template <class ForwareIter1, class ForwareIter2>
ForwareIter1 search(ForwareIter1 first1, ForwareIter1 last1,
                    ForwareIter2 first2, ForwareIter2 last2)
{
    if(first1 == last1 || first2 == last2) return first1;

    ForwareIter2 tmp(first2);
    ++tmp;
    if(tmp == last2)    return find(first1, last1, *first2);

    ForwareIter2 p1, p;
    p1 = first2; ++ p1;
    ForwareIter1 current = first1;
    while(first1 != last1)
    {
        first1 = find(first1, last1, *first2);
        if(first1 == last1) return last1;

        p = p1;
        current = first1;
        if(++current == last1)
            return last1;

        while(*current == *p)
        {
            if(++p == last2)
                return first1;
            if(++current == last1)
                return last1;
        }
        ++first1;
    }
    return first1;
}


template <class ForwardIter1, class ForwardIter2, class Predicate>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                     ForwardIter2 first2, ForwardIter2 last2,
                     Predicate  pred)
{
    // Test for empty ranges
    if (first1 == last1 || first2 == last2)
        return first1;

    // Test for a pattern of length 1.
    ForwardIter2 __tmp(first2);
    ++__tmp;
    if (__tmp == last2) {
        while (first1 != last1 && !pred(*first1, *first2))
            ++first1;
        return first1;
    }

    // General case.
    ForwardIter2 __p1, __p;

    __p1 = first2; ++__p1;

    ForwardIter1 __current = first1;

    while (first1 != last1) {
        while (first1 != last1) {
            if (pred(*first1, *first2))
                break;
            ++first1;
        }
        while (first1 != last1 && !pred(*first1, *first2))
            ++first1;
        if (first1 == last1)
            return last1;

        __p = __p1;
        __current = first1;
        if (++__current == last1) return last1;

        while (pred(*__current, *__p)) {
            if (++__p == last2)
                return first1;
            if (++__current == last1)
                return last1;
        }
        ++first1;
    }
    return first1;
}


// search_n
template <class ForwardIter, class Integer, class Tp>
ForwardIter search_n(ForwardIter first, ForwardIter last, Integer count, Tp& val)
{
    if(count <= 0 )
        return first;
    else
    {
        first = find(first, last, val);
        while(first != last)
        {
            Integer n = count - 1;
            ForwardIter i = first;
            ++i;
            while(i != last && n != 0 && *i == val)
                ++i, --n;
            if(n == 0)
                return first;
            else
                first = find(i, last, val);
        }
        return last;
    }
}

template <class ForwardIter, class Integer, class Tp, class Predicate>
ForwardIter search_n(ForwardIter first, ForwardIter last, Integer count, Tp& val, Predicate pred)
{
    if(count <= 0 )
        return first;
    else
    {
        first = find_if(first, last, pred, ITERATOR_CATEGORY(first));
        while(first != last)
        {
            Integer n = count - 1;
            ForwardIter i = first;
            ++i;
            while(i != last && n != 0 && pred(*i, val))
                ++i, --n;
            if(n == 0)
                return first;
            else
                first = find(i, last, val);
        }
        return last;
    }
}


// swap_ranges

template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_ranges(ForwardIter1 first1, ForwardIter1 last2, ForwardIter2 first2)
{
    for(; first1 != last2; ++first1, ++first2)
        iter_swap(first1, first2);
    return first2;
}

// transform
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter first, InputIter last, OutputIter result, UnaryOperation opr)
{
    for(; first != last; ++first, ++result)
        *result = opr(*first);
    return result;
}


template <class InputIter1, class InputIter2, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, OutputIter result, UnaryOperation opr)
{
    for(; first1 != last1; ++first1, ++first2, ++result)
        *result = opr(*first1, *first2);
    return result;
}

// replace, replace_if, replace_copy, replace_copy_if
template <class ForwardIter, class Tp>
void replace(ForwardIter first, ForwardIter last, const Tp& old_value, const Tp& new_value)
{
    for(; first != last; ++first)
        if(*first == old_value)
            *first = new_value;
}


template <class ForwardIter, class Predicate, class Tp>
void replace_if(ForwardIter first, ForwardIter last, Predicate pred, const Tp& old_value, const Tp& new_value)
{
    for(; first != last; ++first)
        if(pred(*first, old_value))
            *first = new_value;
}

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter replace_copy(_InputIter __first, _InputIter __last, _OutputIter __result,
                         const _Tp& __old_value, const _Tp& __new_value)
{
    for ( ; __first != __last; ++__first, ++__result)
        *__result = *__first == __old_value ? __new_value : *__first;
    return __result;
}

template <class _InputIter, class _OutputIter, class _Predicate, class _Tp>
_OutputIter replace_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result,
                            _Predicate __pred, const _Tp& __new_value)
{
    for ( ; __first != __last; ++__first, ++__result)
        *__result = __pred(*__first) ? __new_value : *__first;
    return __result;
}


// generate and generate_n

template <class _ForwardIter, class _Generator>
void generate(_ForwardIter __first, _ForwardIter __last, _Generator __gen) {
    for ( ; __first != __last; ++__first)
        *__first = __gen();
}

template <class _OutputIter, class _Size, class _Generator>
_OutputIter generate_n(_OutputIter __first, _Size __n, _Generator __gen) {
    for ( ; __n > 0; --__n, ++__first)
        *__first = __gen();
    return __first;
}


// remove , remove_if, remove_copy, remove_copy_if

template <class InputIter, class OutputIter, class Tp>
OutputIter remove_copy(InputIter first, InputIter last, OutputIter result, const Tp& value)
{
    for(; first != last; ++ first)
    {
        if(!(*first == value))
        {
            *result = *first;
            ++result;
        }
    }
    return result;
}

template <class InputIter, class OutputIter, class Predicate>
OutputIter remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred)
{
    for(; first != last; ++ first)
    {
        if(!pred(*first))
        {
            *result = *first;
            ++result;
        }
    }
    return result;
}

template <class ForwardIter, class Tp>
ForwardIter remove(ForwardIter first, ForwardIter last, const Tp& value)
{
    first = find(first, last, value);
    ForwardIter i = first;
    //TODO: 这里的写法有问题
    return first == last ? first : remove_copy(++i, last, first, value);
}


template <class ForwardIter, class Predicate>
ForwardIter remove(ForwardIter first, ForwardIter last, Predicate pred)
{
    first = find(first, last, pred);
    ForwardIter i = first;
    //TODO: 这里的写法有问题
    return first == last ? first : remove_copy_if(++i, last, first, pred);
}


// unique and unique_copy
//TODO: 待定


// reverse and reverse_copy, and their auxiliary functions
template <class BidirectionalIter>
void __reverse(BidirectionalIter first, BidirectionalIter last, bidirectional_iterator_tag)
{
    while(true)
        if(first == last || first == --last)
            return;
        else
            iter_swap(first++, --last);
}

template <class BidirectionalIter>
inline void reverse(BidirectionalIter first, BidirectionalIter last)
{
    __reverse(first, last, ITERATOR_CATEGORY(first));
}


template <class BidirectionalIter, class OutputIter>
inline void reverse_copy(BidirectionalIter first, BidirectionalIter last, OutputIter result)
{
    while(first != last)
    {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}


// rotate and totate_copy, and their auxiliary functions
template <class EculideanRingElement>
EculideanRingElement __gcd(EculideanRingElement m, EculideanRingElement n)
{
    while(n != 0)
    {
        EculideanRingElement t = m % n;
        m = n;
        n = t;
    }
}


template <class ForwardIter, class Distance>
ForwardIter __rotate(ForwardIter first, ForwardIter middle, ForwardIter last, Distance*, forward_iterator_tag)
{
    if(first == middle)
        return last;
    if(last == middle)
        return first;

    ForwardIter first2 = middle;
    do
    {
        swap(*first++, *first2++);
        if(first == middle)
            middle = first2;
    }while(first2 != last);

    ForwardIter new_middle = first;

    first2 = middle;
    while(first2 != last)
    {
        swap(*first++,*first2++);
        if(first == middle)
            middle = first2;
        else if(first2 = last)
            first2 = middle;
    }
    return new_middle;
}

template <class BidirectionalIter, class Distance>
BidirectionalIter __rotate(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                           Distance, bidirectional_iterator_tag)
{
    if(first == middle)
        return last;
    if(last == middle)
        return first;

    __reverse(first, middle, bidirectional_iterator_tag());
    __reverse(middle, last, bidirectional_iterator_tag());

    while(first != middle && middle != last)
        swap(*first++, *--last);

    if(first == middle)
    {
        __reverse(middle, last);
        return last;
    }
    else
    {
        __reverse(first, middle, bidirectional_iterator_tag());
        return first;
    }
}

template <class RandomAccessIter, class Distance, class Tp>
RandomAccessIter __rotate(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Distance*, Tp*)
{

}

template <class ForwardIter, class OutputIter>
OutputIter rotate_copy(ForwardIter first, ForwardIter middle, ForwardIter last, OutputIter result)
{
    return copy(first, middle, copy(middle, last, result));
}



}






















#endif //MYSTL_STL_ALGO_H
