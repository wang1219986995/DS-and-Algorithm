//

#ifndef MYSTL_STL_ALGO_H
#define MYSTL_STL_ALGO_H
#include <stdlib.h>
#include "stl_algobase.h"
#include "stl_tempbuf.h"
#include "stl_heap.h"

namespace MySTL
{

// forward declaration
template <class BidirectionalIter, class Distance>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                            Distance len1, Distance len2);

template <class BidirectionalIter, class Distance, class Compare>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                            Distance len1, Distance len2, Compare comp);

template <class BidirectioanalIter, class Distance, class Pointer>
void __merge_adaptive(BidirectioanalIter first, BidirectioanalIter middle, BidirectioanalIter last,
                      Distance len1, Distance len2, Pointer buffer, Distance buffer_size);

template <class BidirectioanalIter, class Distance, class Pointer>
void __merge_adaptive(BidirectioanalIter first, BidirectioanalIter middle, BidirectioanalIter last,
                      Distance len1, Distance len2, Pointer buffer, Distance buffer_size);

template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result);

template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp);

template <class BidirectionalIter, class Distance>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                            Distance len1, Distance len2);

template <class BidirectionalIter, class Distance, class Compare>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                                Distance len1, Distance len2, Compare comp);
// forward declaration end

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


template <class RandomAccessIter, class Tp>
RandomAccessIter find(RandomAccessIter first, RandomAccessIter last,
                       const Tp& val, random_access_iterator_tag)
{
    typename iterator_traits<RandomAccessIter>::difference_type trip_count
            = (last - first) >> 2;

    for ( ; trip_count > 0 ; --trip_count) 
    {
        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;

        if (*first == val) return first;
        ++first;
    }

    switch(last - first) 
    {
        case 3:
            if (*first == val) return first;
            ++first;
        case 2:
            if (*first == val) return first;
            ++first;
        case 1:
            if (*first == val) return first;
            ++first;
        case 0:
        default:
            return last;
    }
}

template <class RandomAccessIter, class Predicate>
RandomAccessIter find_if(RandomAccessIter first, RandomAccessIter last,
                          Predicate pred, random_access_iterator_tag)
{
    typename iterator_traits<RandomAccessIter>::difference_type trip_count
                = (last - first) >> 2;

    for ( ; trip_count > 0 ; --trip_count) 
    {
        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;
    }

    switch(last - first) 
    {
        case 3:
            if (pred(*first)) return first;
            ++first;
        case 2:
            if (pred(*first)) return first;
            ++first;
        case 1:
            if (pred(*first)) return first;
            ++first;
        case 0:
        default:
            return last;
    }
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

template <class InputIter, class Tp>
typename iterator_traits<InputIter>::difference_type
count(InputIter first, InputIter last, const Tp& value)
{
    typename iterator_traits<InputIter>::difference_type n = 0;
    for(; first != last; ++ first)
        if(*first == value)
            ++n;
    return n;
}

template <class InputIter, class Predicate>
typename iterator_traits<InputIter>::difference_type
count_if(InputIter first, InputIter last, Predicate pred)
{
    typename iterator_traits<InputIter>::difference_type n = 0;
    for(; first != last; ++first)
        if(pred(*first))
            ++n;
    return n;
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
    ForwardIter2 tmp(first2);
    ++tmp;
    if (tmp == last2)
    {
        while (first1 != last1 && !pred(*first1, *first2))
            ++first1;
        return first1;
    }

    // General case.
    ForwardIter2 p1, p;
    p1 = first2; ++p1;
    ForwardIter1 __current = first1;

    while (first1 != last1)
    {
        while (first1 != last1)
        {
            if (pred(*first1, *first2))
                break;
            ++first1;
        }
        while (first1 != last1 && !pred(*first1, *first2))
            ++first1;
        if (first1 == last1)
            return last1;

        p = p1;
        __current = first1;
        if (++__current == last1) return last1;

        while (pred(*__current, *p))
        {
            if (++p == last2)
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

template <class InputIter, class OutputIter, class Tp>
OutputIter replace_copy(InputIter first, InputIter last, OutputIter result,
                         const Tp& old_value, const Tp& new_value)
{
    for ( ; first != last; ++first, ++result)
        *result = *first == old_value ? new_value : *first;
    return result;
}

template <class InputIter, class OutputIter, class Predicate, class Tp>
OutputIter replace_copy_if(InputIter first, InputIter last, OutputIter result,
                            Predicate pred, const Tp& new_value)
{
    for ( ; first != last; ++first, ++result)
        *result = pred(*first) ? new_value : *first;
    return result;
}


// generate and generate_n
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen) {
    for ( ; first != last; ++first)
        *first = gen();
}

template <class OutputIter, class Size, class Generator>
OutputIter generaten(OutputIter first, Size n, Generator gen) {
    for ( ; n > 0; --n, ++first)
        *first = gen();
    return first;
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
    return first == last ? first : remove_copy(++i, last, first, value);
}


template <class ForwardIter, class Predicate>
ForwardIter remove(ForwardIter first, ForwardIter last, Predicate pred)
{
    first = find_if(first, last, pred);
    ForwardIter i = first;
    return first == last ? first : remove_copy_if(++i, last, first, pred);
}


// unique and unique_copy
template <class InputIter, class OutputIter, class Tp>
OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, Tp*)
{
    Tp value = *first;
    *result = value;
    while(++first != last)
    {
        if(!(value == *first))
        {
            value = *first;
            *++result = value;
        }
    }
    return ++result;
}

template <class InputIter, class OutputIter>
inline OutputIter unique_copy(InputIter first, InputIter last, OutputIter result, output_iterator_tag)
{
    return __unique_copy(first, last, result, VALUE_TYPE(first));
}

template <class InputIter, class ForwardIter>
ForwardIter __unique_copy(InputIter first, InputIter last, ForwardIter result, forward_iterator_tag)
{
    *result = *first;
    while(++first != last)
        if(!(*result == *first))
            *++result = *first;
    return ++result;
}

template <class InputIter, class OutputIter>
inline OutputIter unique_copy(InputIter first, InputIter last, OutputIter result)
{
    if(first == last) return result;
    return __unique_copy(first, last, result, ITERATOR_CATEGORY(result));
}

template <class InputIter, class OutputIter, class BinaryPredicate, class Tp>
OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result,
                              BinaryPredicate binary_pred, Tp*)
{
    Tp value = *first;
    *result = value;
    while (++first != last)
        if (!binary_pred(value, *first))
        {
            value = *first;
            *++result = value;
        }
    return ++result;
}

template <class InputIter, class OutputIter, class BinaryPredicate>
inline OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result,
                                     BinaryPredicate binary_pred, output_iterator_tag)
{
    return __unique_copy(first, last, result, binary_pred, VALUE_TYPE(first));
}

