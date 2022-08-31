#include <iostream>
#include <deque>
#include <queue>
#include <stack>
#include <vector>
#include <list>
#include <string>

using namespace std;





// 括号匹配
// 一组包含 { } , [ ], ( ) 三种括号的算式，判断算式中括号是否匹配
bool BracketMatching(string str)
{
    std::stack<char> s;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == '{' || str[i] == '[' || str[i] == '(')
            s.push(str[i]);
        else if(str[i] == '}' || str[i] == ']' || str[i] == ')')
        {
            if(str[i] == '}' && !s.empty() && s.top() == '{')  s.pop();
            else if(str[i] == ']' && !s.empty() && s.top() == '[')  s.pop();
            else if(str[i] == ')' && !s.empty() && s.top() == '(')  s.pop();
            else return false;
        }
    }
    return s.empty();
}





// 实现一个类用于计算数据流冲固定窗口中的数据的平均值
// MovingAverage(int size): size 为窗口大小
// next(int value): 传入数据流的下一个整数value
class MovingAverage
{
public:
    MovingAverage(int size) : size(size), sum(0) {}
    double next(int value)
    {
        if(q.size() > size)
        {
            sum -= q.front();
            q.pop();
        }
        q.push(value);
        sum += value;
        return double(sum)/q.size();
    }

private:
    int size;
    queue<int> q;
    int64_t sum;
};








int main() {
    cout << "------------------stack test--------------------" << endl;
    std::vector<std::pair<string, bool>> stack_test_string = {
            {"8 + (1 + 2) + 1",             true},
            {"{8 + [3 * (1 + 4)]}",         true},
            {") + 1",                       false},
            {"3 + { 8 / [8 + 23 (9 - 2]",   false}
    } ;

    for(int i = 0; i < stack_test_string.size(); i++)
    {
        bool res = BracketMatching(stack_test_string[i].first);
        if(res)
            cout << i + 1 << "th test successed." <<endl;
        else
            cout << i + 1 << "th test failed." << endl;
    }


    cout << "------------------queue test--------------------" << endl;
    MovingAverage movingAverage(5);
    cout << "moving average is:" << movingAverage.next(1) << endl;
    cout << "moving average is:" << movingAverage.next(10) << endl;
    cout << "moving average is:" << movingAverage.next(6) << endl;
    cout << "moving average is:" << movingAverage.next(32) << endl;
    cout << "moving average is:" << movingAverage.next(48) << endl;
    cout << "moving average is:" << movingAverage.next(19) << endl;





    return 0;
}