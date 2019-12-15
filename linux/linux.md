## Ubuntu 

## syscall
```c
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

long syscall(long number, ...);
	Reuturns:  The return value is defined by the system call being invoked.  In 
general, a 0 return value indicates success. A -1 return value indicates an error, and an
 error code is stored in errno.

Description:
  syscall 是库函数，可以用于调用一些汇编接口的系统调用，这些系统调用没有C语言包装函数。
```

## gettid
```c
#include <sys/types.h>
pid_t gettid(void);
	Returns: On success, returns the thread ID of the calling process. This call is 
always successful.

Description:  gettid()  returns  the  caller's thread ID (TID).  In a single-threaded 
process, the thread ID is equal to the process ID (PID, as returned by getpid(2)).  In a 
multithreaded process, all threads have the  same  PID,  but each one has a unique TID.

Note: There is no glibc wrapper for this system call
```