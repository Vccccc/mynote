### 题目
**题目描述**
>

**输入描述**
>

**输出描述**
>

-----
##### 思路

##### Solution
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

# 其他
### 二维数组中的查找
**题目描述**
>在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

**输入描述**

>input:  待查找的二维数组
>target: 查找的数字

**输出描述**
>查找到返回true，查不到返回false。

![题目](./photo/find.jpg)

____
##### Solution
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
### 从尾到头打印链表 
**题目描述**
>输入一个链表的头节点，从尾到头反过来打印出每个节点的值

**输入描述**
>pHead: 头节点


结构体定义如下：
```cpp
struct ListNode
{
  int m_nKey;
  ListNode* m_pNext;
};
```
---------
##### 思路
链表是从头到尾的顺序访问的，与从尾到头的顺序相反。这时可以想到使用栈，来暂存节点数据。当遍历完整个链表后，再从栈顶依次打输出节点的值。
##### 使用栈
```cpp
void PrintList(ListNode* pHead)
{
    std::stack<ListNode*> nodes;
    
    ListNode* pNode = pHead;
    while(pNode != NULL)
    {
	nodes.push(pNode);
	pNode = pNode->m_pNext;
    }

    while(!nodes.empty())
    {
	pNode = nodes.top();
	printf("%d\t", pNode->m_nKey);
     	nodes.pop();
}
```

##### 使用递归
递归本质上就是一个栈结构，所以可以使用递归代替栈。

```cpp
void PrintList(ListNode* pHead)
{
    if(pHead != NULL)
    {
        if(pHead->m_pNext != NULL)
	{
	    PrintList(pHead->m_pNext);
	}
	printf("%d\t", pHead->m_nkey);
    }
}
```
### 快速排序
**题目描述**
>实现快排

**输入描述**
>data: 序列
>length: 序列长度
>start: 序列起始位置
>end: 序列结束位置



-----

##### Solution
```cpp
int Partition(int data[], int length, int start, int end)
{
    if(data == NULL || length <= 0 || start < 0 || end >= length)
        throw new std::exception("Invalid Parameters");

    std::random_device rd;
    std::uniform_int_distribution<> dis(start, end);
    int index = dis(rd);

    swap(&data[index], &data[end]);

    int small = start - 1;
    for(index = start; index < end; ++index)
    {
        if(data[index] < data[end])
        {
            ++small;
            if(small != index)
                swap(&data[index], &data[small]);
        }
    }

    ++small;
    swap(&data[small], &data[end]);

    return small;
}

void QuickSort(int data[], int length, int start, int end)
{
    if(start == end)
        return;

    int index = Partition(data, length, start, end);
    if(index > start)
        QuickSort(data, length, start, index - 1);
    if(index < end)
        QuickSort(data, length, index + 1, end);
    
}
```
### 替换空格
**题目描述**
>请实现一个函数,把字符串中的每个空格替换成“%20”。例如输入“We are happy.”，则输出“We%20are%20happy.”

**输入描述**
>str: 被替换的字符串

```cpp
void replaceSpace(string& str)
{
    int n = 0;
    for(auto& i : str)
    {
        if(i == ' ')
        {
            ++n;
        }
    }
    
    n *= 2;
    n += str.size();
    string res;
    res.resize(n);
    for(int i = str.size() - 1, j = n - 1; i >= 0; --i)
    {
        if(str[i] != ' ') 
        {
            res[j--] = str[i];
        }
        else
        {
            res[j--] = '0';
            res[j--] = '2';
            res[j--] = '%';
        }
    }
    str = res;
}

```
### 用两个栈实现队列
**题目描述**
>用两个栈实现一个队列。队列声明如下，请实现它的两个函数appendTail和deleteHead，分别完成在队列尾部插入结点和在队列头部删除结点的功能

```cpp
template<typename T>
class Queue
{
public:
    Queue();
    ~Queue();

    void appendTail(const T& node);
    T deleteHead();

private:
    stack<T> stack1;
    stack<T> stack2;
};
```

##### 思路
stack是先进后出，而queue是先进先出。出入顺序刚好相反，所以再借助一个stack再反一次，就是queue的顺序了。注意queue为空时的判空处理。
##### Solution
```cpp
template<typename T>
void Queue<T>::appendTail(const T& node)
{
    stack1.push(node);
}

template<typename T>
T Queue<T>::deleteHead()
{
    if(stack2.size() <= 0)
    {
        while(stack1.size() > 0)
        {
            T& data = stack1.top();
            stack1.pop();
            stack2.push(data);
        }
    }

    if(stack2.size() == 0)
        throw new exception("queue is empty");

    T head = stack2.top();
    stack2.pop();

    return head;
}
```
### 重建二叉树
**题目描述**
>输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。

**结点**
```cpp
struct Node
{
    int value_;
    Node* left_;
    Node* right_;
}
```

**输入描述**
>preorder: 前序遍历序列
>inorder: 中序遍历序列
>length: 序列长度

##### 思路
由前序遍历序列找出root结点，根据root结点的值找出root结点在中序遍历序列的位置，用root结点划分root结点的左右子树。
构建树的过程是找出根结点、构建左子树、构建右子树。可以发现，这是由一个大的问题，不断划分成同样的小问题。可以使用递归解决，递归基是当子树没有左右结点时停止。

##### Solution
```cpp
Node* Construct(int* preorder, int* inorder, int length)
{
    if(preorder == NULL || inorder == NULL || length <= 0)
        return NULL;

    return ConstructCore(preorder, preorder + length - 1,
            inorder, inorder + length - 1);
}

Node* ConstructCore(int* startPreorder, int* endPreorder,
        int* startInoder, int* endInorder)
{
    // 前序遍历序列的第一个数字是根结点的值
    int rootValue = startPreorder[0];
    Node* root = new Node();
    root->value_ = rootValue;
    root->left_ = root->right_ = NULL;

    if(startPreorder == endPreorder)
    {
        if(startInoder == endInorder && *startPreorder == *startInoder)
            return root;
        else
            throw std::exception("Invalid input.");
    }

    // 在中序遍历中找到根结点的值
    int* rootInorder = startInoder;
    while(rootInorder <= endInorder && *rootInorder != rootValue)
        ++rootInorder;

    if(rootInorder == endInorder && *rootInorder != rootValue)
        throw std::exception("Invalid input.");

    int leftLength = rootInorder - startInoder;
    int* leftPreorderEnd = startPreorder + leftLength;
    if(leftLength > 0)
    {
        // 构建左子树
        root->left_ = ConstructCore(startPreorder + 1,
                leftPreorderEnd, startInoder, rootInorder - 1);
    }

    if(leftLength < endPreorder - startPreorder)
    {
        // 构建右子树
        root->right_ = ConstructCore(leftPreorderEnd + 1, endPreorder,
                rootInorder + 1, endInorder);
    }
    
    return root;
}
```

### 题目
!["题目"](./photo/413.png)

-----
##### 思路
从左往右开始计算，每次计算以 left 为起点的 slice 数，直到遇到第一个 difference 不同的数，则 left + 1，继续计算。
##### Solution 1
```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        if(A.size() < 3)
        {
            return 0;
        }

        int result = 0;
        for(int left = 0; left <= A.size() - 3;)
        {
            int right = left + 1;
            int difference = A[right] - A[left];
            while(right < A.size() - 1 && A[right + 1] - A[right] == difference)
            {
              result++;
              right++;
            }
            left++;
        }
        return result;
    }
};
```

##### 思路

##### Solution 2
```cpp
```