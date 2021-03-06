# Operating Systems in Three Easy Pieces Note

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
A process can be in one of three states:
>**Running**: In the running state, a process is running on processor. This means it is executing  instructions.
>**Ready**: In the ready state, a process is ready to run but for some reason the OS has chosen not to run it at this given moment.
>**Blocked**: In the blocked state, a process has performed some kind of operation that makes it not ready to run until some other event takes    place. A common example: when a process initiates an I/O request to a disk, it becomes blocked and thus some other process can use the processor.  

**Figure 4.2: Process: State Transitions**
!["Figure 4.2: Process: State Transitions"](./photo/tmp.png)

### 4.5 Data Structures
The OS has some key data structures that track various relevant pieces of information. To track the state of each process, for example, the OS likely keep some kind of **process list** for all process that are ready and some additional information to track which process is currently running. The OS must also track, in some way, blocked processes; when an I/O event completes, the OS should make sure to wake the correct process and ready it to run again.

The following code shows what type of information an OS needs to track about each process in the xv6 kernel.
```c
// the registers xv6 will save and restore
// to stop and subsequently restart a process
struct context {
	int eip;
	int esp;
	int ebx;
	int ecx;
	int edx;
	int esi;
	int edi;
	int ebp;
};
// the different states a process can be in
enum proc_state {
	UNUSED, EMBRYO, SLEEPING,
	RUNNABLE, RUNNING, ZOMBIE
};
// the information xv6 tracks about each process
// including its register context and state
struct proc {
	char* mem; // Start of process memory
	uint sz; // Size of process memory
	char* kstack; // Bottom of kernel stack
	// for this process
	enum proc_state state; // Process state
	int pid; // Process ID
	struct proc* parent; // Parent process
	void* chan; // If non-zero, sleeping on chan
	int killed; // If non-zero, have been killed
	struct file* ofile[NOFILE]; // Open files
	struct inode* cwd; // Current directory
	struct context context; // Switch here to run process
	struct trapframe* tf; // Trap frame for the
	// current interrupt
};
```
From the code, you can see a couple of important pieces of information the OS tracks about a process. The **register context** will hold, for astopped process, the contents of its registers.When a process is stopped,
its registers will be saved to this memory location; by restoring these registers(i.e., placing their values back into the actual physical registers), the OS can resume running the process. 

>ASIDE: DATA STRUCTURE — THE PROCESS LIST
Operating systems are replete with various important data structures that we will discuss in these notes. The process list (also called the task list) is the first such structure. It is one of the simpler ones, but certainly any OS that has the ability to run multiple programs at once will have something akin to this structure in order to keep track of all the running programs in the system. Sometimes people refer to the individual structure that stores information about a process as a Process Control Block (PCB), a fancy way of talking about a C structure that contains information about each process (also sometimes called a process descriptor).

>ASIDE: KEY PROCESS TERMS
• The process is the major OS abstraction of a running program. At any point in time, the process can be described by its state: the contents of memory in its address space, the contents of CPU registers (including the program counter and stack pointer, among others), and information about I/O (such as open files which can be read or written). 
• The process API consists of calls programs can make related to processes. Typically, this includes creation, destruction, and other useful calls.
• Processes exist in one of many different process states, including running, ready to run, and blocked. Different events (e.g., getting scheduled or descheduled, or waiting for an I/O to complete) transition a process from one of these states to the other.
• A process list contains information about all processes in the system. Each entry is found in what is sometimes called a process control block (PCB), which is really just a structure that contains information about a specific process.
## Virtualization
### Limited Direct Execution
In order to virtualize the CPU, the operating system needs to somehow
share the physical CPU among many jobs running seemingly at the same
time. The basic idea is simple: run one process for a little while, then
run another one, and so forth. By time sharing the CPU in this manner,
virtualization is achieved.

There are a few challenges, however, in building such virtualization
machinery. The first is performance: how can we implement virtualization without adding excessive overhead to the system? The second is
control: how can we run processes efficiently while retaining control over
the CPU? Control is particularly important to the OS, as it is in charge of
resources; without control, a process could simply run forever and take
over the machine, or access information that it should not be allowed to
access. Obtaining high performance while maintaining control is thus
one of the central challenges in building an operating system
## 5 Interlude: Process API
