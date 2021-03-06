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
### Binary Tree Inorder traversal
!["题目"](./photo/94.png)
##### 使用递归
递归地中序遍历，将值存储到数组中。
##### solution 1
```c
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        inorderTraversalAux(root, res);
        return res;
    }

    void inorderTraversalAux(TreeNode* root, vector<int>& res)
    {
        if(root != NULL)
        {
            if(root->left != NULL)
            {
                inorderTraversalAux(root->left, res);
            }
            res.push_back(root->val);
            if(root->right != NULL)
            {
                 inorderTraversalAux(root->right, res);
            }    
        }
    }
};
```

##### 使用迭代
通过 stack 模拟递归遍历。
##### solution 2
```c
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> treeStack;

        TreeNode* cur = root;
        while(cur != NULL || !treeStack.empty())
        {
            while(cur != NULL)
            {
                treeStack.push(cur);
                cur = cur->left;
            }
            cur = treeStack.top();
            treeStack.pop();
            res.push_back(cur->val);
            cur = cur->right;
        }
        return res;
    } 
};
```

### Unique Binary Search Trees II
![](./photo/95.png)
##### 思路
构建树需要 root，left，right 节点。观察题目，可以知道数值 1-n 都可以成为 root 节点。每次取[start,end]其中一个值为 root 节点时，它的左子节点的值必须比 root 节点小，它的右子节点的值必须比 root 节点大，所以子树[start, end]的范围可以不断缩小。
##### solution
```c
class Solution {
public:
    vector<TreeNode*> generateTrees(int n) {
        if(n <= 0)
        {
            return vector<TreeNode*>{};
        }

        return createTree(1, n);
    }

    vector<TreeNode*> createTree(int start, int end)
    {
        if(end < start)
        {
            return vector<TreeNode*>{nullptr};
        }

        vector<TreeNode*> ans;
        for(int i = start; i <= end; i++)
        {
            vector<TreeNode*> left_subtrees, right_subtrees;
            left_subtrees = createTree(start, i-1);
            right_subtrees = createTree(i+1, end);

            for(auto& leftSubtree : left_subtrees)
            {
                for(auto& rightSubtree : right_subtrees)
                {
                    TreeNode* root = new TreeNode;
                    root->val = i;
                    root->left = leftSubtree;
                    root->right = rightSubtree;
                    ans.push_back(root);
                }
            }
        }

        return ans;
    }
```

### Validate Binary Search Tree
![](./photo/98.png)

##### 思路 递归
画图可以看出，每个节点都有一个范围值 [lower, upper] 限制，左子节点 lower 取父节点的 lower，左子节点 upper 取父节点的值。右子节点 lower 取父节点的值，右子节点 upper 取父节点的 upper。

##### sulotion 1
```c
class Solution
{
public:
    bool isValidBST(TreeNode *root)
    {
        if (root == NULL)
        {
            return true;
        }

        return help(root, LONG_LONG_MIN, LONG_LONG_MAX);
    }
    bool help(TreeNode* node, long long lower, long long upper)
    {
        if(node == NULL)
        {
            return true;
        }

        int val = node->val;
        if(val <= lower)
        {
            return false;
        }
        if(val >= upper)
        {
            return false;
        }

        if(!help(node->left, lower, val))
        {
            return false;
        }
        if(!help(node->right, val, upper))
        {
            return false;                                                 
        }

        return true;
    }
};
```
##### 使用迭代
通过迭代模拟递归，可以观察到每次递归都会更新 [lower, upper]，所以模拟递归的时候每个节点携带的信息也要有 [lower, upper]，分别通过两个 stack 去缓存就行了。
##### solotion
```c
class Solution
{
public:
    bool isValidBST(TreeNode *root)
    {
        if(root == NULL)
        {
            return true;
        }
        stack<TreeNode*> stacks;
        stack<long long> lowers, uppers;
        auto update = [&stacks, &lowers, &uppers](TreeNode* node, long long lower, long long upper)
        {
            stacks.push(node);
            lowers.push(lower);
            uppers.push(upper);
        };
        
        update(root, LONG_LONG_MIN, LONG_LONG_MAX);
        while(!stacks.empty())
        {
            TreeNode* root = stacks.top();
            long long lower = lowers.top();
            long long upper = uppers.top();
            stacks.pop();
            lowers.pop();
            uppers.pop();

            if(root == NULL)
            {
                continue;
            }
            int val = root->val;
            if(val <= lower || val >= upper)
            {
                return false;
            }

            if (root->left != NULL)
            {
                update(root->left, lower, val);
            }
            if (root->right != NULL)
            {
                update(root->right, val, upper);
            }
        }
        return true;
    }
};
```
### Same Tree
![](./photo/100.png)

