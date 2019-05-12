### unique_lock 和 condition_variable 的使用
```c
bool flag = fasle;
mutex mutex_;
condition_variable cond_;

void threadFunc()
{
  unique_lock<mutex> lk(mutex_); // 申请锁
  while(!flag)
    cond_.wait(); // 等待条件满足
}

int main()
{
  thead thrd(threadFunc);
  {
    unique_lock<mutex> lk(mutex_); // 申请锁
    flag = true;
  }
  cond_.notify_all(); // 唤醒等待线程
  thrd.join();
}
```
cond_.wait()，首先将当前线程置于等待线程列表中(此时已获得锁)，然后解锁。这关闭了在检查条件和让线程休眠
