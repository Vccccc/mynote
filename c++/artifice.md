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

## StringArg
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