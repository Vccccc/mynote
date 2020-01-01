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