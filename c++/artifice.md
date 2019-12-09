# C++ 技巧
## 编码规范
##### 常量
首字符使用 k ，其余字符使用驼峰命名法。
const int kMaxSize = 1024;

##### 变量
**普通变量**
使用驼峰命名法，首字符使用小写。
int lastSecond;
**线程变量**
首字符使用小写字母 t 标识为线程，然后使用 \_ 分隔首字母和变量命，剩余变量名使用使用驼峰命名法。
\__thread int t_lastSecond;

##### 函数
**全局函数**
首字符使用小写字母 g 标识为全局，然后使用 \_ 分隔首字母和变量命，剩余变量名使用使用驼峰命名法。
void g\_flush();

##### typedef
**定义类型**
使用驼峰命名法，首字符使用大写。
typedef void (*FlushFunction)();

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


#### 

## 宏
一些有趣的宏使用技巧
### BINARY_PREDICATE
重载二元谓词(predicate)时，将重复部分抽象出来。
```
class String 
{
public:
    String(const char* str)
        : str_(str), length_(strlen(str))
        {

        }
   
public:
    bool operator==(const String& x) const
    {
        return ((length_ == x.length_) && (memcmp(str_, x.str_, length_) == 0));
    }

    bool operator!=(const String& x) const
    {
        return !(*this == x);
    }

    bool operator<(const String& x) const 
    {
        int length = length_ < x.length_ ? length_ : x.length_;
        int ret = memcmp(str_, x.str_, length);
        if(ret < 0) // 这步 (ret auxcmp 0)只要满足条件则结果为 true, 如仍不满足条件且 (ret != 0)，则结果为 false。
        {
            return true;
        }
        else if(ret == 0)
        {
            return length_ < x.length_;   
        }
        else
        {
            return false;
        }
    }
#define STRING_BINARY_PREDICATE(cmp, auxcmp)                                        \
    bool operator cmp(const String& x) const                                        \
    {                                                                               \
        int r = memcpm(str_, x.str_, length_ < x.length_ ? length_ : x.length_);    \
        return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));             \
    } 

    STRING_BINARY_PREDICATE(<, <);
    STRING_BINARY_PREDICATE(<=, <);
    STRING_BINARY_PREDICATE(>, >);
    STRING_BINARY_PREDICATE(>=, >);
#undef STRING_BINARY_PREDICATE
private:
    const char* str_;
    size_t length_;
};
```

## 枚举
枚举命名使用驼峰命名法，首字符使用大写，成员全使用大写字母，单词之间使用\_分隔。
最后可使用一个枚举量 NUM\_LOG\_LEVELS 记录枚举数。
```c
enum LogLevel
{
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
  NUM_LOG_LEVELS,
};

const char* LogLevelName[LogLevel::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};
```