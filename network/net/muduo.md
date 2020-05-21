# muduo 库笔记

## 技巧总结
### 不可复制类
当一个类不可复制时，一般传递它的指针，以获取这个类。如 channel 类。
### assert 的使用
对传入参数 assert 其合法性。
```c
void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
  assert(implicit_cast<size_t>(numEvents) <= events_.size());
}
```

对
## net
### channel
每个 channel 与一个 fd 相关联，channel 负责处理 4 种事件：可读、可写、关闭文件描述符、错误事件。channel 事件状态变更由 EeventLoop 控制，所以每个 channel 都与一个 EventLoop 相关联。

### EpollPoller
## base