template <class InputIter, class _ForwardIter, class BinaryPredicate>
_ForwardIter __unique_copy(InputIter first, InputIter last,
                           _ForwardIter result,
                           BinaryPredicate binary_pred,
                           forward_iterator_tag) 
{
    *result = *first;
    while (++first != last)
        if (!binary_pred(*result, *first)) *++result = *first;
    return ++result;
}

template <class InputIter, class OutputIter, class BinaryPredicate>
inline OutputIter unique_copy(InputIter first, InputIter last,
                               OutputIter result,
                               BinaryPredicate binary_pred) 
{
    if (first == last) return result;
    return __unique_copy(first, last, result, binary_pred, ITERATOR_CATEGORY(result));
}


template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last)
{
    first = adjacent_find(first, last);
    return unique_copy(first, last, first);
}

template <class ForwardIter, class BinaryPredicate>
ForwardIter unique(ForwardIter first, ForwardIter last, BinaryPredicate binary_pred)
{
    first = adjacent_find(first, last, binary_pred);
    return unique_copy(first, last, first, binary_pred);
}

// reverse and reverse_copy, and their auxiliary functions
template <class BidirectionalIter>
void __reverse(BidirectionalIter first, BidirectionalIter last, bidirectional_iterator_tag)
{
    while(true)
        if(first == last || first == --last)
            return;
        else
            iter_swap(first++, last);
}

template <class RandomAccessIter>
void __reverse(RandomAccessIter first, RandomAccessIter last, random_access_iterator_tag)
{
    while(first < last)
        iter_swap(first ++ , --last);
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
    return m;
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
        __reverse(middle, last, bidirectional_iterator_tag());
        return last;
    }
    else
    {
        __reverse(first, middle, bidirectional_iterator_tag());
        return first;
    }
}

template <class RandomAccessIter, class Distance, class Tp>
RandomAccessIter __rotate(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last,
                          Distance *, Tp *)
{
    Distance n = last - first;
    Distance k = middle - first;
    Distance l = n - k;
    RandomAccessIter result = first + (last - middle);

    if(k == 0)
        return last;
    else if(k == l)
    {
        swap_ranges(first, middle, middle);
        return result;
    }

    Distance d = __gcd(n, k);

    for(Distance i = 0; i < d; ++i)
    {
        Tp tmp = *first;
        RandomAccessIter p = first;

        if(k < l)
        {
            for(Distance j = 0; j < l/d; j++)
            {
                if(p > first + l)
                {
                    *p = *(p - l);
                    p -= l;
                }

                *p = *(p + k);
                p += k;
            }
        }
        else
        {
            for(Distance j = 0; j < k/d - 1; j++)
            {
                if(p < last - k)
                {
                    *p = *(p + k);
                    p += p;
                }

                *p = *(p + k);
                p -= l;
            }
        }
        *p = tmp;
        ++first;
    }
    return result;
}



template <class RandomAccessIter>
RandomAccessIter rotate(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last)
{
    return __rotate(first, middle, last, DISTANCE_TYPE(first), ITERATOR_CATEGORY(first));
}

template <class ForwardIter, class OutputIter>
OutputIter rotate_copy(ForwardIter first, ForwardIter middle, ForwardIter last, OutputIter result)
{
    return copy(first, middle, copy(middle, last, result));
}

// random number
template <class Distance>
inline Distance random_number(Distance n)
{
    return rand() % n;
}

// random_shuffle
template <class RandomAccessIter>
inline void random_shuffle(RandomAccessIter first, RandomAccessIter last)
{
    if(first == last)   return;
    for(RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + random_number((i-first) + 1));
}

template <class RandomAccessIter, class RandomNumberGenerator>
void random_shuffle(RandomAccessIter first, RandomAccessIter last, RandomNumberGenerator& rand)
{
    if(first == last)   return;
    for(RandomAccessIter i = first + 1; i < last; ++i)
        iter_swap(i, first + rand((i-first) + 1));
}

// random_sample and random_sample_n
template <class ForwardIter, class OutputIter, class Distance>
OutputIter random_sample_n(ForwardIter first, ForwardIter last,
                           OutputIter out, const Distance n)
{
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n ,remaining);
    while(m > 0)
    {
        if(random_number(remaining) < m)
        {
            *out = *first;
            ++out;
            --m;
        }

        --remaining;
        ++first;
    }
    return out;
}





template <class ForwardIter, class OutputIter, class Distance, class RandomNumberGenerator>
OutputIter random_sample_n(ForwardIter first, ForwardIter last,
                           OutputIter out, const Distance n, RandomNumberGenerator& rand)
{
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n ,remaining);

    while(m > 0)
    {
        if(rand(remaining) < m)
        {
            *out = *first;
            ++out;
            --m;
        }

        --remaining;
        ++first;
    }
    return out;
}


template <class InputIter, class RandomAccessIter, class Distance>
RandomAccessIter __random_sample(InputIter first, InputIter last, RandomAccessIter out, const Distance n)
{
    Distance m = 0;
    Distance t = n;

    for(; first != last && m < n; ++m, ++first)
        out[m] = *first;

    while(first != last)
    {
        ++t;
        Distance tmp = random_number(t);
        if(tmp < n)
            out[m] = *first;
        ++first;
    }
    return out+m;
}


template <class _InputIter, class _RandomAccessIter, class _RandomNumberGenerator, class _Distance>
_RandomAccessIter __random_sample(_InputIter first, _InputIter last, _RandomAccessIter __out,
                                  _RandomNumberGenerator& __rand, const _Distance __n)
{
    _Distance __m = 0;
    _Distance __t = __n;
    for ( ; first != last && __m < __n; ++__m, ++first)
    __out[__m] = *first;

    while (first != last)
    {
         ++__t;
         _Distance __M = __rand(__t);
         if (__M < __n)
         __out[__M] = *first;
         ++first;
    }
    return __out + __m;
}

template <class InputIter, class RandomAccessIter>
inline RandomAccessIter random_sample(InputIter first, InputIter last,
                                      RandomAccessIter out_first, RandomAccessIter out_last)
{
    return __random_sample(first, last, out_first, out_last - out_first);
}


