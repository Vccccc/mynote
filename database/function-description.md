# C API Function Descriptions
### mysql\_init()
```c
MYSQL *mysql_init(MYSQL *mysql)
    Returns: An initialized MYSQL* handler. NULL if there was insufficient memory to 
allocate a new object
Description:
  Allocates or initializes a MYSQL object suitable for mysql_real_connect(). 
If mysql is a NULL pointer, the function allocates, initializes, and returns 
a new object. Otherwise, the object is initialized and the address of the object 
is returned. If mysql_init() allocates a new object, it is freed when mysql_close() 
is called to close the connection.

In a nonmultithreaded environment, mysql_init() invokes mysql_library_init() 
automatically as necessary. However, mysql_library_init() is not thread-safe in a 
multithreaded environment, and thus neither is mysql_init(). Before calling 
mysql_init(), either call mysql_library_init() prior to spawning any threads, or use 
a mutex to protect the mysql_library_init() call. This should be done prior to any 
other client library call.
```
### mysql\_real\_connect()
```C
MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user,
    const char *passwd, const char *db, unsigned int port,
    const char *unix_socket, unsigned long client_flag)
Description:
  The value of host may be either a host name or an IP address. 
The client attempts to connect as follows:
    If host is NULL or the string "localhost", a connection to the local 
host is assumed:
        On Unix, the client connects using a Unix socket file. The unix_socket 
        parameter or the MYSQL_UNIX_PORT environment variable may be used 
        to specify the socket name.
```
### mysql\_query()
```C
int mysql_query(MYSQL *mysql, const char *stmt_str)
    Returns: 0 for success. Nonzero if an error occurred.
Description:
  Executes the SQL statement pointed to by the null-terminated string 
stmt_str. Normally, the string must consist of a single SQL statement 
without a terminating semicolon (;) or \g. If multiple-statement execution 
has been enabled, the string can contain several statements separated by 
semicolons.
```
### mysql\_real\_query()
```C
int mysql_real_query(MYSQL *mysql, const char *stmt_str, 
        unsigned long length)
    Returns: 0 for success. Nonzero if an error occurred.
Description:
  mysql_real_query() executes the SQL statement pointed to by stmt_str,
a string length bytes long. Normally, the string must consist of a single
SQL statement without a terminating semicolon (;) or \g. If multiple-statement 
execution has been enabled, the string can contain several statements 
separated by semicolons
  mysql_query() cannot be used for statements that contain binary data; 
you must use mysql_real_query() instead. (Binary data may contain the \0 
character, which mysql_query() interprets as the end of the statement string.)
In addition, mysql_real_query() is faster than mysql_query() because it 
does not call strlen() on the statement string.
```
### mysql\_store\_result()
```C
MYSQL_RES *mysql_store_result(MYSQL *mysql)
    Returns: A pointer to a MYSQL_RES result structure with the results. 
    NULL if the statement did not return a result set or an error occurred. 
    To determine whether an error occurred, check whether mysql_error() 
    returns a nonempty string, mysql_errno() returns nonzero, 
    or mysql_field_count() returns zero.
```
### mysql\_free\_result()
```C
void mysql_free_result(MYSQL_RES *result)

Description:
  mysql_free_result() frees the memory allocated for a result set by 
mysql_store_result(), mysql_use_result(), mysql_list_dbs(), and so forth. 
When you are done with a result set, you must free the memory it uses by 
calling mysql_free_result().
Do not attempt to access a result set after freeing it.
```
### mysql\_next\_result()
```C
int mysql_next_result(MYSQL *mysql)
    Returns: 0 for successful and there are more results.
             -1 for successful and there are no more results
             >0 for an error occurred 

Description:
  mysql_next_result() is used when you execute multiple statements 
specified as a single statement string, or when you use CALL statements to 
execute stored procedures, which can return multiple result sets.
mysql_next_result() reads the next statement result and returns a status to 
indicate whether more results exist. If mysql_next_result() returns an 
error, there are no more results.
Before each call to mysql_next_result(), you must call mysql_free_result() 
for the current statement if it is a statement that returned a result set 
(rather than just a result status).
```
### mysql\_error()
```C
const char* mysql_error(MYSQL* mysql)
    Returns: A null-terminated character string that describes the error. 
    An empty string if no error occurred.
Description:
  For the connection specified by mysql, mysql_error() returns a 
null-terminated string containing the error message for the most recently 
invoked API function that failed. If a function did not fail, the return 
value of mysql_error() may be the previous error or an empty string to 
indicate no error.
```
### mysql\_field\_count()
```C
unsigned int mysql_field_count(MYSQL *mysql)
    Returns: An unsigned integer representing the number of columns in a result set.
Description:
  Returns the number of columns for the most recent query on the connection.
The normal use of this function is when mysql_store_result() returned NULL 
(and thus you have no result set pointer). In this case, you can call 
mysql_field_count() to determine whether mysql_store_result() should have 
produced a nonempty result.
```
### mysql\_affected\_rows()
```C
my_ulonglong mysql_affected_rows(MYSQL *mysql)
    Returns: An integer greater than zero indicates the number of rows 
affected or retrieved. Zero indicates that no records were updated for an 
UPDATE statement, no rows matched the WHERE clause in the query or that 
no query has yet been executed. -1 indicates that the query returned an 
error or that, for a SELECT query, mysql_affected_rows() was called prior
to calling mysql_store_result().
Because mysql_affected_rows() returns an unsigned value, you can check 
for -1 by comparing the return value to (my_ulonglong)-1 (or to 
(my_ulonglong)~0, which is equivalent).

Description:
  mysql_affected_rows() may be called immediately after executing a 
statement with mysql_query() or mysql_real_query(). It returns the number 
of rows changed, deleted, or inserted by the last statement if it was an 
UPDATE, DELETE, or INSERT. For SELECT statements, mysql_affected_rows() 
works like mysql_num_rows().
```
### mysql\_fetch\_fields()
```C
MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result)
    Returns: An array of MYSQL_FIELD structures for all columns of a result 
  set. NULL if the result set has no metadata.
Descriptions :
  Returns an array of all MYSQL_FIELD structures for a result set. Each 
structure provides the field definition for one column of the result set.
For metadata-optional connections, this function returns NULL when the 
resultset_metadata system variable is set to NONE. To check whether a 
result set has metadata, use the mysql_result_metadata() function.
```
### mysql\_fetch\_row()
```C
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
    Returns: A MYSQL_ROW structure for the next row, or NULL. The meaning 
of a NULL return depends on which function was called preceding mysql_fetch_row():
1, When used after mysql_store_result() or mysql_store_result_nonblocking(), 
mysql_fetch_row() returns NULL if there are no more rows to retrieve.
2, When used after mysql_use_result(), mysql_fetch_row() returns NULL if 
there are no more rows to retrieve or an error occurred. To determine whether
an error occurred, check whether mysql_error() returns a nonempty string or 
mysql_errno() returns nonzero.

Description:
  mysql_fetch_row() retrieves the next row of a result set:
1, When used after mysql_store_result() or mysql_store_result_nonblocking(),
mysql_fetch_row() returns NULL if there are no more rows to retrieve.
2, When used after mysql_use_result(), mysql_fetch_row() returns NULL if 
there are no more rows to retrieve or an error occurred.

The number of values in the row is given by mysql_num_fields(result). If 
row holds the return value from a call to mysql_fetch_row(), pointers to 
the values are accessed as row[0] to row[mysql_num_fields(result)-1]. NULL 
values in the row are indicated by NULL pointers.
The lengths of the field values in the row may be obtained by calling 
mysql_fetch_lengths(). Empty fields and fields containing NULL both have 
length 0; you can distinguish these by checking the pointer for the field 
value. If the pointer is NULL, the field is NULL; otherwise, the field is 
empty.
```
### mysql\_fetch\_lengths()
```C
unsigned long *mysql_fetch_lengths(MYSQL_RES *result)
    Returns: An array of unsigned long integers representing the size of 
each column (not including any terminating null bytes). NULL if an error 
occurred.

Description:
  Returns the lengths of the columns of the current row within a result set. 
If you plan to copy field values, this length information is also useful for 
optimization, because you can avoid calling strlen(). In addition, if the 
result set contains binary data, you must use this function to determine 
the size of the data, because strlen() returns incorrect results for any 
field containing null characters.
The length for empty columns and for columns containing NULL values is zero
```
### mysql\_insert\_id()
```C
my_ulonglong mysql_insert_id(MYSQL *mysql)
    Returns: The return value of mysql_insert_id() is always zero unless 
explicitly updated under one of the following conditions:
1, INSERT statements that store a value into an AUTO_INCREMENT column. 
This is true whether the value is automatically generated by storing the 
special values NULL or 0 into the column, or is an explicit nonspecial value.
2, In the case of a multiple-row INSERT statement, mysql_insert_id() returns 
the first automatically generated AUTO_INCREMENT value that was successfully 
inserted.
3, If no rows are successfully inserted, mysql_insert_id() returns 0.
4, If an INSERT ... SELECT statement is executed, and no automatically 
generated value is successfully inserted, mysql_insert_id() returns the 
ID of the last inserted row.
5, If an INSERT ... SELECT statement uses LAST_INSERT_ID(expr), 
mysql_insert_id() returns expr.
6, INSERT statements that generate an AUTO_INCREMENT value by inserting 
LAST_INSERT_ID(expr) into any column or by updating any column to 
LAST_INSERT_ID(expr).
7, If the previous statement returned an error, the value of 
mysql_insert_id() is undefined.

Description:
  Returns the value generated for an AUTO_INCREMENT column by the previous 
INSERT or UPDATE statement. Use this function after you have performed an 
INSERT statement into a table that contains an AUTO_INCREMENT field, or 
have used INSERT or UPDATE to set a column value with LAST_INSERT_ID(expr).
```
### mysql\_real\_escape\_string()
```C
unsigned long mysql_real_escape_string(MYSQL *mysql, char *to, 
        const char *from, unsigned long length)
    Returns: The length of the encoded string that is placed into the to
argument, not including the terminating null byte, or -1 if an error occurs.
Because mysql_real_escape_string() returns an unsigned value, you can check 
for -1 by comparing the return value to (unsigned long)-1 
(or to (unsigned long)~0, which is equivalent).

Description:
  The mysql argument must be a valid, open connection because character 
escaping depends on the character set in use by the server.
The string in the from argument is encoded to produce an escaped SQL string,
taking into account the current character set of the connection. The result
is placed in the to argument, followed by a terminating null byte.
Characters encoded are \, ', ", NUL (ASCII 0), \n, \r, and Control+Z. 
Strictly speaking, MySQL requires only that backslash and the quote 
character used to quote the string in the query be escaped. mysql_real_escape_string() 
quotes the other characters to make them easier to read in log files. 
The string pointed to by from must be length bytes long. You must allocate 
the to buffer to be at least length*2+1 bytes long. (In the worst case, 
each character may need to be encoded as using two bytes, and there must be 
room for the terminating null byte.) When mysql_real_escape_string() returns, 
the contents of to is a null-terminated string. The return value is the 
length of the encoded string, not including the terminating null byte.
```

```C

```

```C

```

```C

```

```C

```

```C

```
