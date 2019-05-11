# cmake 
------------

### 1. 创建一个 CMakeLists.txt 文件
### 2. cmake 语法
**find_package**，找到一个外部的project，并加载它的设置。
```c
find_package(<PackageName> [version] [EXACT] [QUIET] [MODULE]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
             [NO_POLICY_SCOPE])
```
PackageName: 外部project名
version: project 版本
REQUIRED: 如果project未找到，则停止程序并打印错误信息。
COMPONENTS: 指定project的组件列表

**eg.**
find_pack


