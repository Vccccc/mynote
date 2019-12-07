# C++ 技巧
## 编码规范
##### 常量
首字符使用 k ，其余字符使用驼峰命名法。
const int kMaxSize = 1024;

#### 类
##### 类名
使用驼峰命名法，首字符使用大写。
class Base
##### 成员函数
使用驼峰命名法，首字符使用小写。
size\_t writableBytes();

##### 成员变量
使用驼峰命名法，首字符使用小写，最后的字符为\_。
int data\_;

##### 类常量
定义方式同 常量。但声明为 static。因为常量只有一个实体就够了。

## 技巧
### StringArg
```c
class StringArg // copyable
{
 public:
  StringArg(const char* str)
    : str_(str)
  { }

  StringArg(const string& str)
    : str_(str.c_str())
  { }

  const char* c_str() const { return str_; }

 private:
  const char* str_;
};
```
用于一些参数为 C-style string 的函数。避免需要重载 string 和 char *。
void func(StringArg arg);

### 参数
参数顺序为，输入、可能改变、输出。
#### 传引用 | 传值
传引用的方式
```c
void fun(string* target);
void fun(string& target);
void fun(const string* target);
void fun(const string& target);
```
当 target 会在函数中改变时，使用方式 1。当传常量引用时， 使用方式 4。