### Preparation
1. Compile with the -g option.
	Example. Compile the program printch.cpp:
g++ -g printch.cpp -o t
2. Start gdb and set the number of source lines to list.
	Example. Run gdb on the printch program and set the number of source lines to list at a time to 28.
	> gdb printch
	> (gdb) set listsize 28

### Setting Breakpoints
1. Set a breakpoint at the beginning of a function.
Example. Set a breakpoint at the beginning of main.
	>(gdb) b main
2. Set a breakpoint at a line of the current file during debugging.
Example. Set a breakpoint at line 35 while in file printch.cpp.
	>(gdb) b 35
3. Set a breakpoint at the beginning of a class member function.
Example. Set a breakpoint at the beginning of member function erase of the class list.
	>(gdb) b list::erase
4. Listing breakpoints.
Example. List all breakpoints which have been set so far in a debugging session.

	>(gdb) info b
Num Type　　　Disp Enb Address　　What
1   breakpoint　　keep y　0x0040104f in main at t.cpp:27
2   breakpoint　　keep y　0x004010a7 in main at t.cpp:35
5. Deleting a breakpoint.
Example. Delete the breakpoint at line 35.
	>(gdb) delete 2

### Running the Program being Debugged
1. Start the program being debugged.
Example 1. The program is printch, which can take an optional command line argument. Start it running with no command line argument.
	>(gdb) r

	Example 2. Start printch running with 			command line argument A.
	>(gdb) r A
2.