template <class InputIter, class RandomAccessIter, class RandomNumberGenerator>
inline RandomAccessIter random_sample(InputIter first, InputIter last,
                                      RandomAccessIter out_first, RandomAccessIter out_last,
                                      RandomNumberGenerator& rand)
{
    return __random_sample(first, last, out_first, out_last - out_first);
}



// partition, stable_partition
template <class ForwardIter, class Predicate>
ForwardIter __partition(ForwardIter first, ForwardIter last, Predicate pred)
{
    if(first == last) return first;
    while(pred(*first))
        if(++first == last) return first;

    ForwardIter next = first;
    while(++next != last)
        if(pred(*next))
        {
            swap(next, first);
            ++first;
        }
    return first;
}


template <class BidirectionalIter, class  Predicate>
BidirectionalIter __partition(BidirectionalIter first, BidirectionalIter last, Predicate pred, bidirectional_iterator_tag)
{
    while(true)
    {
        while(true)
            if(first == last)
                return first;
            else if(pred(*first))
                first++;
            else
                break;

        --last;
        while(true)
            if(first == last)
                return first;
            else if(!pred(*last))
                --last;
            else break;
        iter_swap(first, last);
        ++first;
    }
}

template <class ForwardIter, class Preicate>
inline ForwardIter partition(ForwardIter first, ForwardIter last, Preicate pred)
{
    return __partition(first, last, pred, ITERATOR_CATEGORY(first));
}



template <class ForwardIter, class Predicate, class Distance>
inline ForwardIter __inplace_stable_partition(ForwardIter first, ForwardIter last, Predicate pred, Distance len)
{
     if(len == 1)
         return pred(*first) ? last : first;

     ForwardIter middle = first;
     advance(middle, len/2);
     return rotate(__inplace_stable_partition(first, middle, pred, len/2),
                   middle,
                   __inplace_stable_partition(middle, last, pred, len - len/2));
}

template <class ForwardIter, class Pointer, class Predicate, class Distance>
ForwardIter __stable_partition_adaptive(ForwardIter first, ForwardIter last, Predicate pred, Distance len,
                                        Pointer buffer, Distance buffer_size)
{
    if(len <= buffer_size)
    {
        ForwardIter result1 = first;
        Pointer result2 = buffer;
        for(; first != last; ++first)
        {
            if(pred(*first))
            {
                *result1 = *first;
                ++result1;
            }
            else
            {
                *result2 = *first;
                ++result2;
            }
            copy(buffer, result2, result1);
            return result1;
        }
    }
    else
    {
        ForwardIter middle = first;
        advance(middle, len /2 );
        return rotate(__stable_partition_adaptive(first, middle, pred, len/2, buffer, buffer_size),
                      middle,
                      __stable_partition_adaptive(middle, last, pred, len - len/2, buffer, buffer_size));
    }
}

template <class ForwardIter, class Predicate, class Tp, class Distance>
inline ForwardIter __stable_partition_aux(ForwardIter first, ForwardIter last, Predicate pred, Tp*, Distance*)
{
    Temporary_buffer<ForwardIter, Tp> buf(first, last);
    if(buf.size() > 0)
        return __stable_partition_adaptive(first, last, pred, Distance(buf.requested_size()),
                                           buf.begin(), buf.size());
    else
        return __inplace_stable_partition(first, last, pred, Distance(buf.requested_size()));
}

template <class ForwardIter, class Predicate>
inline ForwardIter stable_partition(ForwardIter first, ForwardIter last, Predicate pred)
{
    if(first == last)
        return first;
    else
        return __stable_partition_aux(first, last, pred, VALUE_TYPE(first), DISTANCE_TYPE(first));
}


// partial_sort() , partial_sort_copy()

template <class RandomAccessIter, class Tp>
inline void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Tp*)
{
    make_heap(first, middle);
    for(RandomAccessIter i = middle; i < last; ++i)
        if(*i < *first)
            __pop_heap(first, middle, i, Tp(*i), DISTANCE_TYPE(first));
    sort_heap(first, middle);
}

template <class RandomAccessIter>
inline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last)
{
    __partial_sort(first, middle, last, VALUE_TYPE(first));
}

template <class RandomAccessIter, class Tp, class Compare>
inline void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Tp*, Compare comp)
{
    make_heap(first, middle, comp);
    for(RandomAccessIter i = middle; i < last; ++i)
        if(comp(*i, *first))
            __pop_heap(first, middle, i, Tp(*i), comp, DISTANCE_TYPE(first));
    sort_heap(first, middle, comp);
}

template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, comp);
}


