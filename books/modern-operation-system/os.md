# Modern Operating Systems
### 2.2.4 Implementing Threads in User Space
线程的实现两个主要的空间为：用户空间和内核，或者混合实现。

线程在 run-time system 上运行。run-time system 是管理线程的子程序的集合。比如，pthread\_create,
pthread\_exit, pthread\_join and pthread\_yield.
当在用户空间管理线程时，每个进程需要有它私有的线程表(thread table)，用于管理该进程下的线程。
比如每个线程的 program counter, stack pointer, registers, state...。run-time system管理线程表



在用户空间实现线程的优点：
1，可以在不支持线程的操作系统上实现线程，比如通过 library 实现。
2, 线程调度非常快，因为不需要 trap into the kernel，不需要切换上下文，内存缓存不需要刷新。

在用户空间实现线程的优点：
1, 当一个线程调用系统调用时 blocking，会导致所有线程 blocking。所以要额外实现针对线程的系统调用，使之当一个线程 blocking 时，不会影响到其他线程。但这并不容易实现。
2, 在进程内，由于没有时钟中断(clock interrupt)，所以无法通过 round-robin 的方式去调度线程。当前线程一直运行，除非自愿进入 run-time system，否则别的线程只能等待。

在内核实现线程的优点：
1， 无需额外实现针对线程的系统调用，当一个线程 blocking 时，内核可以选择一个 ready 的线程。但是在用户空间，线程对内核是不可见的，在 CPU 时间里完全交给进程控制。


在内核实现线程的缺点：
1, 开销大，切换线程时，需要切换上下文。

### 2.3.2 Critical Regions
一个好的解决 critical regions 的方法需要有一下要求：
1， 不能存在两个进程同时出于他们的 critical regions
2， 不能对速度和cpu时间作假设
3， 进程不处于它的 critical regions时，不会被其他进程 block
4， 进程不需要永远等待进入 critical regions

### 2.3.3 Mutaul Exclusion with Busy Waiting
#### Disabling Interrupts
进程进入 critical region 时关闭所有中断，会使得 crutical exclusion。但也会导致该进程一直占用CPU，直到它恢复中断。关闭中断通常对于操作系统很有作用，但很少用在用户空间。

