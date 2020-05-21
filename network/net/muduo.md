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

如果数据与其他数据相关联，使用时判断其状态合法性。
```c
    int fd = channel->fd();
    ChannelMap::const_iterator it = channels_.find(fd);
    assert(it != channels_.end());
    assert(it->second == channel);
```
## net
### channel
每个 channel 与一个 fd 相关联，channel 负责处理 4 种事件：可读、可写、关闭文件描述符、错误事件。channel 事件状态变更由 EeventLoop 控制，所以每个 channel 都与一个 EventLoop 相关联。

### Poller
Poller 是抽象基类，将 EPollPoller 和 PollPoller 的接口抽象出来。
```c
virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
virtual void updateChannel(Channel* channel) = 0;
virtual void removeChannel(Channel* channel) = 0;
```

### EventLoop
EventLoop 通过组合 Poller，完成事件循环。EventLoop 循环地从 Poller 获取事件，并调用触发事件的回调函数。
## base