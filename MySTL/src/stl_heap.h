//
// Created by GW00243464 on 2022/9/8.
//

#ifndef MYSTL_STL_HEAP_H
#define MYSTL_STL_HEAP_H
#include "stl_iterator.h"
namespace MySTL
{

template <class RandomAccessIterator, class Distance, class Tp>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Tp value)
{

}



template <class RandomAccessIterator, class Distance, class Tp>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, Tp*)
{
    __push_heap((first, Distance(first - last) - 1), Distance(0), Tp(*(last - 1)));
}

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __push_heap_aux(first, last, DISTANCE_TYPE(first), VALUE_TYPE(first));
}









template <class RandomAccessIterator, class Distance, class Tp>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Tp value)
{

}

template <class RandomAccessIterator, class Tp, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                       RandomAccessIterator result, Tp value, Distance*)
{

}


template <class RandomAccessIterator, class Tp>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Tp*)
{
    __pop_heap(first, last - 1, last - 1, Tp(*(last - 1)), DISTANCE_TYPE(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __pop_heap_aux(first, last, VALUE_TYPE(first));
}










template <class RandomAccessIterator, class Tp, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Tp*, Distance*)
{

}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, VALUE_TYPE(first), DISTANCE_TYPE(first));
}









template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while(last - first > 1)
        pop_heap(first, last--);
}




}

#endif //MYSTL_STL_HEAP_H
