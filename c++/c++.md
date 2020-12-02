# 转型操作符
1. static_cast
2. const_cast
3. dynamic_cast
4. reinterpret_cast

## 使用转型操作符的好处
使用转型操作符可以使转型操作意图更准确，便于诸如grep之类的工具区分。

### 1. static_cast
static_cast可以将int转型成double，但不能将struct转型成int，或者将double转型成指针。static_cast不能够移除表达式的常量性(constness)

### 2. const_cast
const_cast用于改变表达式中的常量性(constness)或易变性(volatileness)

### 3. dynamic_cast
dynamic_cast用来执行继承体系中“安全的向下转型或跨系转型动作”。可以用dynamic_cast，将“指向base class objects的pointers或references”转型为指向“derived(或sibling base)class objects
的pointers或references”，并得知转型是否成功。如果转型失败，会以一个null指针(当转型对象是指针)或一个exception(当转型对象是reference)表现出来。

### 4. reinterpret_cast
reinterpret_cast转型结果与编译平台相关，所以reinterpret_cast不具移植性。reinterpret_cast的最常用用途是转换“函数指针”类型。
```c
typedef void(*FuncPtr)();
FuncPtr funcPtrArray[10];
int doSomething(); // 将该函数指针放入funcPtrArray中，注意返回类型为int
// 如果没有转型，是不能实现的，因为doSomething的类型与funcPtrArray所能接受的不同。
// 通过使用reinterpret_cast可以强制编译器了解你的意图
funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);
```

# algorithm
### sort
template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

### upper_bound  lower_bound 
Description:
Returns an iterator pointing to the first element in the range [first, last) that is greater than value, or last if no such element is found.

Implementation:
```c
template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
  ForwardIt it;
  typename std::iterator_traits<ForwardIt>::difference_type count, step;
  count = std::distance(first, last);
  while(count > 0)
  {
    it = first;
    step = count / 2;
    std::advance(it, step);
    if(!(value < *it))
    {
      first = ++it;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}
```
Prototype:
template< class ForwardIt, class T >
constexpr ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value );

Description:
Returns an iterator pointing to the first element in the range [first, last) that is not less than (i.e. greater or equal to) value, or last if no such element is found.

Implementation:
```c
template<typename ForwardIt first, ForwardIt last, const T& value)
{
  typename iterator_traits<ForwardIt>::diffrence_type count, step;
  Forward it;
  count = std::distance(first, last);
  while(count > 0)
  {
    it = first;
    step = count / 2;
    std::advance(it, step);
    if(*it < value)
    {
      first = ++it;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}
```




# iterator
### advance
Prototype: 
template< class InputIt, class Distance >
constexpr void advance( InputIt& it, Distance n );

Description:
Increments given iterator it by n elements.
If n is negative, the iterator is decremented.

# 位运算
### 与或非 & | ~
**与运算(&)** | **结果**
:--------|:----------
0 & 1 | 0
1 & 1 | 1
0 & x | 0
1 & x | x

用途：
- 清零，将一个数与0相与。如0 & x = 0。
- 取一个数中某些位。将不需要的位与0相与，将要保留的位与1相与。如1001取前2位，1001 & 1100 = 1000

**或运算(\|)** | **结果**
:-----|:---------
0 \| 1 | 1
0 \| 0 | 0
0 \| x | x
1 \| x | 1

用途：
- 将一个数某些位置1。如将1001中间两位置1，1001 | 0110 = 1111。

**非运算(~)** | **结果**
:--------|:-------
~0 | 1
~1 | 0

用途：
- 将一个数最低位置0。x = 1001, x & ~1 = 1000。其中~1 = 1111111111111110

### 异或 ^
**异或运算(^)** | **结果**
:-------|:-----
0 ^ 0 | 0
1 ^ 1 | 0
0 ^ 1 | 1
0 ^ x | x
1 ^ x | ~x

用途：
- 翻转一个数中的某些位。如x = 1001，翻转后两位。x ^ 0011 = 1010
- 与0相异或，保留原值。如x = 1001，x ^ 0 = x。

# 闭包
## First-class citizen 头等公民
在程序语言设计里，头等公民(通常为 type, object, entity, or value)作为一种 entity，支持任何其他 entities 所支持的操作。这些操作通常包括可作为参数传递、可被函数返回、可修改、可被变量赋值。

## First-class function 头等函数
头等函数是指视 function 为头等公民。这意味着 function 可作为参数传递、可被函数返回、可修改、可被变量赋值或存储在数据结构中。


# offsetof(type, member)
Defined in header \<cstddef>
offsetof 是由编译器定义，是一个返回 size_t 的宏。它返回 type 的 member 的偏移，包括任何填充的内存。如果 member 是 static member 或者是 member funtion，则行为未定义。

# (void)变量
编译器会对未使用的变量提示警告，使用 (void) 变量，可以屏蔽掉这个警告。

# pointer to member
```c
#include <iostream>
using namespace std;

class Car
{
    public:
    int speed;
};

int main()
{
    int Car::*pSpeed = &Car::speed;

    Car c1;
    c1.speed = 1;       // direct access
    cout << "speed is " << c1.speed << endl;
    c1.*pSpeed = 2;     // access via pointer to member
    cout << "speed is " << c1.speed << endl;
    return 0;
}
```

```c
void Apply( SomeClass * c, void (SomeClass::*func)() ) {
    // do hefty pre-call processing
    (c->*func)();  // call user specified function
    // do hefty post-call processing
}
```

# clang++ 编译指令
["官方文档"](https://clang.llvm.org/docs/ClangCommandLineReference.html)

## Actions
The action to perform on the input.
- **-E, --preprocess**: Only run the preprocessor
- **-S, --assemble**: Only run preprocess and compilation steps
- **-c, --compile**: Only run preprocess, compile, and assemble steps
## Preprocessor flags
Flags controlling the behavior of the Clang preprocessor.
- **-isystem<directory>**: add directory to SYSTEM include search path

### Include path management
Flags controlling how #includes are resolved to files.
- **-I\<dir>, --include-directory \<arg>, --include-directory=\<arg>**: Add directory to include search path

# 值类型
每个 C++ 表达式（带有操作数的操作符、字面量、变量名等）可按照两种独立的特性加以辨别：类型和值类别 (value category)。每个表达式都具有某种非引用类型，且每个表达式只属于三种基本值类别中的一种：纯右值 (prvalue)、亡值 (xvalue)、左值 (lvalue)。
- glvalue (“generalized” lvalue) 是一个表达式，其求值确定一个对象、位域或函数。
- prvalue (“pure” rvalue)是一个表达式，其求值为以下之一
	- 计算某个运算符的操作数的值或为 void 表达式（这种纯右值没有结果对象）
	- 初始化某个对象或位域（称这种纯右值有一个结果对象）。除了 decltype 外，所有类和数组的纯右值都有结果对象，即使它被舍弃也是如此。结果对象可以是变量，通过 new-expression 创建的对象，通过 Temporary materialization 实现的临时对象或其成员。
- xvalue (an “eXpiring” value) 是一个表示对象或者位域资源可以重新被使用的 glvalue
- 左值 (lvalue)（如此称呼的历史原因是，左值可以出现于赋值表达式的左边）是非 xvalue 的 glvalue
- 右值 (rvalue)（如此称呼的历史原因是，右值可以出现于赋值表达式的右边）是 prvalue 或 xvalue 

# SFINAE
“Substitution Failure Is Not An Error”

# 模板类型推导

