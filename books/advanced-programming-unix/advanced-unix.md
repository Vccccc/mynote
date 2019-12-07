# Advanced Programming in the UNIX

## 8 Progress Control
### 8.1 Process Identifiers
Every process has a unique process ID, a non-negative integer.

Although unique, process IDs are reused. As processes terminate, their IDs become candidates for reuse. Most UNIX systems implement algorithms to delay reuse, however, so that newly created processes are assigned IDs different from those used by processes that terminated recently.

In addition to the process ID, there are other identifiers for every process. The
following functions return these identifiers.
```c
#include <unistd.h>
pid_t getpid(void);
Returns: process ID of calling process
pid_t getppid(void);
Returns: parent process ID of calling process
uid_t getuid(void);
Returns: real user ID of calling process
uid_t geteuid(void);
Returns: effective user ID of calling process
gid_t getgid(void);
Returns: real group ID of calling process
gid_t getegid(void);
Returns: effective group ID of calling process
```

### 8.3 fork Function
An existing process can create a new one by calling the fork function.
```c
#include <unistd.h>
pid_t fork(void);
Returns: 0 in child, process ID of child in parent, −1 on error

There are two uses for fork:
1. When a process wants to duplicate itself so that the parent and the child can
each execute different sections of code at the same time. This is common for
network servers—the parent waits for a service request from a client. When the
request arrives, the parent calls fork and lets the child handle the request. The
parent goes back to waiting for the next service request to arrive.
2. When a process wants to execute a different program. This is common for
shells. In this case, the child does an exec (which we describe in Section 8.10)
right after it returns from the fork.
```