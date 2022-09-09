//
// Created by GW00243464 on 2022/9/1.
//


#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <queue>
#include <deque>
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


    return 0;
}
