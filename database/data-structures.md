# C API Data Structures
```C
MYSQL

This structure represents handler for one database connection. 
It is used for almost all MySQL functions. Do not try to make a copy of 
a MYSQL structure. There is no guarantee that such a copy will be usable.

MYSQL_RES

This structure represents the result of a query that returns rows 
(SELECT, SHOW, DESCRIBE, EXPLAIN). The information returned from a query 
is called the result set in the remainder of this section.

MYSQL_ROW

This is a type-safe representation of one row of data. 
It is currently implemented as an array of counted byte strings. 
(You cannot treat these as null-terminated strings if field values 
may contain binary data, because such values may contain null bytes 
internally.) Rows are obtained by calling mysql_fetch_row().

MYSQL_FIELD

This structure contains metadata: information about a field, such as 
the field's name, type, and size. Its members are described in more detail 
later in this section. You may obtain the MYSQL_FIELD structures for each 
field by calling mysql_fetch_field() repeatedly. Field values are not 
part of this structure; they are contained in a MYSQL_ROW structure.

MYSQL_FIELD_OFFSET

This is a type-safe representation of an offset into a MySQL field list.
(Used by mysql_field_seek().) Offsets are field numbers within a row, 
beginning at zero.
```