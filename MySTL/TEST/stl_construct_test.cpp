//

#include "gtest/gtest.h"
#include "src/stl_construct.h"
#include <vector>
#include <algorithm>
#include <iostream>




class Base {
public:
    Base(int val) : value(val) {}
    Base() : value(0) {}
    ~Base() { value = -1; }

    int getValue() { return value; }

    bool operator==(const Base& b)
    {  return value == b.value; }
private:
    int value;
};



TEST(Construct, All)
{
    Base b(10);
    Base* pb = (Base*)malloc(sizeof(Base));
    MySTL::construct(pb);
    EXPECT_EQ(pb->getValue(), 0);
    MySTL::construct(pb, Base(10));
    EXPECT_EQ(pb->getValue(), 10);

    std::vector<Base> v(10);
    std::vector<Base> test(10);
    for(int i = 0; i < v.size(); ++ i)
    {
        MySTL::construct(&test[i], Base(i));
        v[i] = Base(i);
    }
    EXPECT_TRUE(std::equal(test.begin(), test.end(), v.begin()));
}


TEST(destroy, All)
{
    std::vector<Base> v(10);
    std::vector<Base> test(10);
    std::vector<Base> samples(10);

    Base* pbs = (Base*)(malloc(10 * sizeof(Base)));
    for(int i = 0; i < 10; ++i)
    {
        MySTL::construct(&pbs[i], Base(i));
        MySTL::construct(&test[i], Base(i));
        samples[i] = Base(i);
    }

    for(int i = 0; i < 10; ++i)
        MySTL::destroy(&pbs[i]);
    MySTL::destroy(test.begin(), test.end());
    std::destroy(samples.begin(), samples.end());
    EXPECT_TRUE(std::equal(pbs, pbs + 10, samples.begin()));
    EXPECT_TRUE(std::equal(test.begin(), test.end(), samples.begin()));

}


int main(int argc, char **argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    std::cin.get();


    return 0;
}