// sort()
template <class RandomAccessIter, class Tp>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, Tp pivot)
{
    while (true)
    {
        while(*first < pivot)
            ++first;
        --last;
        while(*last > pivot)
            --last;
        if(!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}


template <class RandomAccessIter, class Tp, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, Tp pivot, Compare comp)
{
    while (true)
    {
        while(comp(*first, pivot))
            ++first;
        --last;
        while(comp(*last, pivot))
            --last;
        if(!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

const int stl_threshold = 16;

template <class RandomAccessIter, class Tp>
void __unguard_linear_insert(RandomAccessIter last, Tp val)
{
    RandomAccessIter next = last;
    --next;
    while(val < *next)
    {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}


template <class RandomAccessIter, class Tp, class Compare>
void __unguard_linear_insert(RandomAccessIter last, Tp val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while(comp(val < *next))
    {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class RandomAccessIter, class Tp>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, Tp*)
{
    Tp val = *last;
    if(val < *first)
    {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguard_linear_insert(last, val);
}

template <class RandomAccessIter>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last)
{
    if(first == last) return;
    for(RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, VALUE_TYPE(first));
}

template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if(first == last) return;
    for(RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, VALUE_TYPE(first), comp);
}

template <class RandomAccessIter, class Tp>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*)
{
    for(RandomAccessIter i = first; i != last; ++i)
        __unguard_linear_insert(i, Tp(*i));
}

template <class RandomAccessIter>
inline void __unguarded_insertion_sort(RandomAccessIter first, RandomAccessIter last)
{
    __unguarded_insertion_sort_aux(first, last, VALUE_TYPE(first));
}

template <class RandomAccessIter, class Tp, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*, Compare comp)
{
    for(RandomAccessIter i = first; i != last; ++i)
        __unguard_linear_insert(i, Tp(*i), comp);
}

template <class RandomAccessIter, class Compare>
inline void __unguarded_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __unguarded_insertion_sort_aux(first, last, VALUE_TYPE(first), comp);
}


template <class RandomAccessIter>
void __final_insertion_sort(RandomAccessIter first, RandomAccessIter last)
{
    if(last - first > stl_threshold)
    {
        __insertion_sort(first, first + stl_threshold);
        __unguarded_insertion_sort(first + stl_threshold, last);
    }
    else
        __insertion_sort(first, last);
}

template <class RandomAccessIter, class Compare>
void __final_insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if(last - first > stl_threshold)
    {
        __insertion_sort(first, first + stl_threshold, comp);
        __unguarded_insertion_sort(first + stl_threshold, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

template <class Size>
inline Size __lg(Size n)
{
    Size k;
    for(k = 0; n != 1; n >>= 1) ++k;
    return k;
}

template <class RandomAccessIter, class Tp, class Size>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, Tp*, Size depth_limit)
{
    while(last - first > stl_threshold)
    {
        if(depth_limit == 0)
        {
            partial_sort(first, last, last);
            return;
        }

        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last,
                        Tp(median(*first, *(first + (last - first)/2), *(last - 1))));
        __introsort_loop(cut, last, (Tp*) 0, depth_limit);
        last = cut;
    }
}

template <class RandomAccessIter, class Tp, class Size, class Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, Tp*, Size depth_limit, Compare comp)
{
    while(last - first > stl_threshold)
    {
        if(depth_limit == 0)
        {
            partial_sort(first, last, last, comp);
            return;
        }

        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last,
                                                     Tp(median(*first, *(first + (last - first)/2), *(last - 1))), comp);
        __introsort_loop(cut, last, (Tp*) 0, depth_limit, comp);
        last = cut;
    }

}

template <class RandomAccessIter>
inline void sort(RandomAccessIter first, RandomAccessIter last)
{
    if(first != last )
    {
        __introsort_loop(first, last, VALUE_TYPE(first), __lg(last - first) * 2);
        __final_insertion_sort(first, last);
    }
}

template <class RandomAccessIter, class Compare>
inline void sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if(first != last)
    {
        __introsort_loop(first, last, VALUE_TYPE(first), __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}



// stable_sort()
template <class RandomAccessIter>
void __inplace_stable_sort(RandomAccessIter first, RandomAccessIter last)
{
    if(last - first < 15)
    {
        __insertion_sort(first, last);
        return;
    }
    RandomAccessIter middle = first + (last - first) / 2;
    __inplace_stable_sort(first, middle);
    __inplace_stable_sort(middle, last);
    __merge_without_buffer(first, middle, last, middle - first, last - middle);
}

template <class RandomAccessIter, class Compare>
void __inplace_stable_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if(last - first < 15)
    {
        __insertion_sort(first, last, comp);
        return;
    }
    RandomAccessIter middle = first + (last - first) / 2;
    __inplace_stable_sort(first, middle ,comp);
    __inplace_stable_sort(middle, last, comp);
    __merge_without_buffer(first, middle, last, middle - first, last - middle, comp);
}

template <class RandomAccessIter1, class RandomAccessIter2, class Distance>
void __merge_sort_loop(RandomAccessIter1 first, RandomAccessIter1 last,
                       RandomAccessIter2 result, Distance step_size)
{
    Distance two_step = 2 * step_size;
    while(last - first >= two_step)
    {
        result = merge(first, first + step_size, first + step_size, first + two_step, result);
        first += two_step;
    }

    step_size = min(Distance(last - first), step_size);
    merge(first, first + step_size, first + step_size, last, result);
}

template <class RandomAccessIter1, class RandomAccessIter2, class Distance, class Compare>
void __merge_sort_loop(RandomAccessIter1 first, RandomAccessIter1 last, RandomAccessIter2 result,
                       Distance step_size, Compare comp)
{
    Distance two_step = 2 * step_size;

    while(last - first >= two_step)
    {
        result = merge(first, first + step_size, first + step_size, first + two_step, result);
        first += two_step;
    }
    step_size = min(Distance(last - first), step_size);
    merge(first, first + step_size, first + step_size, last, result, comp);
}

const int stl_chunk_size = 7;

template <class RandomAccessIter, class Distance>
void __chunk_insertion_sort(RandomAccessIter first, RandomAccessIter last, Distance chunk_size)
{
    if(last - first >= chunk_size)
    {
        __insertion_sort(first, first + chunk_size);
        first += chunk_size;
    }
    __insertion_sort(first, last);
}

template <class RandomAccessIter, class Distance, class Compare>
void __chunk_insertion_sort(RandomAccessIter first, RandomAccessIter last,
                            Distance chunk_size, Compare comp)
{
    if(last - first >= chunk_size)
    {
        __insertion_sort(first, first + chunk_size, comp);
        first += chunk_size;
    }
    __insertion_sort(first, last, comp);
}

template <class RandomAccessIter, class Pointer, class Distance>
void __merge_sort_with_buffer(RandomAccessIter first, RandomAccessIter last,
                              Pointer buffer, Distance*)
{
    Distance len = last - first;
    Pointer buffer_last = buffer + len;

    Distance step_size = stl_chunk_size;
    __chunk_insertion_sort(first, last, step_size);

    while(step_size < len)
    {
        __merge_sort_loop(first, last, buffer, step_size);
        step_size *= 2;
        __merge_sort_loop(buffer, buffer_last, first, step_size);
        step_size *= 2;
    }
}


template <class RandomAccessIter, class Pointer, class Distance, class Compare>
void __merge_sort_with_buffer(RandomAccessIter first, RandomAccessIter last,
                              Pointer buffer, Distance*, Compare comp)
{
    Distance len = last - first;
    Pointer buffer_last = buffer + len;

    Distance step_size = stl_chunk_size;
    __chunk_insertion_sort(first, last, step_size, comp);

    while(step_size < len)
    {
        __merge_sort_loop(first, last, buffer, step_size, comp);
        step_size *= 2;
        __merge_sort_loop(buffer, buffer_last, first, step_size, comp);
        step_size *= 2;
    }
}


