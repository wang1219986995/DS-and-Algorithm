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

template <typename T1> class base
{
public:
    T1 a;
};
// 普通类
class base00
{
public:
    int e;
};

// 2.Base和Derived类都为template类：derive类可不用typename T
template <typename T>
class derive:public base<T> //  当base作为一个模板类，需要向base传入参数类型，写法就是base<T>,而这个T又是由derive类传入的T作为参数
{
public:
    T b;
};

// 3.Base类为特定的template类
class derive2:public base<int> //  当base作为一个模板类，向base传入类型int
{
public:
    int c;
};

// 4.Derived类为template类
template <typename T>
class derive3 :public base00
{
public:
    T d;
};




class A
{
protected:
    int m_val;
};

class PA : public A {
public:
    void setVal(int val) {m_val = val;}
};

int main()
{
    PA pa;
    pa.setVal(1);
}

