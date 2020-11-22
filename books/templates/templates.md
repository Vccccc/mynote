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
然而，这里有个巨大的陷阱：返回值类型可能会是一个引用类型，因为某些条件下 T 可能是一个引用。因此，应该使返回值类型从 T 退化（decayed）：
```
template<typename T1, typename T2>
auto max(T1 a, T2 b) -> typename decay<decltype(true?a:b)>::type
{
 return b < a ? a : b;
}
```

注意，每个 auto 类型实例化时都会退化。这同样作用于 auto 声明的返回值类型。auto 类型的返回值行为同下面的代码一样，a 是声明为 i 的退化类型 int：
```
int i = 42;
int const& ir = i; // ir refers to i
auto a = ir; // a is declared as new object of type int
```

### 1.3.3 Return Type as Common Type
自从 C++11 起，标准库提供了一种途径去指定选择“the more general type”。std::common_type<>::type 产生由两个或多个传入的不同类型的模板参数的“common type”，比如：
```
#include <type_traits>
template<typename T1, typename T2>
std::common_type_t<T1,T2> max(T1 a, T2, b)
{
  return b < a ? a : b;
}
```

std::common_type 是 type trait，定义在 <type_traits> 中，其产生一个拥有 type member 的结构体。核心用法是：
```
typename std::common_type<T1, T2>::type // since C++11
```

到了 C++14 可以简单地添加后缀，省略 typename 和 ::type 去获取类型：
```
std::common_type_t<T1, T2> // equivalent since C++14
```

## 1.4 Default Template Arguments
对于模板参数可以定义默认值，其称为默认模板参数。默认模板参数甚至可以引用之前的模板参数。

如果希望合并上述方法使得其具有通过多个参数类型来定义返回值类型的功能，可以通过一个具有 common type 的模板参数 RT 作为默认模板参数。同样，这也有多种途径：
```
#include <type_traits>
template<typename T1, typename T2, 
typename RT=decay_t<decltype(true?T1():T2())>
RT max(T1 a, T2, b)
{
  return b < a ? a : b;
}
```

再次提醒，通过使用 decay_t 去保证返回类型不为引用。这种实现方法要求对于传入的类型能够调用默认构造函数。此外还有另一种方法，通过使用 std::declval，但是这会使得声明更加复杂。

同样地可以使用 common_type<> 的 type trait 去指定返回类型的默认值
```
#include <type_traits>
template<typename T1, typename T2, 
typename RT=common_type_t<T1,T2>>
RT max(T1 a, T2, b)
{
  return b < a ? a : b;
}
```

同样记住，common_type 也会退化，所以返回值类型不会为引用。

在所有的例子中，作为调用者，你现在可以使用默认值作为返回值类型。
```
auto a = ::max(4,7.2);
```
或在其他参数类型后显式地指定返回值类型：
```
auto b = ::max<double, int, long double>(7.2,4);
```

然而这会有个问题，我们需要指定三个类型才指定返回值类型。我们希望返回值类型作为第一个参数，但仍然可以由其他参数类型推导。技术上，可以使首个函数模板参数具有默认值，甚至于其他参数没有默认值（普通函数尝试了下不可以）：
```
template<typename RT = long, typename T1, typename T2>
RT max(T1 a, T2, b)
{
  return b < a ? a : b;
}

// 编译通过，但要实现模板指针特化
template<>
long max<long, int*, int*>(int*  a, int*  b)
```
在这样的定义中，比如可以这样调用：
```
int i;
long l;
max(i, l); // returns long(default argument of template parameter for return type)
max<int>(4, 42); // returns int as explicity requested
```

然而，这个方法只有在需要“自然”的默认模板参数才有意义。我们需要的是默认模板参数依赖于其他的模板参数。技术上这是可行的，在 26.5.1 小节讨论，但这个技术依赖于 type traits 和更复杂的声明。

由于这些原因，最简单和最好的方法是让编译期推导返回值类型，如同 1.3.2 小节那样。

## Overloading Function Templates
如同普通函数，函数模板也可以被重载。这里我们讨论关于模板函数的重载规则。

下面的程序展示了模板函数的重载：
```
#include <iostream>
using namespace std;

template<typename T>
auto max(T  a, T  b)
{
    cout << "function template\n";
    return b < a ? a : b;
}

int max(int a, int b)
{
    cout << "ordinary function\n";
    return b < a ? a : b;
}

int main()
{
    ::max(7, 42); // calls the nontemplate for two ints
    ::max(7.0, 42.0); // calls max<double>(by argument deduction)
    ::max('a', 'b'); // calls max<char> (by argument deduction)
    ::max<>(7, 42); // calls max<int> (by argument deduction)
    ::max<double>(7,42); // calls max<double> (no argument deduction)
    ::max('a', 42.7); // calls the nontemplate for two ints
}

```
如例子所示，名字相同的非模板函数可以和模板函数同时存在，也可以和相同类型的实例同时存在。所有其他因素相同，重载解析处理会选择非模板函数而不是通过模板函数产生。第一个调用使用了这个规则：
```
::max(7, 42); // both int values math the nontemplate function perfectly
```
如果模板可以产生更匹配的函数，那么选择模板。如第二个和第三个调用所示：
```
::max(7.0, 42.0);// calls max<double>(by argument deduction)
::max('a', 'b');// calls max<char> (by argument deduction)
```
这里模板是更好的匹配，因为不需要从 double 或 char 到 int 的转换。