##### 思路
通过递归遍历节点，判断节点是否相同。每一次递归的作用都是，判断当前节点是否相同，如果不相同则说明树不相同，如果相同，则要继续判断当前节点的左右节点是否相同。

##### solution
```c
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if(p == NULL)
        {
            if(q == NULL)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(q == NULL)
            {
                return false;
            }
            else if(p->val != q->val)
            {
                return false;
            }
        }

        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};
```

### Symmetric Tree
![](./photo/101.png)

##### 使用递归
画图观察可以发现规律

### Single Number
Description:
Given an array of integers, every element appears twice except for one. Find that single one.
Note: Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?

##### 思路
元素出现偶数次，则元素的 bit 也应为偶数次，因为 x^x = 0, x^0 = x，(0 ^ 1) ^ 1  == 0 ^ (1  ^ 1)  == 0（组合顺序无关），所以当相同元素相异或时（x^x = 0），值为0。0 与 单独的元素异或时(x^0 = x)，则得到该值。

##### Solution
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
### Arithmetic Slices
!["题目"](./photo/413.png)

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
根据 Solution 1 可观察到 left 和 right 之间的 difference 都相同，所以用一个辅助函数计算 left 和 right 之间能组成的 slice，从而避免重复计算。
##### Solution 2
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
              right++;
            }
            result += cacul(left, right);
            left = right;
        }
        return result;
    }

    int cacul(int left, int right)
    {
        if(right - left + 1 < 3)
        {
            return 0;
        }

        int max = right - left + 1 - 2;
        int total = 0;
        while(max >= 1)
        {
            total += max;
            max--;
        }
        return total;
    }
};
```
##### 思路 动态规划
Solution 1 是计算以 left 为起点的 slice，但如果计算以某一个位置作为结尾的 slice 则可通过动态规划去解决问题。dp[tail] 记录以 A[tail] 为结尾的 slice 数量，当计算 dp[tail+1] 时，只需简单计算 A[tail+1] 与 A[tail], A[tail-1] 是否能组成一个 slice 即可，如果能，则多增加一个 A[tail-1] ，A[tail], A[tail+1] 的 slice。
##### Solution 3
```c
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        if(A.size() < 3)
        {
            return 0;
        }

        vector<int> dp(A.size(), 0);
        if(A[1] - A[0] == A[2] - A[1]) 
        {
            dp[2] = 1;
        }
        
        int result = dp[2];
        for(int tail = 3; tail < A.size(); tail++)
        {
            if(A[tail-1] - A[tail-2] == A[tail] - A[tail-1])
            {
                dp[tail] = dp[tail-1] + 1;
            }
            result += dp[tail];      
	}
        return result;
    }
};
```

### Longest Substring with At Least K Repeating Characters
!["题目"](./photo/395.png)
##### 思路
找出满足条件的区间，然后取最大那个。为了找出满足条件的区间，可以通过找出分隔区间的字符。统计出哪些字符不满足条件，提供一个方法去判断当前下标的字符是否满足条件。
##### Solution 1  
```c
class Solution {
public:
    int to_idx(const char& c) { return c - 'a';}
    int longestSubstring(string s, int k) {
        return aux(s, 0, (int)s.size(), k);
    }

