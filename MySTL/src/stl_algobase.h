//
// Created by GW00243464 on 2022/8/31.
//

#ifndef MYSTL_STL_ALGOBASE_H
#define MYSTL_STL_ALGOBASE_H
#include "stl_iterator.h"
#include "type_traits.h"
#include <stdio.h>
#include <string.h>
#include "stl_pair.h"
namespace MySTL
{


template  <class ForwardIter1, class ForwardIter2, class Tp>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, Tp*)
{
    Tp tmp = *a;
    *a = *b;
    *b = tmp;
}


template <class ForwaredIter1, class ForwaredIter2>
inline void iter_swap(ForwaredIter1 a, ForwaredIter2 b)
{
    __iter_swap(a, b, VALUE_TYPE(a));
}

template <class Tp>
inline void swap(Tp& a, Tp& b)
{
    Tp tmp = a;
    a = b;
    b = tmp;
}


// min and max

template <class Tp>
inline const Tp& min(const Tp& a, const Tp& b)
{ return b < a ? b : a; }


template <class Tp>
inline const Tp& max(const Tp& a, const Tp& b)
{ return a < b ? b : a; }


template <class Tp, class Compare>
inline const Tp& min(const Tp& a, const Tp& b, Compare comp)
{ return comp(b < a) ? b : a; }


template <class Tp, class Compare>
inline const Tp& max(const Tp& a, const Tp& b, Compare comp)
{ return comp(a, b) ? b : a; }






// copy

template <class InputIter, class OutputIter, class Distance>
inline OutputIter copy(InputIter first, InputIter last, OutputIter result,
                         input_iterator_tag, Distance*)
{
    for(; first != last; ++result, ++first)
        *result = *first;
    return result;
}

template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter copy(RandomAccessIter first, RandomAccessIter last,
                         OutputIter result, random_access_iterator_tag, Distance*)
{
    for(Distance n = last - first; n > 0; --n)
    {
        *result = *first;
        ++result;
        ++first;
    }
    return result;
}


template <class Tp>
inline Tp* copy_trivial(const Tp* first, const Tp* last, Tp* result)
{
    memmove(result, first, sizeof(Tp) * (last - first));
    return result + (last - first);
}

template <class InputIter, class OutputIter, class BoolType>
struct copy_dispatch{
        static OutputIter copy(InputIter first, InputIter last, OutputIter result)
        {
            typedef typename iterator_traits<InputIter>::iterator_category Category;
            typedef typename iterator_traits<InputIter>::difference_type Distance;
            return copy(first, last, result, Category(), (Distance*)0);
        }
    };

template <class Tp>
struct copy_dispatch<Tp*, Tp*, __true_type>
{
    static Tp* copy(const Tp* first, const Tp* last, Tp* result)
    {  return copy_trivial(first, last, result);  }
};


template <class Tp>
struct copy_dispatch<const Tp*, Tp*, __true_type>
{
    static Tp* copy(const Tp* first, const Tp* last, Tp* result)
    {  return copy_trivial(first, last, result);  }
};

template <class InputIter, class OutputIter>
inline OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
    typedef typename iterator_traits<InputIter>::value_type Tp;
    typedef typename type_traits<Tp>::has_trivial_assignment_operator Trivial;
    return copy_dispatch<InputIter, OutputIter, Trivial>::copy(first, last, result);
}



// copy_backward

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                                  BidirectionalIter2 result, bidirectional_iterator_tag, Distance)
{
    while(first != last)
        *--result = *--last;
    return result;
}


template <class RandomAccessIter, class BidirectionalIter2, class Distance>
inline BidirectionalIter2 copy_backward(RandomAccessIter first, RandomAccessIter last,
                                          BidirectionalIter2 result, random_access_iterator_tag, Distance)
{
    for(Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}


template <class BidirectionalIter1, class BidirectionalIter2, class BoolType>
struct copy_backward_dispatch{
    typedef typename iterator_traits<BidirectionalIter1>::iterator_category Category;
    typedef typename iterator_traits<BidirectionalIter1>::difference_type Distance;
    static BidirectionalIter2 copy(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return copy_backward(first, last, result, Category(), (Distance*)0);
    }
};



template <class Tp>
struct copy_backward_dispatch<Tp*, Tp*, __true_type>
{
    static Tp* copy(const Tp* first, const Tp* last, Tp* result)
    {
        const ptrdiff_t distance = last - first;
        memmove(result - distance, first, sizeof(Tp) * distance);
        return result - distance;
    }
};


template <class Tp>
struct copy_backward_dispatch<const Tp*, Tp*, __true_type>
{
    static Tp* copy(const Tp* first, const Tp* last, Tp* result)
    {
        return copy_backward_dispatch<Tp*, Tp*, __true_type>::copy(first, last, result);
    }
};

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    typedef typename type_traits<typename iterator_traits<BidirectionalIter2>::value_type>
            ::has_trivial_assignment_operator Trival;
    return copy_backward_dispatch<BidirectionalIter1, BidirectionalIter2, Trival>::copy(first, last, result);
}



// copy_n
template <class InputIter, class Size, class OutputIter>
pair<InputIter, OutputIter> __copy_n(InputIter first, Size count, OutputIter result, input_iterator_tag)
{
    for ( ; count > 0; --count) {
        *result = *first;
        ++first;
        ++result;
    }
    return pair<InputIter, OutputIter>(first, result);
}