template <class RandomAccessIter, class Pointer, class Distance>
void __stable_sort_adaptive(RandomAccessIter first, RandomAccessIter last,
                            Pointer buffer, Distance buffer_size)
{
    Distance len = (last - first + 1) / 2;
    RandomAccessIter middle = first + len;
    if(len > buffer_size)
    {
        __stable_sort_adaptive(first, middle, buffer, buffer_size);
        __stable_sort_adaptive(middle, last, buffer, buffer_size);
    }
    else
    {
        __merge_sort_with_buffer(first, middle, buffer, (Distance*) 0);
        __merge_sort_with_buffer(middle, last, buffer, (Distance*) 0);
    }
    __merge_adaptive(first, middle, last, Distance(middle - first),
                     Distance(last - middle), buffer, buffer_size);
}

template <class RandomAccessIter, class Pointer, class Distance, class Compare>
void __stable_sort_adaptive(RandomAccessIter first, RandomAccessIter last,
                            Pointer buffer, Distance buffer_size, Compare comp)
{
    Distance len = (last - first + 1) / 2;
    RandomAccessIter middle = first + len;
    if(len > buffer_size)
    {
        __stable_sort_adaptive(first, middle, buffer, buffer_size, comp);
        __stable_sort_adaptive(middle, last, buffer, buffer_size, comp);
    }
    else
    {
        __merge_sort_with_buffer(first, middle, buffer, (Distance*) 0, comp);
        __merge_sort_with_buffer(middle, last, buffer, (Distance*) 0, comp);
    }
    __merge_adaptive(first, middle, last, Distance(middle - first),
                     Distance(last - middle), buffer, buffer_size, comp);
}


template <class RandomAccessIter, class Tp, class Distance>
inline void __stable_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*, Distance*)
{
    Temporary_buffer<RandomAccessIter, Tp> buf(first, last);
    if(buf.begin() == 0)
        __inplace_stable_sort(first, last);
    else
        __stable_sort_adaptive(first, last, buf.begin(), Distance(buf.size()));
}

template <class RandomAccessIter, class Tp, class Distance, class Compare>
inline void __stable_sort_aux(RandomAccessIter first, RandomAccessIter last, Tp*, Distance*, Compare comp)
{
    Temporary_buffer<RandomAccessIter, Tp> buf(first, last);
    if(buf.begin() == 0)
        __inplace_stable_sort(first, last, comp);
    else
        __stable_sort_adaptive(first, last, buf.begin(), Distance(buf.size()), comp);
}

template <class RandomAccessIter>
inline void stable_sort(RandomAccessIter first, RandomAccessIter last)
{
    __stable_sort_aux(first, last, VALUE_TYPE(first), DISTANCE_TYPE(first));
}

template <class RandomAccessIter, class Compare>
inline void stable_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __stable_sort_aux(first, last, VALUE_TYPE(first), DISTANCE_TYPE(first), comp);
}


template <class InputIter, class RandomAccessIter, class Distance, class Tp>
RandomAccessIter __partial_sort_copy(InputIter first, InputIter last,
                                     RandomAccessIter result_first, RandomAccessIter result_last, Distance*, Tp*)
{
    if(result_first == result_last) return result_last;
    RandomAccessIter result_real_last = result_first;
    while(first != last && result_real_last != result_last)
    {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    make_heap(result_first, result_real_last);
    while(first != last)
    {
        if(*first < *result_first)
            __adjust_heap(result_first, Distance(0), Distance(result_real_last - result_first), Tp(*first));
        ++first;
    }
    sort_heap(result_first, result_real_last);
    return result_real_last;
}

template <class InputIter, class RandomAccessIter>
inline RandomAccessIter partial_sort_copy(InputIter first, InputIter last,
                                          RandomAccessIter result_first, RandomAccessIter result_last)
{
    return __partial_sort_copy(first, last, result_first, result_last, DISTANCE_TYPE(result_first), VALUE_TYPE(first));
}

template <class InputIter, class RandomAccessIter, class Compare, class Distance, class Tp>
RandomAccessIter __partial_sort_copy(InputIter first, InputIter last,
                                     RandomAccessIter result_first, RandomAccessIter result_last,
                                     Compare comp, Distance*, Tp*)
{
    if(result_first == result_last) return result_last;
    RandomAccessIter result_real_last = result_first;
    while(first != last && result_real_last != result_last)
    {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    make_heap(result_first, result_real_last, comp);
    while(first != last)
    {
        if(comp(*first < *result_first))
            __adjust_heap(result_first, Distance(0),
                          Distance(result_real_last - result_first), Tp(*first), comp);
        ++first;
    }
    sort_heap(result_first, result_real_last, comp);
    return result_real_last;
}

template <class InputIter, class RandomAccessIter, class Compare>
inline RandomAccessIter partial_sort_copy(InputIter first, InputIter last,
                                          RandomAccessIter result_first, RandomAccessIter result_last, Compare comp)
{
    return __partial_sort_copy(first, last, result_first, result_last,
                               comp, DISTANCE_TYPE(result_first), VALUE_TYPE(first));
}



// nth_element()

template <class RandomAccessIter, class Tp>
void __nth_element(RandomAccessIter first, RandomAccessIter nth, RandomAccessIter last, Tp*)
{
    while(last - first > 3)
    {
        RandomAccessIter cut = __unguarded_partition(first, last,
                        Tp(median(*first, *(first + (last - first) / 2), *(last - 1))));
        if(cut <= nth)
            first = cut;
        else
            last = cut;
    }
    __insertion_sort(first, last);
}

template <class RandomAccessIter>
inline void nth_element(RandomAccessIter first, RandomAccessIter nth, RandomAccessIter last)
{
    __nth_element(first, nth, last, VALUE_TYPE(first));
}

template <class RandomAccessIter, class Tp, class Compare>
void __nth_element(RandomAccessIter first, RandomAccessIter nth, RandomAccessIter last, Tp*, Compare comp)
{
    while(last - first > 3)
    {
        RandomAccessIter cut = __unguarded_partition(first, last,
                                Tp(median(*first,*(first + (last - first) / 2, *(last - 1)))));
        if(cut <= nth)
            first = cut;
        else
            last = cut;
    }
    __insertion_sort(first, last);
}

template <class RandomAccessIter, class Compare>
inline void nth_element(RandomAccessIter first, RandomAccessIter nth, RandomAccessIter last, Compare comp)
{
    __nth_element(first, nth, last, VALUE_TYPE(first));
}



// Binary search, lower_bound() and upper_bound
template <class ForwardIter, class Tp, class Distance>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last, const Tp& val, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(*middle < val)
        {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}


template <class ForwardIter, class Tp>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last, const Tp& val)
{
    return __lower_bound(first, last, val, DISTANCE_TYPE(first));
}


template <class ForwardIter, class Tp, class Distance, class Compare>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last, const Tp& val,
                          Compare comp, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(comp(*middle, val))
        {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}


template <class ForwardIter, class Tp, class Compare>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp)
{
    return __lower_bound(first, last, val, comp, DISTANCE_TYPE(first));
}

