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
cond_.wait()，首先
