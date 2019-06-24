# Operating Systems in Three Easy Pieces 读书笔记

## Chapter 2 Introduction to Operating Systems
##### What did the processor do?
The processor **fetches** an instruction from memory, **decodes** it, and **executes** it.

### 2.5 Design goals
Design goals of OS:
- **Virtualization**. OS takes physical resources, such as a CPU, memory, or disk, and **virtualizes** them.
- **Concurrency**. OS handles tough and tricky issues related to **concurrency**.
- **Persistence**. OS stores files persistently.
- High performance. 
- Protection. OS provides **prot