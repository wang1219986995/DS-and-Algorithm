//
// Created by GW00243464 on 2022/9/6.
//

#ifndef MYSTL_SORT_H
#define MYSTL_SORT_H

#include <iostream>
#include <vector>
using namespace std;

template <class Tp>
void bubble_sort(vector<Tp>& numbers)
{
    for(int i = 0; i < numbers.size() - 1; ++i)
    {
        for(int j = 0; j < numbers - 1; ++j)
        {
            if(numbers[j] > numbers[j+1])
                swap(numbers[j], numbers[j+1]);
        }
    }
}


template <class Tp, class Compare>
void selection_sort(vector<Tp>& numbers, Compare comp = std::less<Tp>())
{
    int min_index;
    for(int i = 0; i < numbers.size() - 1; ++i)
    {
        min_index = i;
        for(int j = i + 1; j < numbers.size(); ++j)
        {
            if(comp(numbers[j], numbers[i]))
                min_index = j;
        }
        if(min_index != i)
            swap(numbers[i], numbers[min_index]);
    }
}


template <class Tp, class Compare>
void insertion_sort(vector<Tp>& numbers, Compare comp)
{
    for(int i = 1; i < numbers.size(); ++i)
    {
        int index = i - 1;
        while(index >= 0 && comp(numbers[i], numbers[index]))
            index --;

        if(index != i - 1)
        {
            ++index;
            Tp tmp = numbers[i];
            std::copy(numbers.begin() + index, numbers.begin() + i, numbers.begin() + index + 1);
            numbers[index] = tmp;
        }
    }
}


template <class Tp, class Compare>
void shell_sort(vector<Tp>& numbers, Compare comp)
{
    for(int gap = numbers.size() / 2; gap >= 1; gap = gap / 2)
    {
        for(int i = gap; i < numbers.size(); ++i)
        {
            Tp tmp = numbers[i];
            int index = i - gap;
            while(index >= 0 && comp(tmp, numbers[index]))
            {
                numbers[index + gap] = numbers[index];
                index -= gap;
            }
            numbers[index + gap] = tmp;
        }
    }
}

//TODO 待测试
template <class Tp, class Compare>
void merge_sort_aux(vector<Tp>& numbers, int first, int last, Compare comp)
{
    if(first >= last) return;
    int len = last - first + 1;
    int middle = len / 2;
    int start1 = first, end1 = middle;
    int start2 = middle + 1, end2 = last;
    merge_sort_aux(numbers, start1, end1, comp);
    merge_sort_aux(numbers, start2, end2, comp);

    vector<Tp> res(len);
    int i = start1, j = start2, index = 0;
    while(true)
    {
        while(i <= end1 && j <= end2 && comp(numbers[i], numbers[j]) && )
            res[index] = numbers[i++];
        while(i <= end1 && j <= end2 && comp(numbers[j], numbers[i]))
            res[index] = numbers[j++];

        if(i > end1)
        {
            std::copy(numbers.begin() + j, numbers.begin() + end2 + 1, res.begin() + index);
            break;
        }
        else if(j > end2)
        {
            std::copy(numbers.begin() + i, numbers.begin() + end1 + 1, res.begin() + index);
            break;
        }
    }
    std::copy(res.begin(), res.end(), numbers.begin() + first);
}

template <class Tp, class Compare>
void merge_sort(vector<Tp>& numbers, Compare comp)
{
    merge_sort_aux(numbers, 0, numbers.size() - 1, comp);
}



template <class Tp, class Compare>
int paritition(vector<Tp>& numbers, int first, int last, Compare comp)
{
    Tp pivot = numbers[first];
    while(first < last)
    {
        while(first < last && numbers[last] >= pivot)
            --last;
        numbers[first] = numbers[last];

        while(first < last && numbers[first] <= pivot)
            ++first;
        numbers[last] = numbers[first];
    }
    numbers[first] = pivot;
    return first;
}

template <class Tp, class Compare>
void quick_sort_aux(vector<Tp>& numbers, int first, int last, Compare comp)
{
    if(first < last)
    {
        int pivot_index = paritition(numbers, 0, numbers.size() - 1, comp);
        quick_sort_aux(numbers, first, pivot_index - 1, comp);
        quick_sort_aux(numbers, pivot_index + 1, last, comp);
    }
}

template <class Tp, class Compare>
void quick_sort(vector<Tp>& numbers, Compare comp)
{
    quick_sort_aux(numbers, 0, numbers.size() - 1, comp);
}




// heap sort



















#endif //MYSTL_SORT_H
