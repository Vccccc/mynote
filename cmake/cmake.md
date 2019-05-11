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

