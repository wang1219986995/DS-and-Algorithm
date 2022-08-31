//
// Created by GW00243464 on 2022/8/25.
//

#ifndef MYSTL_MY_HEAP_CPP
#define MYSTL_MY_HEAP_CPP

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <queue>

using namespace std;

#define PRINT_ELEMENTS(container) \
    for(auto iter : container)    \
        cout << iter << " ";       \
    cout << endl;



template <class Tp, class Cmp>
class My_Heap
{
public:
    My_Heap() { make_heap(); }
    My_Heap(std::vector<Tp>& v) : elements(v) { make_heap(); }

    size_t parent(size_t i) { return (i-2)/2; }
    size_t left_child(size_t i) { return 2 * i + 2; }
    size_t right_child(size_t i) { return 2 * i + 1; }
    size_t size() const { return elements.size(); }
    Tp top() { return elements[0]; }
    void push(const Tp& val)
    {
        elements.push_back(val);
        size_t index = size() - 1;
        while (index > 0)
        {
            if(Cmp()(elements[index], elements[parent(index)]))
            {
                std::swap(elements[parent(index)], elements[index]);
                index = parent(index);
            }
            else break;
        }
    }

    Tp pop()
    {
        Tp res = elements[0];
        std::swap(elements[0], elements[size() - 1]);
        elements.pop_back();
        maintain_heap(0);
        return res;
    }

private:
    void maintain_heap(size_t i);
    Tp& operator[](size_t i) { return elements[i]; }
    void make_heap()
    {
        for(int i = size() / 2; i >= 0 ; --i)
        {
            maintain_heap(i);
        }
    }

private:
    std::vector<Tp> elements;
};

template <class Tp, class Cmp>
void My_Heap<Tp, Cmp>::maintain_heap(size_t i)
{
    size_t largest = i;
    size_t left, right;
    left = left_child(i);
    right = right_child(i);

    if(left < elements.size() && Cmp()(elements[left], elements[largest]))
        largest = left;

    if(right < elements.size() && Cmp()(elements[right], elements[largest]))
        largest = right;

    if(largest != i)
    {
        std::swap(elements[i], elements[largest]);
        maintain_heap(largest);
    }
}



int main()
{
    std::vector<int> numbers = { 33, 510, 834, 40, 111, 923, 700, 666};
    My_Heap<int, std::less<int>> heap(numbers);
    heap.push(222);
    heap.push(333);
    heap.push(555);

    vector<int> res;
    int size = heap.size();
    for(int i = 0; i < size; ++i)
        res.push_back(heap.pop());
    cout << "heap sort result:" << endl;
    PRINT_ELEMENTS(res);

    cout << endl;
    cout << "The custom heap sort:" << endl;
    struct stringlength_less{
        bool operator()(const std::string& a, const std::string& b)
        {  return a.length() < b.length(); }
    };

    vector<std::string> strs = {"sdfasdf", "sdfasdfasd", "asdfasdfpppppppppppdfsdfsdfsdfsd", "iiwiwiwisdfadsfa", "a", "sdki", "we"};
    My_Heap<std::string, stringlength_less> string_heap(strs);
    vector<std::string> string_res;
    size = string_heap.size();
    for(int i = 0; i < size; ++i)
        string_res.push_back(string_heap.pop());
    cout << "String heap sort result:" << endl;
    for(auto iter : string_res)
        cout << iter << endl;

}





#endif //MYSTL_MY_HEAP_CPP
