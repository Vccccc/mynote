# Chapter 1 Function Templates
### 1.1.3 Two-Phase Translation
templates are "compiled" in two phrase:
1. 定义时不实例化，检查不包含模板参数的代码正确性：
	- 语法错误，比如缺少分号（；）
	- 使用与模板参数无关的未知的 names（type names，function names，...）
	- 与模板参数无关的静态断言（static assertion）
2. 实例化时，再次检测所有与模板参数相关的代码。

#### Compiling and Linking
Two-Phase Translation 导致一个问题：当一个模板函数实例化时，编译器需要知道它的定义，通常普通函数编译和链接只需知道声明即可，但模板参数不同。所以，需要在头文件中实现模板。

#### Type conversions During Type Deduction
Note that automatic type conversions are limited during type deduction:
- 当声明调用参数为引用时，类型推导时连无用的转化也不允许。用相同的模板参数 T 声明的两个参数类型必须一致。
- 当声明调用参数为值时，只有支持无用的转换（decay）：忽略 const，volatile 修饰，引用类型转换成引用类型，raw array 或者函数转换成指针。用相同的模板参数 T 声明的两个参数退化后类型（decayed type）必须一致。

```
// 声明1
template<typename T>
T max(T const& a, T const& b)

// 声明2
template<typename T>
T max(T a, T b)

int i = 17;
int arr[4];

max(&i, arr); // ERROR：声明1，OK：声明2。声明1中 arr 类型为 int const&[4]，声明2中类型为 int*
```

如下调用也是错误的：
```
max(4, 7.2); // ERROR：T 可以推导为 int 或 double
string s;
max("hello", s); // ERROR：T 可以推导为 char const[6] 或 string
```
有三种办法可以处理错误：
1. 转换

