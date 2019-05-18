## __attribute__ 相关的设置
#### __attribute__ ((__packed__))
用于告诉编译器取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐，是GCC特有的语法。
在GCC非紧凑模式下：
struct s{ char ch; int a; }; sizeof(int) = 4; sizeof(s) = 8;
在GCC紧凑模式下：
struct s{ char ch; int a; } __attribute__ ((__packed__));  sizeof(int) = 4; sizeof(s) = 5;

#### __attribute__ ((section("name")))
