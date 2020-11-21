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
Note that automatic type conversions are limited d