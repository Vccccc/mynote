## __attribute__ 相关的设置
#### __attribute__ ((__packed__))
用于告诉编译器取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐，是GCC特有的语法。
在GCC非紧凑模式下：
struct s{ char ch; int a; }; sizeof(int) = 4; sizeof(s) = 8;
在GCC紧凑模式下：
struct s{ char ch; int a; } __attribute__ ((__packed__));  sizeof(int) = 4; sizeof(s) = 5;

#### __attribute__ ((section("name")))
在全局变量或者函数之前加上__attribute__ ((section("name"))) 属性就可以把相应的变量或者函数放到以name作为段名的段中。
比如：
__attribute__ ((section("Foo"))) int global = 42;

#### __attribute__ ((weak))
定义任何一个强符号为弱符号
__attribute__((weak)) weak2 = 2;


#### __attribute__ ((weakref))
声明一个外部的引用为弱引用
__attribute__((weakref)) void foo();