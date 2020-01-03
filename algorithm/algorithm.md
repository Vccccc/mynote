**题目描述**
>

**输入描述**
>

**输出描述**
>

-----
### 思路

### Solution
```cpp
```

# LeetCode原题
### Single Number
Description:
Given an array of integers, every element appears twice except for one. Find that single one.
Note: Your algorithm should have a linear runtime complexity. Could you implement it without using
extra memory?

分析：
元素出现偶数次，表示某个bit出现偶数次。比如数字2出现偶数次，表示在1位和0位，10出现偶数次。那么异或运算下来必然清零。因为有一个数只出现一次，那么异或运算结果必然是那个数。

代码：
```c
int singleNumber(const vector<int> & nums)
	{
		int x = 0;
		for (auto i& nums)
			x ^= i;
		return x;
	}
```

**题目描述**
>在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

**输入描述**

>input:  待查找的二维数组
>target: 查找的数字

**输出描述**
>查找到返回true，查不到返回false。

![题目](./find.jpg)

____
### Solution
```cpp
bool findNumber(const vector<vector<int>>& input, const int target)
{
    int rows = input.size();
    int cols = input[0].size();
    
    bool found = false;
  
    if(input.empty())
        return found;

    int row = 0, col = cols - 1;
    while(row < rows && col >= 0)
    {
        if(input[row][col] == target)
        {
            found = true;
            break;
        }
        else if(input[row][col] > target)
        {
            --col;
        }
        else 
        {
            ++row;
        }
    }
    return found; 
}
```