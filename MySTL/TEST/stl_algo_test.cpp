#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "src/stl_alloc.h"
#include "src/stl_algo.h"
#include "src/stl_vector.h"

using namespace std;

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)

TEST(algorithm, min_and_max)
{
    EXPECT_EQ(MySTL::min<int>(1,3), 1);
    EXPECT_EQ(MySTL::max<int>(1,3), 3);

    struct compare
    { bool operator()(int a, int b) { return a < b; } };

    EXPECT_EQ(MySTL::min<int>(1,4, compare()), 1);
    EXPECT_EQ(MySTL::max<int>(1,3, compare()), 3);
}



class TestClass {
public:
    int value;
    TestClass() {}
    TestClass(int val) : value(val) {}
};
bool operator ==(const TestClass& a, const TestClass&b) { return a.value == b.value; }

TEST(algorithm, copy_and_fill)
{
    int* numbers = (int*) malloc(101 * sizeof(int));
    for(int i = 0; i < 100; ++i)
        numbers[i] = i;

    std::vector<int> v(100);
    MySTL::vector<int> test(100);

    std::copy(numbers, numbers + 100, v.begin());
    MySTL::copy(numbers, numbers + 100, test.begin());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    TestClass* base = (TestClass*) malloc(101 * sizeof(TestClass));
    for(int i = 0; i < 100; ++i)
        base[i] = TestClass(i);

    std::vector<TestClass> v2(150);
    MySTL::vector<TestClass> test2(150);
    std::copy_backward(base, base + 100, v2.end());
    MySTL::copy_backward(base, base + 100, test2.end());
    std::copy_n(base, 50, v2.begin());
    MySTL::copy_n(base, 50, test2.begin());
    EXPECT_TRUE(std::equal(v2.begin(), v2.end(), test2.begin()));


    std::vector<TestClass> v3(100);
    MySTL::vector<TestClass> test3(100);
    std::fill(v3.begin(), v3.begin() + 50, TestClass(10));
    MySTL::fill(test3.begin(), test3.begin() + 50, TestClass(10));
    std::fill_n(v3.begin() + 50, 50, TestClass(100));
    MySTL::fill_n(test3.begin() + 50, 50, TestClass(100));
    EXPECT_TRUE(std::equal(v3.begin(), v3.end(), test3.begin()));
}

TEST(algorithm, search)
{
    std::vector<int> v(150, 100);
    MySTL::vector<int> test(150,100);
    for(int i = 0; i < 100; ++i)
    {
        v[i] = i;
        test[i] = i;
    }
    int numbers[5] = {31,32,33,34,35};
    EXPECT_TRUE(std::equal(
            std::search(v.begin(), v.end(), numbers, numbers + 5), v.end(),
            MySTL::search(test.begin(), test.end(), numbers, numbers + 5)));

    int value = 100;
    EXPECT_TRUE(std::equal(
            std::search_n(v.begin(), v.end(), 10, value), v.end(),
            MySTL::search_n(test.begin(), test.end(), 10, value)));
}

TEST(algorithm, rotate)
{
    std::vector<int> v(100);
    MySTL::vector<int> test(100);
    for(int i = 0; i < 100; ++i)
    {
        v[i] = i;
        test[i] = i;
    }
    std::rotate(v.begin(), v.begin() + 30, v.end());
    MySTL::rotate(test.begin(), test.begin() + 30, test.end());
}


TEST(algorithm, partition)
{
    struct predicate
    {
        bool operator()(int value) { return value < 500; }
    };

    std::vector<int> v(300);
    MySTL::vector<int> test(300);
    for(int i = 0; i < 300; ++i)
    {
        int num = std::rand() % 1000;
        v[i] = num;
        test[i] = num;
    }
    std::partition(v.begin(), v.end(), predicate());
    MySTL::partition(test.begin(), test.end(), predicate());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    std::for_each(v.begin(), v.end(), [](int& val) { val -= 500; });
    MySTL::for_each(test.begin(), test.end(), [](int& val) { val -= 500; });
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));
}

TEST(algorithm, sort)
{
    std::vector<int> v(10000);
    MySTL::vector<int> test(10000);
    for(int i = 0; i < 10000; ++i)
    {
        int num = std::rand() % 10000;
        v[i] = num;
        test[i] = num;
    }


    std::vector<int> v1(v);
    MySTL::vector<int> test1(test);
    std::sort(v1.begin(), v1.end());
    MySTL::sort(test1.begin(), test1.end());
    EXPECT_TRUE(std::equal(v1.begin(), v1.end(), test1.begin()));

    v1 = v;
    test1 = test;
    std::stable_sort(v1.begin(), v1.end());
    MySTL::stable_sort(test1.begin(), test1.end());
    EXPECT_TRUE(std::equal(v1.begin(), v1.end(), test1.begin()));

    v1 = v;
    test1 = test;

    auto iter_v = v1.begin() + 5000;
    auto iter_test = test1.begin() + 5000;
    std::nth_element(v1.begin(), iter_v, v1.end());
    MySTL::nth_element(test1.begin(), iter_test, test1.end());
    EXPECT_EQ(*iter_v, * iter_test);

    std::vector<int> v2(10000);
    MySTL::vector<int> test2(10000);
    std::partial_sort_copy(v1.begin(), v1.end(), v2.begin(), v2.end());
    MySTL::partial_sort_copy(test1.begin(), test1.end(), test2.begin(), test2.end());
    EXPECT_TRUE(std::equal(v2.begin(), v2.end(), test2.begin()));
}

