#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include "gtest/gtest.h"
#include "src/stl_vector.h"
#include "src/stl_map.h"

template <class Iterator1, class Iterator2>
struct predicate_test
{
public:
    bool operator()(Iterator1 x, Iterator2 y)
    { return (x.first == y.first) && (x.second == y.second); }
};

TEST(map_insert, int)
{
    MySTL::map<int, int> my_m;
    std::map<int, int> std_m;
    for(int i = 0; i < 100; ++i)
    {
        my_m.insert(MySTL::pair<int, int>(i, 2 * i));
        std_m.insert(std::pair<int, int>(i, 2 * i));
    }
    EXPECT_EQ(my_m.size(), std_m.size());
    EXPECT_TRUE(std::equal(std_m.begin(), std_m.end(), my_m.begin(),
                predicate_test<std::pair<int, int>, MySTL::pair<int, int>>()));
}


TEST(map_erase, int)
{
    MySTL::map<int, int> my_m;
    std::map<int, int> std_m;
    for(int i = 0; i < 100; ++i)
    {
        my_m.insert(MySTL::pair<int, int>(i, 2 * i));
        std_m.insert(std::pair<int, int>(i, 2 * i));
    }
    EXPECT_EQ(my_m.size(), std_m.size());
    EXPECT_TRUE(std::equal(std_m.begin(), std_m.end(), my_m.begin(),
                           predicate_test<std::pair<int, int>, MySTL::pair<int, int>>()));

    for( int i = 0; i < 100/4; ++i)
    {
        int n = rand() % my_m.size();
        my_m.erase(n);
        std_m.erase(n);
    }
    EXPECT_EQ(my_m.size(), std_m.size());
    EXPECT_TRUE(std::equal(std_m.begin(), std_m.end(), my_m.begin(),
                           predicate_test<std::pair<int, int>, MySTL::pair<int, int>>()));

    my_m.erase(my_m.begin(), my_m.end());
    std_m.erase(std_m.begin(), std_m.end());
    EXPECT_EQ(my_m.size(), std_m.size());
}

TEST(map_other, int)
{

}




//int main(int argc, char **argv)
//{
//    ::testing::InitGoogleTest(&argc, argv);
//    RUN_ALL_TESTS();
//
//    std::cout << "Press enter to exit..." << std::endl;
//    std::cin.get();
//    return 0;
//}