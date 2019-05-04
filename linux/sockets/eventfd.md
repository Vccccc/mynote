#### Name
eventfd - create a file descriptor for event notification.

#### Synopsis
#include <sys/eventfd.h>
int eventfd(unsigned int initval, int flags);

#### Description
eventfd()  creates  an  "eventfd  object"  that can be used as an event
       wait/notify mechanism by user-space applications, and by the kernel  to
       notify  user-space  applications  of  events.   The  object contains an
       unsigned 64-bit integer (uint64_t) counter that is  maintained  by  the
       kernel.   This  counter  is initialized with the value specified in the
       argument initval.