同样可以显式地指定一个空的模板参数列表。这个语法表示用模板解析一个调用，但所有模板参数应该由调用参数推导：
```
::max<>(7,42);//calls max<int> (by argument deduction)
```

因为自动类型转换不被模板参数推导所考虑而被普通函数所考虑，所以最后一个调用了非模板函数（'a' 和 42.7 都被转换成 int）：
```
::max('a', 42.7); // only the nontemplate function allows nontrivival conversions
```

一个有趣的例子是使得重载最大值模板函数能够只显式地指定返回值类型：
```
#include <iostream>
using namespace std;

template<typename T1, typename T2>
auto max(T1  a, T2  b)
{
    cout << "function template\n";
    return b < a ? a : b;
}

template<typename RT,typename T1, typename T2>
RT max(T1  a, T2  b)
{
    cout << "RT function template\n";
    return b < a ? a : b;
}

int main()
{
    auto a = ::max(4, 7.2); // use first template
    auto b = ::max<long double>(7.2,4);// use second template
    //auto c = ::max<int>(4, 7.2); // ERROR:both function templates match
}
```
当两个模板都匹配时，重载解析处理选择报错。所以重载函数模板时，必须保证对于每个调用只有一个匹配。

一个对于指针和 C-strings 最大值模板重载的例子：
```
#include <iostream>
#include <cstring>
#include <string>
using namespace std;
// maximum of two values of any type:
template <typename T>
T max(T a, T b)
{
    cout << "any type\n";
    return b < a ? a : b;
}
// maximum of two pointers:
template <typename T>
T* max(T* a, T* b)
{
    cout << "pointers\n";
    return *b < *a ? a : b;
}
// maximum of two C-strings:
char const* max(char const* a, char const* b)
{
    cout << "C-strings\n";
    return strcmp(b,a) ? a : b;
}

int main()
{
    int a = 7;
    int b = 42;
    auto m1 = ::max(a,b); // max() for two values of type int

    string s1 = "hey";
    string s2 = "you";
    auto m2 = ::max(s1, s2); // max() for two values of type string

    int* p1 = &b;
    int* p2 = &a;
    auto m3 = ::max(p1, p2); // maximum of two pointers

    char const* x = "hello";
    char const* y = "world";
    auto m4 = ::max(x,y); // maximum of two C-strings
}

```

注意，在所有 max() 的重载中参数都是传值。通常而言，这是一个好注意，不去改变超过重载函数模板所需要的东西。你应该限制改变参数数量或者限制改变为显式地指定模板参数。否则，会有一些预期之外的事发生。比如，如果实现了参数传引用的 max() 模板，然后重载参数传值的 C-strings，你就不能使用 three-argument 版本的 max() 去计算三个 C-strings：
```
#include <iostream>
#include <cstring>
using namespace std;
// maximum of two values of any type:
template <typename T>
T const& max(T const& a, T const& b)
{
    cout << "maximum of two values of any type\n";
    return b < a ? a : b;
}
// maximum of two C-strings(call-by-value):
char const* max(char const* a, char const* b)
{
    cout << "maximum of two C-strings(call-by-value)\n";
    return strcmp(b, a) ? a : b;
}
// maximum of three values of any type(call-by-reference):
template <typename T>
T const& max(T const& a, T const& b, T const& c)
{
    cout << "maximum of three values of any type(call-by-reference)\n";
    return ::max(::max(a,b), c); // error if max(a,b) uses call-by-value
}

int main()
{
    auto m1 = ::max(7,42,68); // OK
    char const* s1 = "a";
    char const* s2 = "b";
    char const* s3 = "c";
    //auto m2 = ::max(s1, s2, s3); // run-time ERROR(undefined behavior)
}
```

问题出现在以下语句，如果你调用对于三个 C-strings 的 max()：
```
auto m2 = ::max(s1, s2, s3);
```
会导致运行时错误，因为 max(a,b) 创建了一个新的临时局部变量，其被返回引用，但该临时变量在返回语句完成后就销毁了，留给 main() 的只是一个悬挂引用。不幸地是，这个错误十分微妙，且难以察觉。

相反，对于第一个调用，不会产生这个问题。因为这些临时变量（7，42 和 68）是在 main() 中创建，它们直到 main() 语句结束才被销毁。

这是唯一一个例子由于重载解析规则导致代码行为和预期不符。此外，需要保证所有重载的版本在调用前声明。因为当一个相应的函数调用时，不是所有的重载函数都可以被看到。比如，定义 three-argument 版本但没有看到 two-argument 版本。