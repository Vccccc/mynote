# Operating Systems in Three Easy Pieces 读书笔记

## Chapter 2 Introduction to Operating Systems
##### What did the processor do?
The processor **fetches** an instruction from memory, **decodes** it, and **executes** it.

### 2.5 Design goals
Design goals of OS:
- **Virtualization**. OS takes physical resources, such as a CPU, memory, or disk, and **virtualizes** them.
- **Concurrency**. OS handles tough and tricky issues related to **concurrency**.
- **Persistence**. OS stores files persistently.
- **High performance**. 
- **Protection**. OS provides **protection** between applications, as well as between the OS and applications. **isolating** processes from one another is the key to protection and thus underlies much of what an OS must do.
- **Reliability**. OS strive to provide a high degree of reliability.
- **Energy-efficiency** 
- **Security**. An extension of protection. Against malicious applications is critical, especially in these highly-networked times.
- **Mobility**. Mobility is increasingly important as OSes are run on smaller and smaller
devices.

## 4 The Abstraction: The Process 
To virtualization of the CPU, the OS will need both some low-level machinery and high-level intelligence. We call low-level machinery mechanisms; **mechanisms** are low-level methods or protocols that implement a needed piece of functionality.

**context switch** is a mechanism, which gives the OS the ability to stop running one program and start running another on a given CPU. this **time-sharing** mechanism is employed by all modern OSes.

On top of these mechanisms resides some of the intelligence in the OS, in the form of policies. **Policies** are algorithms for making some kind of decision within the OS. For example, given a number of possible programs to run on a CPU, which program should the OS run? A scheduling policy in the OS will make this decision, likely using historical information (e.g., which program has run more over the last minute?),
workload knowledge (e.g., what types of programs are run), and performance metrics (e.g., is the system optimizing for interactive performance,
or throughput?) to make its decision.

### 4.1 The Abstraction: A Process
##### What constitutes a process?
>**machine state**

**machine state**: What a program can read or update when it is running.

##### What parts of the machine are important to the execution of this program?
>1. **Memory**. One obvious component of machine state that comprises a process is its **memeory**. The memeory that the process can address(called its **address space**) is part 
of the process.
>2. **Registers**. Also part of the process's machine state are **registers**; many instructions explicitly read or update registers and thus clearly they are important to the execution of the process.

Note that there are some particularly special registers that form part of this machine state. For example, the **program counter(PC)**(sometimes called the **instruction pointer or IP**) tell us which instruction of the program is currently being executed; similarly a **stack pointer** and associated **frame pointer** are used to manage the stack for function parameters, local variables, and return address.

Finally, programs often access persistent storage devices too. Such **I/O information** might include a list of the files the process currently has open.

### 4.3 Process Creation
The first thing that the OS must do to run a program is to **load** its code and any static data(e.g., initialized variables) into memory, into the address space of the process.

Some memroy must be allocated for the program's **run-time stack**(or just **stack**). C programs use the stack for local variables, function parameters, and return address; the OS allocates this memory and gives it to the process.

The OS will also do some other initialization tasks, particularly as related to input/output (I/O). For example, in UNIX systems, each process by default has three open file descriptors, for standard input, output, and error; these descriptors let programs easily read input from the terminal and print output to the screen.

By loading the code and static data into memory, by creating and initializing a stack, and by doing other work as related to I/O setup, the OS has now (finally) set the stage for program execution. It thus has one last task: to start the program running at the entry point, namely main(). By jumping to the main() routine (through a specialized mechanism that we will discuss next chapter), the OS transfers control of the CPU to the newly-created process, and thus the program begins its execution.

### 4.4 Process State
A process can be on one of three