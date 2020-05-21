# muduo 库笔记

## net
### channel
每个 channel 与一个 fd 相关联，channel 负责处理 4 种事件：可读、可写、关闭文件描述符、错误事件。channel 事件状态变更由 EeventLoop 控制，所以每个 channel 都与一个 EventLoop 相关联。
## base