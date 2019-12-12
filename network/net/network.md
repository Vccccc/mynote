## man 7 ip 查询 IP 相关信息
```c
//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//	IP 协议不支持端口，端口由上层协议实现，如 TCP, UDP
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };
```
##  #include <arpa/inet.h>
>主要是与网络地址相关定义和函数
#### inet_pton inet_ntop
```c
#include <arpa/inet.h>
int inet_pton(int af, const char* src, void* dst);
	Returns: 1 on success. 0 if src does not contain a character string 
representing a valid network address in the specified address family. -1 if af does 
not contain a valid address family.

const char *inet_ntop(int af, const void *src,
                             char *dst, socklen_t size);
	Returns: On success, inet_ntop() returns a non-null pointer  to  dst. NULL
 is returned if there was an error, with errno set to indicate the error
```

#### htonl htons ntohl ntohs
```c
#include <arpa/inet.h>
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```

## #include <sys/socket.h>
>主要是与socket相关函数
#### socket shutdown getsockname getpeername
```c
#include <sys/types.h> // 在linux下不是必须的
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
	Returns: On  success,  a  file  descriptor  for  the new socket is returned. 
 On error, -1 is returned, and errno is set appropriately.

Description: domain常用AF_INET。type常用SOCK_STREAM、SOCK_DGRAM、SOCK_NONBLOCK、SOCK_CLOEXEC。
protocol常置为0。

#include <sys/socket.h>
int shutdown(int sockfd, int how)
	Returns: On  success,  zero is returned.  On error, -1 is returned, 
and errno is set appropriately.

Description: 参数how可指定为SHUT_RD、SHUT_WR或SHUT_RDWR。

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	Returns: On  success,  zero is returned.  On error, -1 is returned, 
and errno is set appropriately.

Description: getsockname() returns the current address to which the socket sockfd is
bound, in the buffer pointed to by addr. The addrlen argument  should be  
initialized to indicate the amount of space pointed to by addr.

int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	Returns: On  success,  zero is returned.  On error, -1 is returned, 
and errno is set appropriately.

Description: getpeername()  returns  the address of the peer connected to the socket
sockfd, in the buffer pointed to by addr.  The addrlen argument  should be  
initialized to indicate the amount of space pointed to by addr.
```

####  connect
```c
int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
	Returns: If  the connection or binding succeeds, zero is returned.  
On error, -1 is returned, and errno is set appropriately.

```

#### bind listen accept
```c
#include <sys/types.h> // 在linux下不是必须的
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
	Returns: On  success,  zero is returned.  On error, -1 is returned, and errno
 is set appropriately.

int listen(int sockfd, int backlog);
	Returns: On  success,  zero is returned.  On error, -1 is returned, 
and errno is set appropriately.

Description: backlog参数描述的是服务器端TCP ESTABLISHED 状态对应的全连接队列长度，
somaxconn是内核定义的最大长度。

int accept(int sockfd, struct sockaddr* addr, socklen_t addrlen);
int accept4(int sockfd, struct sockaddr* addr, socklen_t addrlen, int flags);
	Both return: On  success,  these system calls return a nonnegative integer
that is a descriptor for the accepted socket.  On  error,  -1  is  returned, 
and errno is set appropriately.

Decription: accept4和accept区别在于提供了参数flag，flag可置为SOCK_NONBLOCK、SOCK_CLOEXEC
```

#### getsockname getpeername
```c
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	Returns: On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
Description: 获取 sockfd 当前所绑定的地址

int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	Returns:On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
Description: 获取远端连接 sockfd 的地址
```

## #include <endian.h>
#### 大小端转换
```c
#include <endian.h>
       uint16_t htobe16(uint16_t host_16bits);
       uint16_t htole16(uint16_t host_16bits);
       uint16_t be16toh(uint16_t big_endian_16bits);
       uint16_t le16toh(uint16_t little_endian_16bits);

       uint32_t htobe32(uint32_t host_32bits);
       uint32_t htole32(uint32_t host_32bits);
       uint32_t be32toh(uint32_t big_endian_32bits);
       uint32_t le32toh(uint32_t little_endian_32bits);

       uint64_t htobe64(uint64_t host_64bits);
       uint64_t htole64(uint64_t host_64bits);
       uint64_t be64toh(uint64_t big_endian_64bits);
       uint64_t le64toh(uint64_t little_endian_64bits);
```
The functions with names of the form "htobenn" convert from host byte order to big-endian order.
The functions with names of the form "htolenn" convert from host byte order to little-endian order.
The functions with names of the form "benntoh" convert from big-endian order to host byte order.
The functions with names of the form "lenntoh" convert from little-endian order to host byte order.

