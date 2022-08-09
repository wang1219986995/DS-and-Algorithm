#ifndef SORT_H
#define SORT_H
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
void inseration_sort(std::vector<T> &values)
{
    for(int i = 1; i < values.size(); ++i)
    {
        std::cout  << i << ":" << values[i] << std::endl;
        T key = values[i];
        for(int j = i - 1; j >= 0; --j)
        {
            if(values[j] >= key && values[j+1] < key)
            {
                std::copy(&values[j+1], &values[i-1], &values[j+2]);
                values[j+1] = key;
                break;
            }
        }
    }
}



template <typename T>
void merge_sort(std::vector<T> &values)
{


}













#endif // SORT_H
