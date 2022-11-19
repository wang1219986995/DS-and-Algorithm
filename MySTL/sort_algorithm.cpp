
#include <vector>
#include <iostream>
#include "sort.h"

using namespace std;










int main()
{
    cout << "Hello!" <<endl;
    vector<int> v = {1, 5, 7, 10, 3, 88, 31, 2};

    shell_sort(v, [] (int x, int y) { return x < y; });



    for(auto iter : v)
        cout << iter << " ";
    return 0;
}