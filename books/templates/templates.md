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
1. 转换参数使得类型匹配
	> max(static_cast<double>(4), 7.2); // OK
2. 显示指定 T 的类型，防止编译器类型推导
	> max<double>(4, 7.2); // OK
3. 用不同的模板参数

#### Type Deduction for Default Arguments
对于默认调用参数不支持类型推导。比如：
```
template<typename T>
void f(T = "");
f(1); // OK
f(); // ERROR：不能推导 T
```
为了支持这个例子，可以声明默认模板参数：
```
template<typename T = string>
void f(T = "");
f(1); // OK
f(); // OK
```

## 1.3 Multiple Template Parameters
函数模板有两部分不同的参数：
1. 模板参数（Templates parameters）在尖括号中声明
	> template\<typename T> // T 是模板参数
2. 调用参数（Call parameters）在圆括号中声明：
	> T max(T a, T b) // a 和 b 是调用参数

可以拥有多个模板参数，比如使用两个可能不同类型的调用参数定义 max 模板：
```
template<typename T1, typename T2>
T1 max(T1 a, T2 b)
{
  return b < a ? a : b;
}
auto m = ::max(4, 7.2); // OK，但是返回值类型为第一个参数定义
```
使用两个不同类型参数调用 max 会导致一些问题。如果使用其中之一的类型作为返回值类型，可能会导致在调用者意图之外，另外一个参数发生类型转换。返回值类型依赖于调用参数的顺序。求 66.66 和 42 的最大值将返回 66.66，但 42 和 66.66 会返回 66。

C++ 提供了一些方法处理这个问题：
- 采用第三个模板参数作为返回值
- 让编译器决定返回值
- 声明返回值为两个参数的“common type”
### 1.3.1 Template Parameters for Return Types
通过模板参数推导我们使用模板函数如使用普通函数一样，无需显式指定模板参数的类型。但也可以显式地指明模板参数的类型：
```
tempate<typename T>
T max(T a, T b);
::max<double>(4, 7.2); 
```

在模板参数和调用参数没有关联时和模板参数不能判断时，调用模板函数时必须显式地指明模板参数的类型。比如，可以通过第三个模板参数作为函数的返回值：
```
tempate<typename T1, typename T2, typename RT>
RT max(T1 a, T2 b);
```
然而，因为返回类型不作为类型推导的一部分（推导可视作重载解析的一部分：不基于返回值类型的处理（即函数签名与返回值无关），唯一的例外 conversion operator members 的返回值类型），和 RT 不出现在调用参数中，所以不能推导 RT 的类型。

结论是，必须显式地指明模板参数列表，比如：
```
::max<int,double,double>(4,7.2); // OK，但太繁琐
```
到现在为之，所以的例子中函数模板参数要么全部，要么都没有显式地指定。另外的方法是只显式指定第一个参数，剩余的参数通过类型推导处理。通常，必须显式地指定所有不能隐式判断的参数。因此，可以改变模板参数的顺序，使调用者只需指定返回值类型。
```
tempate<typename RT, typename T1, typename T2>
RT max(T1 a, T2 b);
::max<double>(4,7.2); // OK，返回值类型为 double，T1 和 T2 通过推导
```

注意，以上这些不同的版本的 max() 并没有带来重大意义的好处。如果两个不同类型的参数传入，对于 one-parameter 版本你可以指定参数类型和返回值类型。这是一个好的方法，令其简单。


### 1.3.2 Deducing the Return Type
如果返回值类型依赖于模板参数，最简单和最好的办法是通过编译器推导出返回类型。自从 C++14 起，可以不用声明任何返回值类型（但还是声明返回值类型为 auto）。：
```
template<typename T1, typename T2>
auto max(T1 a, T2 b)
{
 return b < a ? a : b;
}
```

如果 a 和 b 是两种不同的算术类型，那么 common arithmetic type 被返回。

注意：
```
template<typename T1, typename T2>
auto max(T1 a, T2 b) -> decltype(b<a?a:b);
```
是一个声明，所以编译器可以在编译期通过操作符 ?: 的规则找出 max() 的返回值类型。不需要匹配实现。实际上，在声明中使用 true 作为 ?: 的条件判断已经足够了。
```
template<typename T1, typename T2>
auto max(T1 a, T2 b) -> decltype(true?a:b);
```
