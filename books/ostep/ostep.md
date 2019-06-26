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

## Chapter 4 Virtualization
