#### perror
```c
#include <stdio.h>
void perror(const char* s);

Description
The perror() function produces a message on standard error describing
the last error encountered during a call to a system or  library  function.
First (if s is not NULL and *s is not a null byte ('\0')), the argument
string s is printed, followed by a colon and a blank.   Then  an  error
message corresponding to the current value of errno and a new-line.

```