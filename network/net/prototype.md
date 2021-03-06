```c
//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
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

Description: domain常用AF_INET。type常用SOCK_STREAM、SOCK_DGRAM。protocol常置为0。

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
