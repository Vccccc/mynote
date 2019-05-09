##  #include <arpa/inet.h>
>主要是与网络地址相关定义和函数
#### inet_pton
```c
#include <arpa/inet.h>
int inet_pton(int af, const char* src, void* dst);
	Returns:1 on success. 0 if src does not contain a character string 
representing a valid network address in the specified address family. -1 if af does 
not contain a valid address family.
```

#### htonl htons nto