TEST(algorithm, binary_search)
{
    std::vector<int> v(10000);
    MySTL::vector<int> test(10000);
    for(int i = 0; i < 10000; ++i)
    {
        int num = std::rand() % 10000;
        v[i] = num;
        test[i] = num;
    }

    std::sort(v.begin(), v.end());
    MySTL::sort(test.begin(), test.end());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));
    auto iter_v = std::lower_bound(v.begin(), v.end(), 5000);
    auto iter_test = MySTL::lower_bound(test.begin(), test.end(), 5000);
    EXPECT_EQ(*iter_v, *iter_test);

    iter_v = std::upper_bound(v.begin(), v.end(), 5000);
    iter_test = MySTL::upper_bound(test.begin(), test.end(), 5000);
    EXPECT_EQ(*iter_v, *iter_test);


    auto v_pair = std::equal_range(v.begin(), v.end(), 5000);
    auto test_pair = MySTL::equal_range(test.begin(), test.end(), 5000);
    EXPECT_TRUE(std::equal(v_pair.first, v_pair.second, test_pair.first));
}

TEST(algorithm, merge)
{
    std::vector<int> v1(1000);
    std::vector<int> v2(1000);
    std::vector<int> v3(1000);
    MySTL::vector<int> test1(1000);
    MySTL::vector<int> test2(1000);
    MySTL::vector<int> test3(1000);

    for(int i = 0; i < 1000; ++i)
    {
        int num = std::rand() % 10000;
        v1[i] = num;
        v2[i] = num * 2;
        test1[i] = num;
        test2[i] = num * 2;
    }
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    MySTL::sort(test1.begin(), test1.end());
    MySTL::sort(test2.begin(), test2.end());

    std::vector<int> v_res(2000);
    MySTL::vector<int> test_res(2000);
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v_res.begin());
    MySTL::merge(test1.begin(), test1.end(), test2.begin(), test2.end(), test_res.begin());
    EXPECT_TRUE(std::equal(v_res.begin(), v_res.end(), test_res.begin()));


    std::sort(v3.begin(), v3.begin() + v3.size() / 2);
    std::sort(v3.begin() + v3.size() / 2, v3.end());
    MySTL::sort(test3.begin(), test3.begin() + test3.size() / 2);
    MySTL::sort(test3.begin() + test3.size() / 2, test3.end());
    std::inplace_merge(v3.begin(), v3.begin() + v3.size()/2, v3.end());
    MySTL::inplace_merge(test3.begin(), test3.begin() + test3.size()/2, test3.end());
    EXPECT_TRUE(std::equal(v3.begin(), v3.end(), test3.begin()));



}


