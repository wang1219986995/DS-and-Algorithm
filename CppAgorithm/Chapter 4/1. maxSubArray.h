#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


// 最大连续子数组：递归实现
int maxSubArray(vector<int>& nums) {
    if (nums.size() <= 0)    return 0;
    return maxSubArrayAux(nums, 0, nums.size() - 1);
}

int maxSubArrayAux(vector<int>& nums, int low, int high)
{
    cout << "Aux: low-high = " << low << "-" << high << endl;
    if (low == high) return nums[low];
    else
    {
        int mid = (low + high) / 2;
        int left_sum = maxSubArrayAux(nums, low, mid);
        int right_sum = maxSubArrayAux(nums, mid + 1, high);
        int cross_sum = maxSubArrayCrossSum(nums, low, mid, high);

        if (left_sum >= right_sum && left_sum >= cross_sum)  return left_sum;
        else if (right_sum >= left_sum && right_sum >= cross_sum)    return right_sum;
        else return cross_sum;
    }
}

int maxSubArrayCrossSum(vector<int>& nums, int low, int mid, int high)
{
    int left_max = INT_MIN, right_max = INT_MIN;
    int sum = 0;
    for (int i = mid; i >= low; --i)
    {
        cout << "i:" << i << endl;
        sum += nums[i];
        if (sum > left_max)
        {
            left_max = sum;
        }
    }

    sum = 0;
    for (int i = mid + 1; i <= high; ++i)
    {
        sum += nums[i];
        if (sum > right_max)
        {
            right_max = sum;
        }
    }
    return left_max + right_max;
}



// 最大连续子数组：暴力求解
int maxSubArray_violence(vector<int>& nums)
{
    if (nums.size() <= 0)    return 0;
    int result = INT_MIN, sum = 0;
    for (int i = 0; i < nums.size(); ++i)
    {
        sum = 0;
        for (int j = i; j < nums.size(); ++j)
        {
            sum += nums[j];
            if (sum > result)    result = sum;
        }
    }
    return result;
}

// 动他规划解法
int maxSubArray_Dynamic(vector<int>& nums)
{
    int pre = 0, maxRes = nums[0];
    for (int i = 0; i < nums.size(); ++i)
    {
        pre = max(pre + x, x);
        maxRes = max(pre, maxRes);
    }
    return maxRes;
}

//TODO: 线段树知识及解法