template <class ForwardIter, class Tp, class Distance>
inline ForwardIter __upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(val < *middle)
            len = half;
        else
        {
            first = middle;
            ++first;
            len = len -half - 1;
        }
    }
    return first;
}

template <class ForwardIter, class Tp>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last, const Tp& val)
{
    return __upper_bound(first, last, val, DISTANCE_TYPE(first));
}

template <class ForwardIter, class Tp, class Distance, class Compare>
inline ForwardIter __upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(comp(val, *middle))
            len = half;
        else
        {
            first = middle;
            ++first;
            len = len -half - 1;
        }
    }
    return first;
}


template <class ForwardIter, class Tp, class Compare>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp)
{
    return __upper_bound(first, last, val, comp, DISTANCE_TYPE(first));
}

template <class ForwardIter, class Tp, class Distance>
pair<ForwardIter, ForwardIter> __equal_range(ForwardIter first, ForwardIter last, const Tp& val, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, right, left;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(*middle < val)
        {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if(val < *middle)
            len = half;
        else
        {
            left = lower_bound(first, middle, val);
            advance(first, len);
            right = upper_bound(++middle, first, val);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

template <class ForwardIter, class Tp>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const Tp& val)
{
    return __equal_range(first, last, val, DISTANCE_TYPE(first));
}

template <class ForwardIter, class Tp, class Compare, class Distance>
pair<ForwardIter, ForwardIter> __equal_range(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, right, left;

    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(*middle < val)
        {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if(val < *middle)
            len = half;
        else
        {
            left = lower_bound(first, middle, val);
            advance(first, len);
            right = upper_bound(++middle, first, val);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

template <class ForwardIter, class Tp, class Compare>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const Tp& val, Compare comp)
{
    return __equal_range(first, last, val, comp, DISTANCE_TYPE(first));
}

template <class ForwardIter, class Tp>
bool binary_search(ForwardIter first, ForwardIter last, const Tp& val)
{
    ForwardIter i = lower_bound(first, last, val);
    return i != last && !(val < *i);
}

template <class ForwardIter, class Tp, class Compare>
bool binary_search(ForwardIter first, ForwardIter last, const Tp& val, Compare comp)
{
    ForwardIter i = lower_bound(first, last, val, comp);
    return i != last && !comp(val, *i);
}



// merge()

template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first2 < *first1)
        {
            *result = * first2;
            ++first2;
        }
        else
        {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}


template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(first2, *first1))
        {
            *result = * first2;
            ++first2;
        }
        else
        {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

// inplace_merge()
template <class BidirectionalIter, class Distance>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                            Distance len1, Distance len2)
{
    if(len1 == 0 || len2 == 0)
        return;
    if(len1 + len2 == 2)
    {
        if(*middle < *first)
            iter_swap(first, middle);
        return;
    }

    BidirectionalIter first_cut = first;
    BidirectionalIter second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if(len1 > len2)
    {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut);
        distance(middle, second_cut, len22);
    }
    else
    {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut);
        distance(first, first_cut, len11);
    }

    BidirectionalIter new_middle = rotate(first_cut, middle, second_cut);
    __merge_without_buffer(first, first_cut, new_middle, len11, len22);
    __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
}


template <class BidirectionalIter, class Distance, class Compare>
void __merge_without_buffer(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                            Distance len1, Distance len2, Compare comp)
{
    if(len1 == 0 || len2 == 0)
        return;
    if(len1 + len2 == 2)
    {
        if(comp(*middle, *first))
            iter_swap(first, middle);
        return;
    }

    BidirectionalIter first_cut = first;
    BidirectionalIter second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if(len1 > len2)
    {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut, comp);
        distance(middle, second_cut, len22);
    }
    else
    {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut, comp);
        distance(first, first_cut, len11);
    }

    BidirectionalIter new_middle = rotate(first_cut, middle, second_cut);
    __merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
    __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
}


template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter1 __rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle, BidirectionalIter1 last,
                                     Distance len1, Distance len2, BidirectionalIter2 buffer, Distance buffer_size)
{
    BidirectionalIter2 buffer_end;
    if(len1 > len2 && len2 <= buffer_size)
    {
        buffer_end = copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    }
    else if(len1 <= buffer_size)
    {
        buffer_end = copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    }
    else
        return rotate(first, middle, last);
}

