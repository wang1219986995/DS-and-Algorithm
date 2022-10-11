//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "src/stl_alloc.h"
using namespace std;

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)
namespace MySTL
{

TEST(default_alloc, private)
{
    default_alloc alloc;
    bool result = true;

    for(int i = 0; i < MySTL::NFREELISTS; ++i)
        if(*(alloc.S_free_list + i) != 0) result = false;
    EXPECT_TRUE(result);
    EXPECT_EQ(sizeof(alloc.S_free_list), 128);

    EXPECT_EQ((size_t) 8, alloc.S_round_up(1));
    EXPECT_EQ((size_t) 16, alloc.S_round_up(15));
    EXPECT_EQ((size_t) 16, alloc.S_round_up(9));
    EXPECT_EQ((size_t) 120, alloc.S_round_up(119));
    EXPECT_EQ((size_t) 128, alloc.S_round_up(121));
    EXPECT_EQ((size_t) 128, alloc.S_round_up(128));
    EXPECT_EQ((size_t) 128, alloc.S_round_up(127));

    EXPECT_EQ((size_t) 0, alloc.S_freelist_index(7));
    EXPECT_EQ((size_t) 1, alloc.S_freelist_index(16));
    EXPECT_EQ((size_t) 2, alloc.S_freelist_index(17));
    EXPECT_EQ((size_t) 15, alloc.S_freelist_index(121));
    EXPECT_EQ((size_t) 15, alloc.S_freelist_index(128));

    int objs = 40;
    char* res = alloc.S_chunk_alloc(8, objs);
    EXPECT_EQ(40 * 8, (unsigned int)(alloc.S_start_free - res));
    EXPECT_EQ(40 * 8, (unsigned int)(alloc.S_end_free - alloc.S_start_free));

    char* next_chunk = alloc.S_start_free;
    res = (char*) alloc.S_refill(8);
    EXPECT_EQ(next_chunk, res);
    typename MySTL::default_alloc::Obj* start = *(alloc.S_free_list + alloc.S_freelist_index(8));
    for(int i = 0; i < 19; ++i)
        ++start;
    EXPECT_EQ((alloc.S_start_free), (char*)start);
}


TEST(default_alloc, public)
{
    default_alloc alloc;
    void* first = alloc.allocate(8);

    void* second = alloc.allocate(8);
    EXPECT_EQ((uint64_t)first + 8, (uint64_t) second);

    alloc.deallocate(second, 8);
    EXPECT_EQ((uint64_t)second, (uint64_t)*(alloc.S_free_list + alloc.S_freelist_index(8)));

    int* tmp = (int*)first;
    for(int i = 0; i < 2; ++i)
    {
        *tmp = i;
        ++tmp;
    }
    first = alloc.reallocate(first, 8, 16);
    tmp = (int*)first;
    EXPECT_EQ(0, *tmp ++);
    EXPECT_EQ(1, *tmp);

}

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    cout << "Press enter to exit..." << endl;
    cin.get();
    return 0;
}



