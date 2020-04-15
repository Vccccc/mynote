## Tree
树的结点Node
```cpp
template<typename T>
struct Node
{
    T key_;
    Node* parent_;
    Node* left_;
    Node* right_;
}
```

二叉搜索树性质：
>设x是二叉搜索树中的一个结点。如果y是x左子树中的一个结点，那么y.key_ <= x.key_。如果y是x右子树中的一个结点，那么y.key_ >= x.key_。

**中序遍历**
>输出的子树根的关键字值位于其左子树的关键字值和右子树的关键字值之间
```cpp
// 递归算法
void inorderTreeWalk(Node* x)
{
    if(x != NULL)
    {
	inorderTreeWalk(x->left_);
	std::cout << x->key_;
	inorderTreeWalk(x->right_);
    }
}

// 非递归算法，使用stack
void inorderTreeWalk(Node* x)
{
    std::stack<Node*> nodes;
    while(x != NULL || !nodes.empty())
    {
        if(x != NULL)
	{
	    nodes.push(x);
	    x = x->left_;
	}
	else
	{
	    x = nodes.top();
	    std::cout << x->key_;
	    x = x->right_;
	    nodes.pop()
	}
    }
}   
```
**前序遍历**
>输出的子树根的关键字值位于其左子树的关键字值和右子树的关键字值之前
```cpp
void inorderTreeWalk(Node* x)
{
    if(x != NULL)
    {
	std::cout << x->key_;
	inorderTreeWalk(x->left_);
	inorderTreeWalk(x->right_);
    }
}
```
**后序遍历**
>输出的子树根的关键字值位于其左子树的关键字值和右子树的关键字值之后
```cpp
void inorderTreeWalk(Node* x)
{
    if(x != NULL)
    {
	inorderTreeWalk(x->left_);
	inorderTreeWalk(x->right_);
	std::cout << x->key_;
    }
}
```
***查找***
```cpp
Node* search(Node* x, T key)
{
    while(x != NULL && x->key_ != key)
    {
        if(x->key_ < key)
            x = x->left-;
        else
            x = x->right_;
    }
    return x;
}
```

## Graph
图论的应用示例：
1. 地图。正在计划旅行的人也许想知道“从普罗维登斯到普林斯顿的最短路线” 。 对最短路径上经历过交通堵塞的旅行者可能会问：“从普罗维登斯到普林斯顿的哪条路线最快？ ”要回答这些问题，我们都要处理有关结点（十字路口） 之间多条连接（公路） 的信息。
2. 网页信息。当我们在浏览网页时， 页面上都会包含其他网页的引用（链接） 。 通过单击链接，
我们可以从一个页面跳到另一个页面。 整个互联网就是一张图， 结点是网页， 连接就是超链接。 图算法是帮助我们在网络上定位信息的搜索引擎的关键组件。
