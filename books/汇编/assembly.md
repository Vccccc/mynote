# pcasm-book
## Introduction
### 1.2.2 The CPU
时钟脉冲：脉冲信号是一个按一定电压幅度，一定时间间隔连续发出。脉冲信号之间的时间间隔称为周期；而将在单位时间（如1秒）内所产生的脉冲个数称为频率。

频率为1.5GHz的CPU每秒产生1500000000次脉冲，也称每次脉冲为一个周期。

一个指令需要多少周期依赖于CPU的模式。

#### 寄存器
寄存器 | 说明
:----|:----
AX | AX可拆分为AH,AL
BX | BC可拆分为BH,BL
CX | CX可拆分为CH,CL
DX | DX可拆分为DH,DL
SI | 不可拆分
DI | 不可拆分
BP | 
SP |
CS | Code Segment
DS | Data Sege