template <class RandomAccessIter, class Size, class OutputIter>
inline pair<RandomAccessIter, OutputIter> __copy_n(RandomAccessIter first, Size count, OutputIter result, random_access_iterator_tag)
{
    RandomAccessIter last = first + count;
    return pair<RandomAccessIter, OutputIter>(last, copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
inline pair<InputIter, OutputIter> __copy_n(InputIter first, Size count, OutputIter result)
{
    return copy_n(first, count, result, ITERATOR_CATEGORY(first));
}

template <class InputIter, class Size, class OutputIter>
inline pair<InputIter, OutputIter> copy_n(InputIter first, Size count, OutputIter result)
{
    return __copy_n(first, count, result);
}


// fill and fill_n

template <class ForwardIter, class _Tp>
void fill(ForwardIter first, ForwardIter last, const _Tp& __value) {
    for ( ; first != last; ++first)
        *first = __value;
}

template <class OutputIter, class Size, class _Tp>
OutputIter fill_n(OutputIter first, Size n, const _Tp& __value) 
{
    for ( ; n > 0; --n, ++first)
        *first = __value;
    return first;
}

inline void fill(unsigned char* first, unsigned char* last, const unsigned char& c)
{
    unsigned char tmp = c;
    memset(first, tmp, last - first);
}

inline void fill(signed char* first, signed char* last, const signed char& c)
{
    signed char tmp = c;
    memset(first, static_cast<unsigned char>(tmp), last - first);
}

inline void fill(char* first, char* last, const char& c) 
{
    char tmp = c;
    memset(first, static_cast<unsigned char>(tmp), last - first);
}


template <class Size>
inline unsigned char* fill_n(unsigned char* first, Size n, const unsigned char& c)
{
  fill(first, first + n, c);
  return first + n;
}

template <class Size>
inline signed char* fill_n(char* first, Size n, const signed char& c)
{
  fill(first, first + n, c);
  return first + n;
}

template <class Size>
inline char* fill_n(char* first, Size n, const char& c) 
{
  fill(first, first + n, c);
  return first + n;
}

// equal and mismatch

template <class _InputIter1, class _InputIter2>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2) {
    while (__first1 != __last1 && *__first1 == *__first2) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2,
                                        _BinaryPredicate __binary_pred) {
    while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2) {
    for ( ; __first1 != __last1; ++__first1, ++__first2)
        if (*__first1 != *__first2)
            return false;
    return true;
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _BinaryPredicate __binary_pred) {
    for ( ; __first1 != __last1; ++__first1, ++__first2)
        if (!__binary_pred(*__first1, *__first2))
            return false;
    return true;
}

//--------------------------------------------------
// lexicographical_compare and lexicographical_compare_3way.
// (the latter is not part of the C++ standard.)

template <class _InputIter1, class _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2) {
    for ( ; __first1 != __last1 && __first2 != __last2
            ; ++__first1, ++__first2) {
        if (*__first1 < *__first2)
            return true;
        if (*__first2 < *__first1)
            return false;
    }
    return __first1 == __last1 && __first2 != __last2;
}

template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __comp) {
    for ( ; __first1 != __last1 && __first2 != __last2
            ; ++__first1, ++__first2) {
        if (__comp(*__first1, *__first2))
            return true;
        if (__comp(*__first2, *__first1))
            return false;
    }
    return __first1 == __last1 && __first2 != __last2;
}

inline bool
lexicographical_compare(const unsigned char* __first1,
                        const unsigned char* __last1,
                        const unsigned char* __first2,
                        const unsigned char* __last2)
{
    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;
    const int __result = memcmp(__first1, __first2, min(__len1, __len2));
    return __result != 0 ? __result < 0 : __len1 < __len2;
}

inline bool lexicographical_compare(const char* __first1, const char* __last1,
                                    const char* __first2, const char* __last2)
{
    return lexicographical_compare((const signed char*) __first1,
                                   (const signed char*) __last1,
                                   (const signed char*) __first2,
                                   (const signed char*) __last2);
}

template <class _InputIter1, class _InputIter2>
int __lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                   _InputIter2 __first2, _InputIter2 __last2)
{
    while (__first1 != __last1 && __first2 != __last2) {
        if (*__first1 < *__first2)
            return -1;
        if (*__first2 < *__first1)
            return 1;
        ++__first1;
        ++__first2;
    }
    if (__first2 == __last2) {
        return !(__first1 == __last1);
    }
    else {
        return -1;
    }
}

inline int
__lexicographical_compare_3way(const unsigned char* __first1,
                               const unsigned char* __last1,
                               const unsigned char* __first2,
                               const unsigned char* __last2)
{
    const ptrdiff_t __len1 = __last1 - __first1;
    const ptrdiff_t __len2 = __last2 - __first2;
    const int __result = memcmp(__first1, __first2, min(__len1, __len2));
    return __result != 0 ? __result
                         : (__len1 == __len2 ? 0 : (__len1 < __len2 ? -1 : 1));
}

inline int
__lexicographical_compare_3way(const char* __first1, const char* __last1,
                               const char* __first2, const char* __last2)
{
    return __lexicographical_compare_3way(
            (const signed char*) __first1,
            (const signed char*) __last1,
            (const signed char*) __first2,
            (const signed char*) __last2);
}

template <class _InputIter1, class _InputIter2>
int lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                 _InputIter2 __first2, _InputIter2 __last2)
{
    return __lexicographical_compare_3way(__first1, __last1, __first2, __last2);
}

}

#endif //MYSTL_STL_ALGOBASE_H
