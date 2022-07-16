#include <iostream>
#include "sort.h"
using namespace std;









int main()
{
    vector<int> values = {1,3,4,6,9,2,12,7,333,211,245,145,198};
    inseration_sort<int>(values);
    for(auto ite : values)
    {
        cout << ite << " ";
    }
    cout << endl;




    cout << "Hello World!" << endl;
    return 0;
}
