//

#ifndef MYSTL_STL_HEAP_H
#define MYSTL_STL_HEAP_H
#include "stl_iterator.h"
namespace MySTL
{

template <class RandomAccessIterator, class Distance, class Tp>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Tp value)
{
    Distance parent = (holeIndex - 1) / 2;
    while(holeIndex > topIndex && *(first + parent) < value)
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
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



template <class RandomAccessIterator, class Distance, class Tp, class Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex,
                 Tp value, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while(holeIndex > topIndex && comp(*(first + parent), value))
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Compare, class Distance, class Tp>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, Tp*)
{
    __push_heap(first, Distance((last - first) - 1), Distance(0), Tp(*(last - 1)), comp);
}

template <class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __push_heap_aux(first, last, comp, DISTANCE_TYPE(first), VALUE_TYPE(first));
}















template <class RandomAccessIterator, class Distance, class Tp>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Tp value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while(secondChild < len)
    {
        if(*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild --;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if(secondChild == len)
    {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator, class Tp, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                       RandomAccessIterator result, Tp value, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last-first), value);
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




template <class RandomAccessIterator, class Distance, class Tp, class Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Tp value, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while(secondChild < len)
    {
        if(comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if(secondChild == len)
    {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}

template <class RandomAccessIterator, class Tp, class Compare, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result,
                       Tp value, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <class RandomAccessIterator, class Tp, class Compare>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Tp*, Compare comp)
{
    __pop_heap(first, last - 1, last - 1, Tp(*(last - 1)), comp, DISTANCE_TYPE(first));
}

template <class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __pop_heap_aux(first, last, VALUE_TYPE(first), comp);
}





template <class RandomAccessIterator, class Tp, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Tp*, Distance*)
{
    if(last - first < 2)    return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;

    while(true)
    {
        __adjust_heap(first, parent, len, Tp(*(first + parent)));
        if(parent == 0) return;
        parent--;
    }
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, VALUE_TYPE(first), DISTANCE_TYPE(first));
}


template <class RandomAccessIterator, class Compare, class Tp, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Tp*, Distance*)
{
    if(last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;

    while(true)
    {
        __adjust_heap(first, parent, len, Tp(*(first + parent)), comp);
        if(parent == 0) return;
        parent--;
    }
}

template <class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __make_heap(first, last, comp, VALUE_TYPE(first), DISTANCE_TYPE(first));
}



template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while(last - first > 1)
        pop_heap(first, last --);
}


template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    while(last - first > 1)
        pop_heap(first, last--, comp);
}




} // namespace

#endif //MYSTL_STL_HEAP_H
