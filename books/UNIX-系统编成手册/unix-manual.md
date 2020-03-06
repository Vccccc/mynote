# UNIX 系统编成手册

## chapter 4 文件I/O
### open
打开或者创建文件。
```c
// int open(const char *pathname, int flags, mode_t mode)
// 如果不是创建文件， mode 不用指定
void create_c()
{
    int fd = open("create.txt", O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        perror("failed to open file");
        return;
    }
    close(fd);
}
// 如果文件存在，打开文件会失败
void open_c()
{
    int fd = open("open.txt", O_TRUNC |O_RDWR | O_APPEND);
    if (fd < 0)
    {
        perror("failed to open file");
        return;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    create_c();
    open_c();
    return 0;
}
```

### read write
读写文件
```
void read_c()
{
    int fd = open("create.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("failed to open file");
        return;
    }

    char buf[1024];
    int n = read(fd, buf, sizeof buf - 1);
    if(n < 0)
    {
        perror("failed to read");
        return;
    }
    buf[n] = '\0';
    printf("%s", buf);
    close(fd);
}

void write_c()
{
    int fd = open("create.txt", O_WRONLY | O_APPEND | O_TRUNC);
    if (fd < 0)
    {
        perror("failed to open file");
        return;
    }

    const char* str = "hello\nwolrd\n";
    int n = write(fd, str, strlen(str));
     if(n < 0)
    {
        perror("failed to write");
        return;
    }
    close(fd);
}
int main(int argc, char *argv[])
{
    write_c();
    read_c();
    
    return 0;
}

output:
hello
wolrd

```


## chapter 10 时间

两种时间类型：
- 真实时间：度量这一时间的起点有二：一为某个标准点；二为进程声明周期内的某个
固定时点（通常为程序启动）。前者为日历（calendar）时间，适用于需要对数据库记录或
文件上打上时间戳的程序；后者则称之为流逝（elapsed）时间或者挂钟（wall clock），
主要针对需要周期性操作或定期从外部输入设备进行度量的程序。
- 进程时间：一个进程所使用的 CPU 时间总量，适用于对程序、算法性能的检查和优化。

### 10.1 日历时间（Calendar time）

无论地理位置如何，UNIX 系统内部对时间的表示方式均是以自 Epoch 以来的秒数来度量的。
Epoch 亦即通用协调时间（UTC）的 1970 年 1 月 1 日早晨零点。日历时间存储于类型为 time\_t的变量中。

系统调用 gettimeofday()，可于 tv 指向的缓冲区中返回日历时间。
```c
#include <sys/time.h>
int gettimeofday(struct timeval* tv, struct timezone* t);
    Returns 0 on success, or -1 on error
```
参数 tv 是指向如下数据结构的一个指针：
```c
struct timeval
{
    time_t tv_sec; /* Seconds since 00:00:00, 1 Jan 1970 UTC */
    suseconds_t tv_usec; /* Additional microseconds (long int) */
};
```
虽然 tv\_usec 字段能提供微妙级精度，但其返回值的准确性则由依赖于架构的具体实现来决定。
gettimeofday() 的参数 tz 是个历史产物，目前已废弃，应始终将其置为 NULL。

time()系统调用返回自 Epoch 以来的秒数（和函数 gettimeofday() 所返回的 tv 参数中 tv\_sec 字段的数值相同）。
```c
#include <time.h>
time_t time(time_t* timeep);
    Returns number of seconds since the Epoch, or(time_t)-1 on error
```

如果参数 timep 不为 NULL，那么还会将自 Epoch以来的秒数置于 timep 所指向的位置。
由于 time() 会以两种方式返回相同值，而使用时唯一可能出错的地方是赋予 timep 参数一个无效地址，因此
往往会简单地采用如下调用（不做错误检查）：t = time(NULL)

### 10.2 时间转换函数

如下图所示，用于在 time\_t 值和其他时间格式之间相互转换的函数，其中包括打印输出。

![获取和使用日历时间的函数](./photo/10-1.png)

### 10.2.1 将 time\_t 转换为可打印格式
为了将 time\_t 转换为可打印格式，ctime() 函数提供了一个简单方法。

```c
#include <time.h>
char* ctime(const time_t* timep);
    Returns pointer to statically allocated string terminated by newline 
    and \0 on success, or NULL on error
```

把一个指向 time\_t 的指针为 timep 参数传入函数 ctime()，将返回一个长达 26 字节的字符串，
内含标准格式的日期和时间，如下所示：Thu Mar  5 17:24:33 2020

SUSv3 规定，调用 Ctime(), gmtime(), localTime() 或 asctime() 中的任一函数，都可能会覆盖
由其他函数返回，且经静态分配的数据结构。

### 10.2.2 time\_t 和分解时间之间的转换

函数 gmtime() 和 localtime() 可将一 time\_t 值转换为一个所谓的分解时间（broken-down time)。
分解时间被置于一个经由静态分配的结构中，其地址则作为函数结果返回。

```c
#include <time.h>
struct tm* gmtime(const time_t* timep);
struct tm* localtime(const time_t* timep);
    Both return a pointer to a statically allocated broken-down time structure
    on success, or NULL on error
```
