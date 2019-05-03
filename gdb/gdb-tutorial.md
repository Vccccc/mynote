### Preparation
1. Compile with the -g option.
	Example. Compile the program t.cpp:
g++ -g t.cpp -o t
2. Start gdb and set the number of source lines to list.
	Example. Run gdb on the printch program and set the number of source lines to list at a time to 28.
	> gdb t
	> (gdb) set listsize 28

### Setting Breakpoints