#### Strict Alternation
```c
a:
while(TRUE)
{
    while(turn != 0)；// loop
    critical_region();
    turn = 1;
    noncritical_region();
}

b:
while(TRUE)
{
    while(turn != 1)；// loop
    critical_region();
    turn = 0;
    noncritical_region();
}
``````
  整数 turn 初始化为0。首先线程 a/b 发现 turn 为0。线程 a 进入临界区，而线程 b 则循环等待，直到 turn 为 1。持续地检查一个变量，直到一个值出现被称为 **busy waiting**。这种情况应该尽量避免，因为它浪费了 CPU 时间。只有当一个短暂的合适的等待才会使用 **busy waiting**。使用 **busy waiting**的锁称为 **spin lock**。
 当线程 a 离开它的临界区，将 turn 置为 1，此时线程 b 得以进入它的临界区。假如线程 b 很快就离开它的临界区，并将 turn 置为0，此时线程 a/b 都处于它们的非临界区。假如线程 a 的非临界区很短，它很快又开始一轮循环，并将 turn 置为1。假设线程 b 非临界区很长，此时线程 a 又开始一轮循环，此时 turn 为1，所以线程 a 会陷入 **busy waiting**，必须等待线程 b 进入新的循环后将 turn 置为0.这种方式需要严格交替，不适用于当一个线程比另一个线程慢很多。并且这种方式违反了一个好的解决 critical regions 的方法的条件 3.

#### Peterson’s Solution
```c
#define FALSE 0
#define TRUE 1
#define N 2 /* number of processes */
int turn; /* whose turn is it? */
int interested[N]; /* all values initially 0 (FALSE) */
void enter_region(int process); /* process is 0 or 1 */
{
    int other; /* number of the other process */
    other = 1 - process; /* the opposite of process */
    interested[process] = TRUE; /* show that you are interested */
    turn = process; /* set flag */
    while (turn == process && interested[other] == TRUE) /* null statement */ ;
}
void leave_region(int process) /* process: who is leaving */
{
    interested[process] = FALSE; /* indicate departure from critical region */
}
```
这是一个纯软件实现的解决方法。

#### The TSL Instruction
现在许多计算机，尤其是有多处理器的计算机，通常会提供一个指令，如 **TSL**(Test and Set Lock):
TSL RX, LOCK 
这个指令，读取 lock 的内容到寄存器 RX 中，然后设置一个非零值在内存地址为 lock 上。这个指令保证原子性，期间其他处理器不可以访问这个地址，直到指令完成。CPU 执行 **TSL** 指令锁住内存总线防止其他 CPU 访问，直到指令完成为止。
注意！**TSL** 指令锁住内存总线和关闭中断有所区别。处理器 1 关闭中断后不能阻止处理器 2 去读写该内存。
为了使用 **TSL** 指令，需要借助一个共享变量 lock。当 lock 为 0 时，任何处理器都可以使用 **TSL** 将其置为 1，然后读写共享内存。当它处理完以后，处理器通过一个普通的 **move** 指令将其还原为 0。
如下方法可以阻止两个进程同时进入临界区。第一个指令复制 lock 的值到 REGISTER，并将 lock 设为 1.第二个指令将 REGISTER 与 0 对比，如果不相等，则说明 lock 已经被设置了，所以程序要跳转到最开始再测试一遍。当进程离开临界区时，则释放 lock 就行了，只需要普通的指令 **move** 将其设为 0。为了阻止两个进程同时进入临界区，处理器只需要在进入临界区之前调用 enter_region，在离开临界区时调用 leave_region 即可。这个方法需要处理器用正确的方式调用，如果有一个进程不正确调用，这个方法就会失效。
```c
enter_region:
TSL REGISTER,LOCK | copy lock to register and set lock to 1
CMP REGISTER,#0 | was lock zero?
JNE enter_region | if it was not zero, lock was set, so loop
RET | return to caller; critical region entered
leave_region:
MOVE LOCK,#0 | store a 0 in lock
RET | retur n to caller
```

另一个可以代替 **TSL** 指令是 **XCHG**，这个指令交换两个位置的内容，比如寄存器和内存。这个指令使用方法本质上和 **TSL** 相同。All Intel x86 CPUs use XCHG instruction for low-level synchronization。
```c
enter_region:
MOVE REGISTER,#1 | put a 1 in the register
XCHG REGISTER,LOCK | swap the contents of the register and lock variable
CMP REGISTER,#0 | was lock zero?
JNE enter region | if it was non zero, lock was set, so loop
RET | return to caller; critical region entered
leave_region:
MOVE LOCK,#0 | store a 0 in lock
RET | retur n to caller
```

### 2.3.4 Sleep and Wakeup
使用 Peterson’s Solution 和指令 **TSL**， **XCHG**都可以解决临界区的问题。但是这些解决办法都有个缺陷，就是需要 **busy waiting**。本质上来说，这些方法都做了这些工作：当进程想进入临界区时，它先是否能进入临界区，如果不行就循环等待，直到可以为止。
这些方法不仅浪费 CPU 时间，有时候还会导致意想不到的影响。假如 CPU 中有两个进程，H 具有高优先级，L 具有低优先级。调度规则是当 H 可以调用时永远优先调用 H。当某个时刻，L 在它的临界区内，此时 H 可以被调用(I/O操作完成)。H 开始  **busy waiting**，但是 L 永远不会被调度，因为 H 的优先级高，所以 L 没有机会离开临界区，H 则无限循环。这种情况有时候被称为 **priority inversion problem**

#### The Producer-Consumer Problem
生产者-消费者问题，又称为有界缓冲区问题。两个进程共享一个固定大小的缓冲区。其中一个是生产者，负责填充缓冲区，另一个是消费者，负责消耗缓冲区。问题出现在，当生产者想将新的 item 放进缓冲区时，发现缓冲区满了，于是生产者进入睡眠状态，直到消费者取出 item 并唤醒它。同样地，当消费者想取出 item 时，却发现缓冲区为空，于是消费者进入睡眠状态，直到生产者放入 item 并唤醒它。这会导致竞争。
使用 count 记录缓冲区有多少 item。如果缓冲区最大 item 数为 N，生产者首先检查是否 count 为 N，如果是，则进入睡眠，否则放入 item 并增加 count。
消费者同样地，首先检查是否 count 为 0，如果是，则进入睡眠，否则取出 item 并减少 count。生产者和消费者都检查是否可以唤醒对方，如果可以则唤醒对方。
当缓冲区为空时，消费者读取 count 后刚判断它是否为 0，调度器选择运行生产者。生产者放入一个 item 后发现此时 count 为 1后发送唤醒信号。而消费者此时判断 count 为 0，准备进入睡眠状态。问题出现在当唤醒信号发出时，处理器运行的不是消费者，所以这个信号相当于丢失了。当调度器调度消费者时，它判断 count 为 0，进入睡眠状态。于是调度器调度生产者，生产者不断放入 item，直至缓冲区满了，它也进入睡眠状态。此时生产者、消费者都处于睡眠状态。
一个快速解决的办法是引入 **wakeup waiting bit**，当一个唤醒信号发送给一个未睡眠的进程时，wakeup waiting bit 被设置。晚一点，当这个进程准备睡眠时，发现 wakeup waiting bit 被设置了，则取消 wakeup waiting bit，但该进程不进入睡眠了。但当我们有多个进程时，就要引入多个 **wakeup waiting bit**，本质上来说问题还是存在的。
```c
#define N 100 /* number of slots in the buffer */
int count = 0; /* number of items in the buffer */
void producer(void)
{
    int item;
    while (TRUE) { /* repeat forever */
        item = produce_item( ); /* generate next item */
        if (count == N) sleep( ); /* if buffer is full, go to sleep */
        insert_item(item); /* put item in buffer */
        count = count + 1; /* increment count of items in buffer */
        if (count == 1) wakeup(consumer); /* was buffer empty? */
    }
}
void consumer(void)
{
    int item;
    while (TRUE) { /* repeat forever */
        if (count == 0) sleep( ); /* if buffer is empty, got to sleep */
        item = remove_item( ); /* take item out of buffer */
        count = count ? 1; /* decrement count of items in buffer */
        if (count == N ? 1) wakeup(producer); /* was buffer full? */
        consume_item(item); /* pr int item */
    }
}
```

### 2.3.5 Semaphores
采用一种新的变量类型 **semaphore** 去记录唤醒信号，如果值为 0 则表示没有信号被记录，如果为负数则表示有一个或多个信号被记录。通常有两种操作作用于 **semaphore**,分别是 **up** 和 **down** 操作。**down** 操作作用于 **semaphore**，如果值大于 0，它则减少值，然后继续(即不睡眠)。如果值为 0 则进程进入睡眠，但 **down**操作此时还不算全部完成，检查值，改变值，睡眠这些操作都是原子操作。
**up** 操作增加 **semaphore** 的值。如果有一个或多个进程因为 **semaphore** 而出于睡眠状态，系统则从中选出一个，使其完成之前未完成的 **down** 操作。所以在 **up** 操作于一个有进程睡眠的 **semaphore** 后，该值仍为 0，但睡眠的进程减少了。

#### Solving the Producer-Consumer Problem Using Semaphores
通过使用 **semaphore** 可以解决上面唤醒信号丢失的问题。**up** 和 **down** 操作通常实现为系统调用，当检查值、改变值、使进程休眠时操作系统只需要简单的关闭所有中断，因为这些操作都是使用很少的指令就可以完成，不会造成什么影响。如果在多核的情况下，就需要借助 **TSL** 和 **XCHG** 指令去保证同一时间只有一个 CPU 能访问 **semaphore**。

```c
#define N 100 /* number of slots in the buffer */
typedef int semaphore; /* semaphores are a special kind of int */
semaphore mutex = 1; /* controls access to critical region */
semaphore empty = N; /* counts empty buffer slots */
semaphore full = 0; /* counts full buffer slots */
void producer(void)
{
    int item;
    while (TRUE) { /* TRUE is the constant 1 */
        item = produce_item( ); /* generate something to put in buffer */
        down(&empty); /* decrement empty count */
        down(&mutex); /* enter critical region */
        insert_item(item); /* put new item in buffer */
        up(&mutex); /* leave critical region */
        up(&full); /* increment count of full slots */
    }
}
void consumer(void)
{
    int item;
    while (TRUE) { /* infinite loop */
        down(&full); /* decrement full count */
        down(&mutex); /* enter critical region */
        item = remove_item( ); /* take item from buffer */
        up(&mutex); /* leave critical region */
        up(&empty); /* increment count of empty slots */
        consume_item(item); /* do something with the item */
    }
}
```
full 记录有多少槽有 item。empty 记录有多少空的槽。使用 mutex 确保生产者、消费者不会同一时间访问槽。**semaphore** 初始化为 1 用来保证多个进程在某个时刻只能有一个进程进入临界区，这称为 **binary semaphore**。

### 2.3.6 Mutexes
**mutex** 是一种共享变量，它只能处于两种状态：unlocked or locked。当值为 0 时为 unlocked，其他值时为 locked。由于 **mutex** 比较简单，它可以在提供 **TSL** 指令的用户空间实现。
```c
mutex_lock:
    TSL REGISTER,MUTEX | copy mutex to register and set mutex to 1
    CMP REGISTER,#0 | was mutex zero?
    JZE ok | if it was zero, mutex was unlocked, so return
    CALL thread_yield | mutex is busy; schedule another thread
    JMP mutex_lock | tr y again
    ok: RET | return to caller; critical region entered
