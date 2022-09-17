//
// Created by GW00243464 on 2022/9/1.
//


#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <queue>
#include <deque>
#include <stddef.h>
#include "gtest/gtest.h"
using namespace std;

class Base
{
public:

protected:
    int a;
};

class Derive : public Base
{
public:

    void set_a(int val) { a = val; }
};

class Derive2 : public Derive
{
public:
    void reset_a(int val) { a = val; }
};

int main()
{
    ptrdiff_t a;
    std::queue<int> q;
    std::deque<int> d;
    cout << "Hello!" <<endl;
    std::vector<int> v1 = {1,2,3,7};
    std::vector<int> v2 = {2,3, 4, 5};

    std::vector<int> res(v1.size() + v2.size());
    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());


    for(auto iter : res)
        cout << iter << " ";
    cout << endl;


    Derive2 test;
    test.reset_a(10);


    std::vector<int> v;
    std::generate_n(
            std::back_insert_iterator<std::vector<int>>(v), // C++17: std::back_insert_iterator(v)
            10, [n=0]() mutable { return ++n; }             // or use std::back_inserter helper
    );

    for (int n : v)
        std::cout << n << ' ';
    std::cout << '\n';

    return 0;
}
