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
#include <math.h>
#include <string.h>
#include <string>
#include "gtest/gtest.h"
using namespace std;


bool isIPV4(string str)
{
    if(str.size() < 7) return false;
    int start = 0;
    int count = 0;
    for(int i = 0; i < str.size(); ++i)
    {
        if(str[i] == '.')
        {
            string tmp = str.substr(start, i - start);
            int value = std::atoi(tmp.c_str());
            if(value < 1 || value > 255) return false;
            if(++count > 3) return false;
            start = i + 1;
        }
    }
    string tmp = str.substr(start, str.size());
    int value = std::atoi(tmp.c_str());
    if(value < 1 || value > 255) return false;
    return true;
}


int main()
{
    cout << isIPV4("1.1.1.1") << endl;
    cout << isIPV4("111.12.78.1000") << endl;
    cout << isIPV4("1.1.1.1.2.23.4") << endl;
    cout << isIPV4("1.1.1.1") << endl;
    cout << isIPV4("1.1.1.1") << endl;


    string test = "123";
    cout << test.size() << " " << test.length() << endl;

    std::vector<std::string> v = {"123", "234"};
    std::destroy(v.begin(), v.end());

}

