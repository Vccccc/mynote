# Chapter 1
#### Basic wildcards
**Wildcard** | **Matches**
|:--------|:------------|
| ? | Any single character|
| * | Any string of characters|
| [set] | Any character in set |
| [!set] | Any character not in set |

#### Using the set construct wildcards
**Expression** | **Matches** |
|:------|:------------|
| [abc] | a, b, or c |
| [.,;] | Perioad, comma, or semicolon |
| [-_] | Dash or underscore |
| [a-c] | a, b, or c |
| [a-z] | All lowercase letters |
| [!0-9] | All non-digits |
| [0-9!] | All digits and exclamation point |
| [a-zA-Z] | All lower-and uppercase letters |
| [a-zA-Z0-9_-] | All letters, all digits, underscore, and dash |

#### Popular UNIX data filtering utilities
**Utility** | **Purpose** |
|:----------|:-----------|
cat | Copy input to output 
grep | Search for strings in the input
sort | Sort lines in the input 
cut | Extract columns from input 
sed | Perform editing operations on input 
tr | Translate characters in the input to other characters

#### cat cut sort 指令的使用
![](photo/cat-cut-sort.png)

#### cut 指令的使用:cut作用于每一行，-d: 表示用:作为分隔域(field), -f配合-d使用，-f1是第一域。-b指定第几字节
![](photo/cut.png)

#### background/foreground jobs
1. background job, put an ampersand(&) after the command
2. foreground job, a job run the normal way 

command **jobs**: For each background job, **jobs** prints a line to show an indication of the job's status.

### TODO: 将See Chapter改成超链接
#### Special characters
**Character** | **Meaning** | **See** **Chapter**
:-----------|:-------------|:-----------------
~ | Home directory | Chapter 1
` | Command substitution | Chatper 4
# | Comment | Chapter 4
$ | Variable expression | Chapter 3
& | Background job | Chapter 1
* | String wildcard | Chatper 1
( | Start subshell | Chapter 8
) | End subshell | Chapter 8
\ | Quote next character | chatper 1
\| | Pipe | Chapter 1
[ | Start character-set wildcard | Chapter 1
] | End character-set wildcard | Chapter 1
{ | Start command block | Chapter 7
} | End command block | Chapter 7
; | Shell command separator | Chapter 3
' | Strong quote | Chapter 1
<"> | Weak quote | Chapter 1
< | Input redirect | Chapter 1 
> | Output redirect | Chapter 1
/ | Pathname directory separator | Chapter 1
? | Single-character wildcard | Chapter 1
! | Pipeline logical NOT | Chapter 5

# Chapter 3
#### source 
**source** executes the commands in the specified file.

### 3.2 Aliases
alias name=command
This syntax specifies that name is an alias for command.

alias cdcv='cd ./cv'
Notice the quotes around the full cd command; these are
necessary if the string being aliased consists of more than
one word.

### 3.4.2.4 Command search path
PATH是一个变量，shell通过它保存的地址来找到输入的命令 
**echo** **$PATH**，可以回显PATH的值
可以添加路径在PATH中，在~/.bashrc写入如下指令
PATH=$PATH":/home/cv/whatever"
### 3.4.25 Command hashing
每次通过PATH搜索命令会花费许多时间，所以bash有一个hash table用于
存储最近使用过的命令。
在终端输入**hash**，就可以查看最近使用过的命令

### 4.2 Shell Variables
1. **$#**, 表示命令行参数个数(包括命令本身)
2. **$\***, 表示一个由所有命令行参数(除了第0个参数)组成的，由IFS(internal field separator)分隔