    int aux(const string& str, int start, int end, int k)
    {
        vector<int> dp(26, 0);
        for(int i = start; i < end; i++)
        {
            dp[to_idx(str[i])]++;
        }

        int idx = start;
	// 注意！会导致溢出
        // while(dp[to_idx(str[idx])] >= k && idx < end) idx++;
        while( idx < end && dp[to_idx(str[idx])] >= k) 
        {
            idx++;
        }
        if(idx == end)
        {
            return idx - start;
        }

        int right_start = idx + 1;
	// 注意！会导致溢出
        // while(dp[to_idx(str[right_start])] < k && right_start < end) right_start++;
	while(right_start < end && dp[to_idx(str[right_start])] < k) 
        {
            right_start++;
        }
        return max(aux(str, start, idx, k), aux(str, right_start, end, k));
    }
};
```
##  depth-first search
### Arithmetic Slices II - Subsequence
!["题目"](./photo/446.png)

##### 思路
枚举所有序列。通过两条线路来枚举所有序列，序列中存在或不存在 A[dep]。
时间复杂度为 O(2^n)，空间复杂度为 O(n)。
##### Solution 1 Brute Force 
```c
class Solution {
public:
    int n;
    int ans;
    
    void dfs(int dep, vector<int>& A, vector<int> cur)
    {
        if(dep == n)
        {
            if(cur.size() < 3)
            {
                return;
            }

            for(int i = 1; i < cur.size(); i++)
            {
                if(cur[i] - cur[i-1] != cur[1] - cur[0])
                {
                    return;
                }
            }

            ans++;
            return;
        }

        dfs(dep+1, A, cur); // 不包含 A[dep] 的线路
        cur.push_back(A[dep]);
        dfs(dep+1, A, cur);  // 包含 A[dep] 的线路
        return;
    }

    int numberOfArithmeticSlices(vector<int>& A) {
        n = A.size();
        ans = 0;
        
        vector<int> cur;
        dfs(0, A, cur);
        return ans;
    }
};
```

##### 思路
用 f[i][d] 记录以 A[i] 为结尾，距离为 d 的 arithmetic subsequences。但如果 arithmetic subsequences 长度至少为 3 的话，那么前两个数不能形成 arithmetic subsequences。所以 f[i][d] 都为 0，无法利用 f[i][d] 的信息。但假设定义 weak arithmetic subsequences 为长度至少 2
且任意两个相邻的数的距离相同的序列。此时用 f[i][d] 记录以 A[i] 为结尾，距离为 d 的 weak arithmetic subsequences。此时就可以利用 f[i][d] 的信息进行推导。
> f[i][A[i]-A[j]] = f[j][A[i]-A[j]] + 1 // 这里 +1 是因为 pair(i, j)组成了一个weak arithmetic subsequence

如果 f[i][d] 不为 0 则说明 f[i][d] 至少存在一个 weak arithmetic subsequences。在一个 weak arithmetic subsequence 追加一个数必定能组成一个 arithmetic subsequence。所以 f[i+1][d] 的 arithmetic subsequences 等于 f[i][d] 的 weak arithmetic subsequences。
##### Solution 2 动态规划
```c
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        int n = A.size();
        long long ans = 0;

        vector<map<long long, int>> cnt(n);
        for (int i = 1; i < n; i++) {
          for (int j = 0; j < i; j++) {
            long long delta = (long long)A[i] - (long long)A[j];
            int sum = 0;
            if (cnt[j].find(delta) != cnt[j].end()) {
              sum = cnt[j][delta];
            }
            cnt[i][delta] += sum + 1;
            ans += sum;
          }
        }
        return ans;
    }
};
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


## 回文字符串
### 判断是否为回文字符串
##### 思路
```c
bool isPalindrome(string str)
{
    if(str.size() < 2)
    {
        return false;
    }
    int left = 0;
    int right = str.size() - 1;

    while(left < right)
    {
        if(str[left] != str[right])
        {
            return false;
        }
        left--;
        right--;
    }

    return true;
}
```