template <class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
                                    BidirectionalIter2 first2, BidirectionalIter2 last2,
                                    BidirectionalIter3 result)
{
    if(first1 == last1)
        return copy_backward(first2, last2, result);
    if(first2 == last2)
        return copy_backward(first1, last1, result);

    --last1;
    --last2;
    while(true)
    {
        if(*last2 < *last1)
        {
            *--result = *last1;
            if(first1 == last1)
                return copy_backward(first2, ++last2, result);
            --last1;
        }
        else
        {
            *--result = *last2;
            if(first2 == last2)
                return copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3, class Compare>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
                                    BidirectionalIter2 first2, BidirectionalIter2 last2,
                                    BidirectionalIter3 result, Compare comp)
{
    if(first1 == last1)
        return copy_backward(first2, last2, result);
    if(first2 == last2)
        return copy_backward(first1, last1, result);

    --last1;
    --last2;
    while(true)
    {
        if(comp(*last2, *last1))
        {
            *--result = *last1;
            if(first1 == last1)
                return copy_backward(first2, ++last2, result);
            --last1;
        }
        else
        {
            *--result = *last2;
            if(first2 == last2)
                return copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <class BidirectioanalIter, class Distance, class Pointer>
void __merge_adaptive(BidirectioanalIter first, BidirectioanalIter middle, BidirectioanalIter last,
                      Distance len1, Distance len2, Pointer buffer, Distance buffer_size)
{
    if(len1 <= len2 && len1 <= buffer_size)
    {
        Pointer buffer_end = copy(first, middle, buffer);
        merge(buffer, buffer_end, middle, last, first);
    }
    else if(len2 <= buffer_size)
    {
        Pointer buffer_end = copy(middle, last, buffer);
        __merge_backward(first, middle, buffer, buffer_end, last);
    }
    else
    {
        BidirectioanalIter first_cut = first;
        BidirectioanalIter second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if(len1 > len2)
        {
            len11 = len1 / 2;
            advance(first_cut, len11);
            second_cut = lower_bound(middle, last, *first_cut);
            distance(middle, second_cut, len22);
        }
        else
        {
            len22 = len2 / 2;
            advance(second_cut, len22);
            first_cut = upper_bound(first, middle, *second_cut);
            distance(first_cut, middle, len11);
        }
        BidirectioanalIter new_middle = __rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        __merge_adaptive(first, first_cut, middle, len11, len22, buffer, buffer_size);
        __merge_adaptive(middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size);
    }
}

template <class BidirectioanalIter, class Distance, class Pointer, class Compare>
void __merge_adaptive(BidirectioanalIter first, BidirectioanalIter middle, BidirectioanalIter last,
                      Distance len1, Distance len2, Pointer buffer, Distance buffer_size, Compare comp)
{
    if(len1 <= len2 && len1 <= buffer_size)
    {
        Pointer buffer_end = copy(first, middle, buffer);
        merge(buffer, buffer_end, middle, last, first);
    }
    else if(len2 <= buffer_size)
    {
        Pointer buffer_end = copy(middle, last, buffer);
        __merge_backward(first, middle, buffer, buffer_end, last);
    }
    else
    {
        BidirectioanalIter first_cut = first;
        BidirectioanalIter second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if(len1 > len2)
        {
            len11 = len1 / 2;
            advance(first_cut, len11);
            second_cut = lower_bound(middle, last, *first_cut, comp);
            distance(middle, second_cut, len22);
        }
        else
        {
            len22 = len2 / 2;
            advance(second_cut, len22);
            first_cut = upper_bound(first, middle, *second_cut, comp);
            distance(first_cut, middle, len11);
        }
        BidirectioanalIter new_middle = __rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        __merge_adaptive(first, first_cut, middle, len11, len22, buffer, buffer_size, comp);
        __merge_adaptive(middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size, comp);
    }
}

template <class BidirectionalIter, class Tp, class Distance>
inline void __inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                                Tp*, Distance*)
{
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);

    Temporary_buffer<BidirectionalIter, Tp> buf(first, last);
    if(buf.begin() == 0)
        __merge_without_buffer(first, middle, last, len1, len2);
    else
        __merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()));

}

template <class BidirectionalIter, class Tp, class Distance, class Compare>
inline void __inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last,
                                Tp*, Distance*, Compare comp)
{
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);

    Temporary_buffer<BidirectionalIter, Tp> buf(first, last);
    if(buf.begin() == 0)
        __merge_without_buffer(first, middle, last, len1, len2);
    else
        __merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()), comp);
}

template <class BidirectionalIter>
inline void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last)
{
    if(first == middle || middle == last)
        return;
    __inplace_merge_aux(first, middle, last, VALUE_TYPE(first), DISTANCE_TYPE(first));
}

template <class BidirectionalIter, class Compare>
inline void inplace_merge(BidirectionalIter first, BidirectionalIter middle, BidirectionalIter last, Compare comp)
{
    if(first == middle || middle == last)
        return;
    __inplace_merge_aux(first, middle, last, VALUE_TYPE(first), DISTANCE_TYPE(first), comp);
}


// set algorithm: includes  set_union(), set_intresection() , set_difference(),
// set_symmetric_difference()

template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
{
    while(first1 != last1 && first2 != last2)
        if(*first2 < *first1)
            return false;
        else if(*first1 < *first2)
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

template <class InputIter1, class InputIter2, class Compare>
bool includes(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare comp)
{
    while(first1 != last1 && first2 != last2)
        if(comp(*first2, *first1))
            return false;
        else if(comp(*first1, *first2))
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2)
        {
            *result = *first1;
            ++first1;
        }
        else if(*first2 < *first1)
        {
            *result = *first2;
            ++first2;
        }
        else
        {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return MySTL::copy(first2, last2, copy(first1, last1, result));
}

template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1;
            ++first1;
        }
        else if(comp(*first2, *first1))
        {
            *result = *first2;
            ++first2;
        }
        else
        {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return MySTL::copy(first2, last2, copy(first1, last1, result));
}

template  <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2)
            ++first1;
        else if(*first2 < *first1)
            ++first2;
        else
        {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}


template  <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result, Compare comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
            ++first1;
        else if(comp(*first2, *first1))
            ++first2;
        else
        {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2)
        {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if(*first2 < *first1)
            ++first2;
        else
        {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}


template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result, Compare comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if(comp(*first2, *first1))
            ++first2;
        else
        {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}


template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                     InputIter2 first2, InputIter2 last2,
                                     OutputIter result)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2)
        {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if(*first2 < *first1)
        {
            *result = *first2;
            ++first2;
            ++result;
        }
        else
        {
            ++first1;
            ++first2;
        }
    }
    return copy(first2, last2, copy(first1, last1, result));
}


template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                    InputIter2 first2, InputIter2 last2,
                                    OutputIter result, Compare comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if(comp(*first2, *first1))
        {
            *result = *first2;
            ++first2;
            ++result;
        }
        else
        {
            ++first1;
            ++first2;
        }
    }
    return copy(first2, last2, copy(first1, last1, result));
}

// min_element() and max_element()
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last)
{
    if(first == last) return first;
    ForwardIter result = first;
    while(++first != last)
        if(*result < *first)
            result = first;
    return result;
}

template <class ForwardIter, class Compare>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compare comp)
{
    if(first == last) return first;
    ForwardIter result = first;
    while(++first != last)
        if(comp(*result, *first))
            result = first;
    return result;
}

template <class ForwardIter>
ForwardIter min_element(ForwardIter first, ForwardIter last)
{
    if(first == last) return first;
    ForwardIter result = first;
    while(++first != last)
        if(*first < *result)
            result = first;
    return result;
}

template <class ForwardIter, class Compare>
ForwardIter min_element(ForwardIter first, ForwardIter last, Compare comp)
{
    if(first == last) return first;
    ForwardIter result = first;
    while(++first != last)
        if(comp(*first, result))
            result = first;
    return result;
}