## #include <poll.h>
```c
struct pollfd {
               int   fd;         /* file descriptor */
               short events;     /* requested events */
               short revents;    /* returned events */
           };
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	Returns: On  success, a positive number is returned; this is the number of 
structures which have nonzero revents fields (in other words, those descriptors with 
events or errors reported).  A value of  0  indicates  that  the  call timed out and no 
file descriptors were ready.  On error, -1 is returned, and errno is set appropriately.

Description: 参数 timeout 是毫秒。常用 bit mask 有 POLLIN, POLLOUT, POLLHUP 分别监听可
读、可写和 hang up 事件。
```

## #include <sys/select.h>
```c
 /* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, struct timeval *timeout);
	Returns: On success, select() and pselect() return the number of file  
descriptors  contained  in  the  three  returned descriptor  sets (that is, the total 
number of bits that are set in readfds, writefds, exceptfds) which may be zero if the 
timeout expires before anything interesting happens.  On error, -1 is returned, and errno 
 is  set to indicate the error; the file descriptor sets are unmodified, and timeout 
becomes undefined.

Description: 参数 nfds 应设为 最大文件描述符的数值加 1。

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);
```

## #include <sys/time.h>
```
struct timeval {
               long    tv_sec;         /* seconds */
               long    tv_usec;        /* microseconds */
           };

struct timespec {
               long    tv_sec;         /* seconds */
               long    tv_nsec;        /* nanoseconds */
           };

 struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };
```
```c
Example:
#include <stdio.h>
       #include <stdlib.h>
       #include <sys/time.h>
       #include <sys/types.h>
       #include <unistd.h>

       int
       main(void)
       {
           fd_set rfds;
           struct timeval tv;
           int retval;

           /* Watch stdin (fd 0) to see when it has input. */

           FD_ZERO(&rfds);
           FD_SET(0, &rfds);

           /* Wait up to five seconds. */

           tv.tv_sec = 5;
           tv.tv_usec = 0;

           retval = select(1, &rfds, NULL, NULL, &tv);
           /* Don't rely on the value of tv now! */

           if (retval == -1)
               perror("select()");
           else if (retval)
               printf("Data is available now.\n");
               /* FD_ISSET(0, &rfds) will be true. */
           else
               printf("No data within five seconds.\n");

           exit(EXIT_SUCCESS);
       }
```

## 定时器
### #include <time.h>
```c
 struct timespec {
               time_t tv_sec;                /* Seconds */
               long   tv_nsec;               /* Nanoseconds */
           };

           struct itimerspec {
               struct timespec it_interval;  /* Interval for periodic timer */
               struct timespec it_value;     /* Initial expiration */
           };

#include <sys/timerfd.h>
int timerfd_create(int clockid, int flags);
int timerfd_settime(int fd, int flags,
             const struct itimerspec *new_value, struct itimerspec *old_value);
int timerfd_gettime(int fd, struct itimerspec *curr_value);
	Returns:  On  success,  timerfd_create()  returns  a  new file descriptor.  On 
error, -1 is returned and errno is set to indicate the error. timerfd_settime() and 
timerfd_gettime() return 0 on success; on error they return -1, and set errno  to  indi‐
cate the error.

Description: timerfd_create 创建一个 timer object，返回一个文件描述符引用这个 object。参
数 clockid 用于指定标记 timer 进度的时钟，它必须唯一指定如下值：
CLOCK_REALTIME: 可设置的系统时钟
CLOCK_MONOTONIC: 一个不可设置的单调递增时钟，它用于测量从过去某个未指定、在系统  startup 
后无法更改的时间点的时间
参数 flags 可以按位或改变 timerfd_create 的行为：
TFD_NONBLOCK: Set  the  O_NONBLOCK  file  status flag on the new open file description.
TFD_CLOEXE: Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor.

timerfd_settime() arms (starts) or disarms (stops) the timer referred to by the file 
descriptor fd.
new_value.it_value specifies the initial expiration of the timer, in seconds and 
nanoseconds.  Setting  either field  of  new_value.it_value to a nonzero value arms the 
timer.  Setting both fields of new_value.it_value to zero disarms the timer. Setting one
 or both fields of new_value.it_interval to nonzero values specifies the  period,  in  
seconds  and nanoseconds,   for   repeated   timer   expirations   after   the  initial
  expiration.   If  both  fields  of new_value.it_interval are zero, the timer expires 
just once, at the time specified by new_value.it_value. By default, the initial 
expiration time specified in new_value is interpreted relative to the current time  on 
the  timer's  clock at the time of the call (i.e., new_value.it_value specifies a time 
relative to the current value of the clock specified by clockid).

The it_value field returns the amount of time until the timer will next expire.  If both
 fields of this structure are zero, then the timer is currently disarmed.  This field 
always contains a relative value
```
#### Operating on a timer file descriptor
- **read**：If the timer has already expired one or  more  times  since  its  settings  
were  last  modified  using timerfd_settime(),  or  since  the last successful read(2),
then the buffer given to read(2) returns an unsigned 8-byte integer (**uint64_t**) 
containing the number  of  expirations  that  have  occurred.   (The returned value is 
in host byte order—that is, the native byte order for integers on the host machine.) If
no timer expirations have occurred at the time of the read(2), then the call either 
blocks until the next timer expiration, or fails with the error EAGAIN if the file 
descriptor has been made  nonblocking (via the use of the fcntl(2) F_SETFL operation to set the O_NONBLOCK flag).
- **poll(2), select(2) (and similar)**：The file descriptor is readable (the select(2) readfds argument; the poll(2) POLLIN  flag)  if  one  or more timer expirations have occurred.