TEST(algorithm, include_and_set)
{
    std::vector<int> v1(1000);
    MySTL::vector<int> test1(1000);

    for(int i = 0; i < 1000; ++i)
    {
        int num = std::rand() % 500;
        v1[i] = num;
        test1[i] = num;
    }
    std::sort(v1.begin(), v1.end());
    MySTL::sort(test1.begin(), test1.end());

    std::vector<int> v_sample = { 1,2,3 };
    MySTL::vector<int> test_sample(3);
    std::copy(v_sample.begin(), v_sample.end(), test_sample.begin());

    bool v_res = std::includes(v1.begin(), v1.end(), v_sample.begin(), v_sample.end());
    bool test_res = std::includes(test1.begin(), test1.end(), test_sample.begin(), test_sample.end());
    EXPECT_EQ(v_res, test_res);


    std::vector<int> v2 = {1, 2, 3, 4, 5};
    std::vector<int> v3 = {      3, 4, 5, 6, 7};
    std::vector<int> v_dest1(10, 0);

    MySTL::vector<int> test2(5);
    MySTL::vector<int> test3(5);
    std::copy(v2.begin(), v2.end(), test2.begin());
    std::copy(v3.begin(), v3.end(), test3.begin());
    MySTL::vector<int> test_dest1(10, 0);
    std::set_union(v2.begin(), v2.end(), v3.begin(), v3.end(), v_dest1.begin());
    MySTL::set_union(test2.begin(), test2.end(), test3.begin(), test3.end(), test_dest1.begin());
    EXPECT_TRUE(std::equal(v_dest1.begin(), v_dest1.end(), test_dest1.begin()));


    std::vector<int> v_dest2(10, 0);
    MySTL::vector<int> test_dest2(10, 0);
    std::set_intersection(v2.begin(), v2.end(), v3.begin(), v3.end(), v_dest2.begin());
    MySTL::set_intersection(test2.begin(), test2.end(), test3.begin(), test3.end(), test_dest2.begin());
    EXPECT_TRUE(std::equal(v_dest2.begin(), v_dest2.end(), test_dest2.begin()));

    std::vector<int> v_dest3(10, 0);
    MySTL::vector<int> test_dest3(10, 0);
    std::set_difference(v2.begin(), v2.end(), v3.begin(), v3.end(), v_dest3.begin());
    MySTL::set_difference(test2.begin(), test2.end(), test3.begin(), test3.end(), test_dest3.begin());
    EXPECT_TRUE(std::equal(v_dest3.begin(), v_dest3.end(), test_dest3.begin()));

    std::vector<int> v_dest4(10, 0);
    MySTL::vector<int> test_dest4(10, 0);
    std::set_symmetric_difference(v2.begin(), v2.end(), v3.begin(), v3.end(), v_dest4.begin());
    MySTL::set_symmetric_difference(test2.begin(), test2.end(), test3.begin(), test3.end(), test_dest4.begin());
    EXPECT_TRUE(std::equal(v_dest4.begin(), v_dest4.end(), test_dest4.begin()));
}

TEST(algorithm, other)
{
    std::vector<int> v1(1000);
    MySTL::vector<int> test1(1000);

    for(int i = 0; i < 1000; ++i)
    {
        int num = std::rand() % 1000;
        v1[i] = num;
        test1[i] = num;
    }

    auto iter_v = std::max_element(v1.begin(), v1.end());
    auto iter_test = MySTL::max_element(test1.begin(), test1.end());
    EXPECT_EQ(*iter_v, *iter_test);

    iter_v = std::min_element(v1.begin(), v1.end());
    iter_test = MySTL::min_element(test1.begin(), test1.end());
    EXPECT_EQ(*iter_v, *iter_test);


    std::vector<char> s = {'a', 'b', 'c', 'd'};
    MySTL::vector<char> test(4);
    std::copy(s.begin(), s.end(), test.begin());

    std::next_permutation(s.begin(), s.end());
    MySTL::next_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));

    std::next_permutation(s.begin(), s.end());
    MySTL::next_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));

    std::next_permutation(s.begin(), s.end());
    MySTL::next_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));

    std::prev_permutation(s.begin(), s.end());
    MySTL::prev_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));

    std::prev_permutation(s.begin(), s.end());
    MySTL::prev_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));

    std::prev_permutation(s.begin(), s.end());
    MySTL::prev_permutation(test.begin(), test.end());
    EXPECT_TRUE(std::equal(s.begin(), s.end(), test.begin()));


    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    test1.push_back(1);
    test1.push_back(2);
    test1.push_back(3);

    std::vector<int> v_sample = {1,2,3};
    MySTL::vector<int> test_sample(3);
    std::copy(v_sample.begin(), v_sample.end(), test_sample.begin());

    auto v_iter = std::find_first_of(v1.begin(), v1.end(), v_sample.begin(), v_sample.end());
    auto test_iter = MySTL::find_first_of(test1.begin(), test1.end(), test_sample.begin(), test_sample.end());
    EXPECT_EQ(*v_iter, *test_iter);

    v_iter = std::find_end(v1.begin(), v1.end(), v_sample.begin(), v_sample.end());
    test_iter = MySTL::find_end(test1.begin(), test1.end(), test_sample.begin(), test_sample.end());
    EXPECT_EQ(*v_iter, *test_iter);


    EXPECT_EQ(std::is_heap(v1.begin(), v1.end()), MySTL::is_heap(test1.begin(), test1.end()));
    std::make_heap(v1.begin(), v1.end());
    MySTL::make_heap(test1.begin(), test1.end());
    EXPECT_EQ(std::is_heap(v1.begin(), v1.end()), MySTL::is_heap(test1.begin(), test1.end()));

    EXPECT_EQ(std::is_sorted(v1.begin(), v1.end()), MySTL::is_sorted(test1.begin(), test1.end()));
    std::sort(v1.begin(), v1.end());
    MySTL::sort(test1.begin(), test1.end());
    EXPECT_EQ(std::is_sorted(v1.begin(), v1.end()), MySTL::is_sorted(test1.begin(), test1.end()));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    cout << "Press enter to exit..." << endl;
    cin.get();
    return 0;
}



