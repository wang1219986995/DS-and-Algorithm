#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <memory>
#include <queue>
#include <deque>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <string>
#include <map>
#include "gtest/gtest.h"
#include <unordered_map>
#include "stdio.h"
#include "btree_set.h"
#include "btree_map.h"



#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class node
{
public:
    struct leaf
    {
        int a;
        int b;
        char c;
        double d;
    };

    int nums[10];
};

template <class Key>
class rb_tree
{ };

template <class Key>
class avl_tree
{ };

template <class Key, class Container=rb_tree<Key>>
class test_set
{
public:
    Container c;
};

class Test
{
public:
    int& get_reference()
    {
        return num;
    }
public:
    int num;
};

typedef vector<int>::allocator_type IntAlloc;
int main( )
{
    btree::btree_set<int> s;
    s.insert(10);
    cout << "1" << endl;
    s.insert(10);
    s.insert(8);
    for(int i = 0; i < 10000; ++i)
        s.insert(i);

}




