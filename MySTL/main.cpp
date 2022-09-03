//
// Created by GW00243464 on 2022/9/1.
//


#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;



int main()
{
    cout << "Hello!" <<endl;
    cout << CHAR_MAX << " " << SCHAR_MAX <<endl;
    std::vector<int> v = {1,1,2,3,4,5,6,1};

    std::unique(v.begin(), v.end());
    for(auto iter : v)
        cout << iter << " ";




    return 0;
}