mutex_unlock:
    MOVE MUTEX,#0 | store a 0 in mutex
    RET | return to caller
```
mutex\_lock 和 enter\_region的实现有个关键的地方不同。enter\_region 会进入 **busy waiting**，直到时间片用完或者其他进程被调度。但在用户空间又有些不同，因为线程没有 clock 会导致它停止。这会导致线程一直在 **busy waiting** 而又拿不到锁，因为其他线程没有机会运行并释放锁。
mutex\_lock 不同于 enter\_region 是当 mutex\_lock  请求锁失败后，它会调用 thread\_yield 放弃 CPU 给其他线程。

#### Futexes
随着越来越多的并发编程，同步和锁的效率对于性能十分重要。**spin lock** 如果等待时间短就快，反之则会浪费 CPU 时间。如果存在很多竞争，通过 block 进程和在 lock 释放时通知内核 unblock 进程会提高效率。这需要频繁的切换到内核，当竞争激烈时它很有效，但竞争不激烈时切换到内核的代价就很昂贵了。
一个解决的办法是使用 **futex(fast user space mutex)**。**futex** 是 linux 实现的一种基础锁（像 mutex）除非很必要否则避免切换到内核。**futex** 由两部分组成：kernel service and user library。kernel sevice 提供一个 "wait queue" 给进程等待一个 lock。这些进程不会运行，除非 kernel unblocked 进程。应该避免将一个进程放入 "wait queue"，因为这需要调用一个系统调用（切换到内核开销大）。所以在**竞争不激烈**的情况下，**futex** 完全运行在用户空间。特别地，进程之间共享一个 32-bit 整形

### 2.3.7 Monitors