// next_permutation() prev_permutation()
template <class BidirectionalIter>
bool next_permutation(BidirectionalIter first, BidirectionalIter last) {
    if (first == last)
        return false;
    BidirectionalIter i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;

    for(;;)
    {
        BidirectionalIter ii = i;
        --i;
        if (*i < *ii)
        {
            BidirectionalIter j = last;
            while (!(*i < *--j)) {}
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}

template <class BidirectionalIter, class Compare>
bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compare __comp)
{
    if (first == last)
        return false;
    BidirectionalIter i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;

    for(;;)
    {
        BidirectionalIter ii = i;
        --i;
        if (__comp(*i, *ii))
        {
            BidirectionalIter j = last;
            while (!__comp(*i, *--j)) {}
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}

template <class BidirectionalIter>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
{
    if (first == last)
        return false;
    BidirectionalIter i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;

    for(;;) {
        BidirectionalIter ii = i;
        --i;
        if (*ii < *i)
        {
            BidirectionalIter j = last;
            while (!(*--j < *i)) {}
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}

template <class BidirectionalIter, class Compare>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last,
                      Compare __comp)
{
    if (first == last)
        return false;
    BidirectionalIter i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;

    for(;;)
    {
        BidirectionalIter ii = i;
        --i;
        if (__comp(*ii, *i))
        {
            BidirectionalIter j = last;
            while (!__comp(*--j, *i))
            {}
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}


// find_first_of()
template <class InputIter, class ForwardIter>
InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2)
{
    for(; first1 != last1; ++first1)
        for(ForwardIter iter = first2; iter != last2; ++iter)
            if(*first1 == *iter)
                return first1;
    return last1;
}


template <class InputIter, class ForwardIter, class BinaryPredicate>
InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2, BinaryPredicate comp)
{
    for(; first1 != last1; ++first1)
        for(ForwardIter iter = first2; iter != last2; ++iter)
            if(comp(*first1, *iter))
                return first1;
    return last1;
}


// find_end()
template <class ForwardIter1, class ForwardIter2>
ForwardIter1 __find_end(ForwardIter1 first1, ForwardIter1 last1,
                      ForwardIter2 first2, ForwardIter2 last2,
                      forward_iterator_tag, forward_iterator_tag)
{
    if(first2 == last2)
        return last1;
    else
    {
        ForwardIter1 result = last1;
        while(true)
        {
            ForwardIter1 new_result = search(first1, last1, first2, last2);
            if(new_result == last1)
                return result;
            else
            {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}


template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
ForwardIter1 __find_end(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        forward_iterator_tag, forward_iterator_tag, BinaryPredicate comp)
{
    if(first2 == last2)
        return last1;
    else
    {
        ForwardIter1 result = last1;
        while(true)
        {
            ForwardIter1 new_result = search(first1, last1, first2, last2, comp);
            if(new_result == last1)
                return result;
            else
            {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1 __find_end(BidirectionalIter1 first1, BidirectionalIter1 last1,
                              BidirectionalIter2 first2, BidirectionalIter2 last2,
                              bidirectional_iterator_tag, bidirectional_iterator_tag)
{
    typedef reverse_iterator<BidirectionalIter1> RevIter1;
    typedef reverse_iterator<BidirectionalIter2> RevIter2;

    RevIter1 rlast1(first1);
    RevIter2 rlast2(first2);
    RevIter1 rresult = search(RevIter1(last1), rlast1, RevIter2(last2), rlast2);

    if(rresult == rlast1)
        return last1;
    else
    {
        BidirectionalIter1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
    }
}

template <class BidirectionalIter1, class BidirectionalIter2, class BinaryPredicate>
BidirectionalIter1 __find_end(BidirectionalIter1 first1, BidirectionalIter1 last1,
                              BidirectionalIter2 first2, BidirectionalIter2 last2,
                              bidirectional_iterator_tag, bidirectional_iterator_tag,
                              BinaryPredicate comp)
{
    typedef reverse_iterator<BidirectionalIter1> RevIter1;
    typedef reverse_iterator<BidirectionalIter2> RevIter2;

    RevIter1 rlast1(first1);
    RevIter2 rlast2(first2);
    RevIter1 rresult = search(RevIter1(last1), rlast1, RevIter2(last2), rlast2, comp);

    if(rresult == rlast1)
        return last1;
    else
    {
        BidirectionalIter1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
    }
}


template <class ForwardIter1, class ForwardIter2>
inline ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                             ForwardIter2 first2, ForwardIter2 last2)
{
    return __find_end(first1, last1, first2, last2, ITERATOR_CATEGORY(first1), ITERATOR_CATEGORY(first2));
}

template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
inline ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                             ForwardIter2 first2, ForwardIter2 last2,
                             BinaryPredicate comp)
{
    return __find_end(first1, last1, first2, last2, ITERATOR_CATEGORY(first1), ITERATOR_CATEGORY(first2), comp);
}


// is_heap()
template <class RandomAccessIter, class Distance>
bool __is_heap(RandomAccessIter first, Distance n)
{
    Distance parent = 0;
    for(Distance child = 1; child < n; ++child)
    {
        if(first[parent] < first[child])
            return false;
        if((child & 1) == 0)
            ++parent;
    }
    return true;
}

template <class RandomAccessIter, class Distance, class StrictWeakOrdering>
bool __is_heap(RandomAccessIter first, StrictWeakOrdering comp, Distance n)
{
    Distance parent = 0;
    for(Distance child = 1; child < n; ++child)
    {
        if(comp(first[parent], first[child]))
            return false;
        if((child & 1) == 0)
            ++parent;
    }
    return true;
}

template <class RandomAccessIter>
inline bool is_heap(RandomAccessIter first, RandomAccessIter last)
{
    return __is_heap(first, last - first);
}

template <class RandomAccessIter, class StrictWeakOrdering>
inline bool is_heap(RandomAccessIter first, RandomAccessIter last, StrictWeakOrdering comp)
{
    return __is_heap(first, comp, last - first);
}

// is_sorted()
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last)
{
    if(first == last)
        return true;
    ForwardIter next = first;
    ++next;
    for(; next != last; first = next, ++next)
        if(*next < *first)
            return false;
    return true;
}


template <class ForwardIter, class StrictWeakOrdering>
bool is_sorted(ForwardIter first, ForwardIter last, StrictWeakOrdering comp)
{
    if(first == last)
        return true;
    ForwardIter next = first;
    ++next;
    for(; next != last; first = next, ++next)
        if(comp(*next, *first))
            return false;
    return true;
}


} // namespace MySTL
#endif //MYSTL_STL_ALGO_H
