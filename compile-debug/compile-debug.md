# cmake 
------------

## 1. 创建一个 CMakeLists.txt 文件
## 2. cmake 语法

#### find_package
找到一个外部的project，并加载它的设置。
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
find_package(Boost 1.69 COMPONENTS program_options REQUIRED)

#### include_directories
Add the given directories to those the compiler uses to search for include files. 
Relative paths are interpreted as relative to the current source directory.
```c
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
```
**eg.**
include_directories(${Boost_INCLUDE_DIR})

#### add_executable
Add an executable to the project using the specified source files.
```c
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               [source1] [source2 ...])
```

**eg.**
add_executable(my_project 1.cpp 2.cpp)

# gdb
### Preparation
1. Compile with the -g option.
	Example. Compile the program printch.cpp:
g++ -g printch.cpp -o t
2. Start gdb and set the number of source lines to list.
	Example. Run gdb on the printch program and set the number of source lines to list at a time to 28.
	> gdb printch
	> (gdb) set listsize 28

### Setting Breakpoints
1. Set a breakpoint at the beginning of a function.
Example. Set a breakpoint at the beginning of main.
	>(gdb) b main
2. Set a breakpoint at a line of the current file during debugging.
Example. Set a breakpoint at line 35 while in file printch.cpp.
	>(gdb) b 35
3. Set a breakpoint at the beginning of a class member function.
Example. Set a breakpoint at the beginning of member function erase of the class list.
	>(gdb) b list::erase
4. Listing breakpoints.
Example. List all breakpoints which have been set so far in a debugging session.

	>(gdb) info b
Num Type　　　Disp Enb Address　　What
1   breakpoint　　keep y　0x0040104f in main at printch.cpp:27
2   breakpoint　　keep y　0x004010a7 in main at printch.cpp:35
5. Deleting a breakpoint.
Example. Delete the breakpoint at line 35.
	>(gdb) delete 2

### Running the Program being Debugged
1. Start the program being debugged.
Example 1. The program is printch, which can take an optional command line argument. Start it running with no command line argument.
	>(gdb) r

	Example 2. Start printch running with 			command line argument A.
	>(gdb) r A
2. Execute a single statement. If the statement is a function call, just single step into the function.
	>(gdb) s
3. Execute a single statement. If the statement is a function call, execute the entire function and return to the statement just after the call; that is, step over the function.
	>(gdb) n
4. Execute from the current point up to the next breakpoint if there is one, otherwise execute until the program terminates.
	>(gdb) c
5. Execute the rest of the current function; that is, step out of the function.
	>(gdb) finish

### Examining Variables
1. Print the value of a variable or expression.
	Example 1. Print the value of a variable count
	>(gdb) p count

	Example 2. Print the value of the expression fname[i+1]
	>(gdb) p fname[i+1]

### List Source Code and the Next Statement
1. List lines of source code.
	Example. List the next listsize number of lines of code. Note that listsize's value can be change with the set command.
	>(gdb) l
2. List lines of source code centered around a particular line.
	Example. List the lines centered around line 41.
	>(gdb) l 41
3. Show the next statement that will be executed.
	>(gdb) where
#0  mystrcpy (copyto=0x259fc6c "*", copyfrom=0x259fddc "ABC") at printch.cpp:27
#1  0x4010c8 in main (argc=3, argv=0x25b0cb8) at printch.cpp:40
	
	The statement at line 27 of the function mystrcpy is the next statement and the function mystrcpy was called by main.