## #include <eventfd.h>
```c
#include <sys/eventfd.h>
int eventfd(unsigned int initval, int flags);
	Returns:On  success,  eventfd()  returns  a new eventfd file descriptor.  On 
error, -1 is returned and errno is set to indicate the error.

Description: eventfd()  creates an "eventfd object" that can be used as an event
 wait/notify mechanism by user-space applications, and by the kernel to notify user-space
 applications of  events.   The  object  contains  an  unsigned 64-bit  integer 
(uint64_t)  counter  that  is maintained by the kernel.  This counter is initialized with
 the value specified in the argument initval.
```
参数 flags 可以按位或改变 eventfd 的行为：
- **EFD_CLOEXEC**
- **EFD_NONBLOCK**
- **EFD_SEMAPHORE**:Provide semaphore-like semantics for reads from the new file descriptor

**Operating on a eventfd file descriptor**
- **read(2)**: Each  successful  read(2) returns an 8-byte integer.  A read(2) fails with the error EINVAL if the size of the supplied buffer is less than 8 bytes.The  semantics  of  read(2)  depend  on  whether  the eventfd counter currently has a nonzero value and
whether the EFD_SEMAPHORE flag was specified when creating the eventfd file descriptor:
	1. If EFD_SEMAPHORE was not specified and the eventfd counter has  a  nonzero  value,  then  a  read(2) returns 8 bytes containing that value, and the counter's value is reset to zero.
	2. If EFD_SEMAPHORE was specified and the eventfd counter has a nonzero value, then a read(2) returns 8 bytes containing the value 1, and the counter's value is decremented by 1.
	3. If the eventfd counter is zero at the time of the call to read(2), then the call either blocks until the  counter  becomes nonzero (at which time, the read(2) proceeds as described above) or fails with the error EAGAIN if the file descriptor has been made nonblocking.
- **write(2)**:A write(2) call adds the 8-byte integer value supplied in its buffer to the counter.  The maximum value that   may   be   stored  in  the  counter  is  the  largest  unsigned  64-bit  value  minus  1  (i.e., 0xfffffffffffffffe).  If the addition would cause the counter's value to exceed the maximum,  then  the write(2)  either  blocks  until  a read(2) is performed on the file descriptor, or fails with the errorEAGAIN if the file descriptor has been made nonblocking.
A write(2) fails with the error EINVAL if the size of the supplied buffer is less than 8 bytes,  or  if an attempt is made to write the value 0xffffffffffffffff.
- **poll(2), select(2) (and similar)**:The returned file descriptor supports poll(2) (and analogously epoll(7)) and select(2), as follows:
	1. The  file  descriptor  is  readable (the select(2) readfds argument; the poll(2) POLLIN flag) if the counter has a value greater than 0.
	2. The file descriptor is writable (the select(2) writefds argument; the poll(2) POLLOUT flag) if it is possible to write a value of at least "1" without blocking.
	3. If  an  overflow  of the counter value was detected, then select(2) indicates the file descriptor as being both readable and writable, and poll(2) returns a POLLERR event.  As noted above, write(2) can never  overflow the counter.  However an overflow can occur if 2^64 eventfd "signal posts" were performed by the KAIO subsystem (theoretically possible, but practically unlikely).  If an overflow has occurred, then read(2) will return that maximum uint64_t value (i.e., 0xffffffffffffffff).

# Muduo
## IP 地址的抽象
