# Advanced Programming in the UNIX

## 8 Progress Control
### 8.1 Process Identifiers
Every process has a unique process ID, a non-negative integer.

Although unique, process IDs are reused. As processes terminate, their IDs become candidates for reuse. Most UNIX systems implement algorithms to delay reuse, however, so that newly created processes are assigned IDs different from those used by processes that terminated recently.