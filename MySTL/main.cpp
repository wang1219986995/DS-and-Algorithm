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
#include <map>
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
#include <unordered_map>

int main()
{
    cout << isIPV4("1.1.1.1") << endl;
    cout << isIPV4("111.12.78.1000") << endl;
    cout << isIPV4("1.1.1.1.2.23.4") << endl;
    cout << isIPV4("1.1.1.1") << endl;
    cout << isIPV4("1.1.1.1") << endl;
    string test = "123";
    cout << test.size() << " " << test.length() << endl;


    cout << CHAR_MAX << std::endl;
    cout << SCHAR_MAX << std::endl;


    std::vector<int> v = {3,2,1};
    std::cout << next_permutation(v.begin(), v.end()) << std::endl;
    std::cout << *v.begin() << std::endl;

    std::map<int, std::string> m;
    m[1] = "a";

    cout << "if elseif test :" << endl;
    int value = 0;
    if(value == 0)
        std::cout << value << endl;
    else if(value < 1)
        std::cout << value << endl;


}



