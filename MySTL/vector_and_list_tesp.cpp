#include <iostream>
#include <deque>
#include <queue>
#include <vector>
#include <list>
#include "My_Heap.h"

// #include "src/alloc.h"
using namespace std;
bool condition_equal(int n)
{  return n == 1; }

int main() {

    std::list<int> l = { 1, 81, 42, 37, 14, 72, 36, 59, 1, 1};
    std::vector<int> v = {  81, 42, 37, 14, 72, 36, 59, 1, 1};

    cout << "This is a comparison between vector and list elements memory address:" << endl;
    std::cout << "List elements memory address(discontinuous address): " << std::endl;
    for(list<int>::iterator iter = l.begin(); iter != l.end(); ++iter)
        cout << &(*iter) << "  ";
    cout << endl;
    std::cout << "Vector elements memory address(continuous address): " << std::endl;
    for(vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter)
        cout << &(*iter) << "  ";
    cout << endl;



    cout << "------------------vector test-----------------------" << endl;
    cout << "Befor change vector's elements, view all elemnets first:" << endl;
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    cout << "insert() function will insert a element in the position:" << endl;
    v.insert(v.begin() + 1, 2);
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;
    cout << "This function will insert elements between two iterators:" << endl;
    vector<int> tmp = {3,4,5};
    v.insert(v.begin() + 2, tmp.begin(), tmp.end());
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    cout << "resize() function will reset vector's size:" << endl;
    v.resize(5);
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    std::cout << "vector's capacity and size is not same, size means current elements number, \n"
                 "capacity means current maximum number of elements that can ben stored." << std::endl;
    std::cout << "Such as, v.size() = " << v.size() << ", but v.capacity() = " << v.capacity() << std::endl;

    cout << "push_back() and pop_back() function can add or delete elements from vector's end:" << endl;
    v.push_back(80);
    v.push_back(81);
    v.push_back(82);
    v.push_back(83);
    v.push_back(84);
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    v.pop_back();
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    cout << "erase() and erase_if() function can delete elements:" << endl;
    cout << "First, we delete last element:" <<endl;
    v.erase(v.end() - 1);
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    cout << "Second, we delete all elements of greater than 80:" <<endl;
    std::erase_if(v, [](int n) { return n >= 80; });
    for(auto iter : v)
        cout << iter << " ";
    cout << endl;

    cout << "Last, we clear the vector:" <<endl;
    v.clear();
    std::cout << "vector size is: " << v.size() << endl;


    std::cout << std::endl;
    std::cout << std::endl;


    cout << "------------------list test-----------------------" << endl;

    cout << "Befor change list's elements, view all elemnets first:" << endl;
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;
    cout << "Delete and add elements from list's begin and end:" <<endl;
    l.pop_front();
    l.pop_back();
    l.pop_back();

    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    l.push_front(4);
    l.push_front(5);
    l.push_back(99);
    l.push_back(100);
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "erase function can delete elements by position:" << endl;
    list<int>::iterator range_begin = l.begin();
    list<int>::iterator range_end = l.end();
    advance(range_end, -1);
    cout << "delete one element:" << endl;
    l.erase(range_end);
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    advance(range_end, -1);
    advance(range_begin, 8);
    cout << "delete multiple elements:" << endl;
    l.erase(range_begin, range_end);
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "reverse list:" <<endl;
    l.reverse();
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "sort list:" <<endl;
    l.sort();
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;


    cout << "merge list:" <<endl;
    list<int> tmp_list = {0,0,0,0,0,0};
    l.merge(tmp_list);
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "unique list:" <<endl;
    l.unique();
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "remoue elements equal to 0" <<endl;
    l.remove(0);
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;

    cout << "remove elements that gerater than 80:" <<endl;
    struct condition{
    public:
        bool operator()(int n) { return n > 80; }
    };
    int count = l.remove_if(condition());
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;
    cout << "deleted elements count: " << count << endl;

    cout << "remove elements that gerater than 10:" <<endl;
    count = l.remove_if([](int n) { return n > 10; });
    for(auto iter : l)
        cout << iter << " ";
    cout << endl;
    cout << "deleted elements count: " << count << endl;

    cout << "clear the list:" << endl;
    l.clear();
    cout << "Now, the list size = " << l.size() << endl;


    return 0;
}
