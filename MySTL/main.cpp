#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <memory>
#include <queue>
#include <deque>
#include <stddef.h>
#include <math.h>
#include <functional>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "gtest/gtest.h"
#include "stdio.h"
#include "src/stl_alloc.h"
#include "src/stl_btree.h"
#include "src/stl_map.h"


class HashTest
{
public:
    int a;
    int b;
    int c;

    bool operator==(const HashTest& x)
    {
        return a==x.a && b==x.b && c==x.c;
    }
};

inline bool operator==(const HashTest& x, const HashTest& y)
{
    return x.a==y.a && x.b==y.b && x.c==y.c;
}


int main( )
{



    MySTL::map<int, int> m;
    m.insert(MySTL::pair<int, int>(1, 10));


    std::cout << "finish" << std::endl;

}




