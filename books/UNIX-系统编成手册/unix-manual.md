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
