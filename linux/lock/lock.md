### unique_lock 和 condition_variable 的使用
```c
bool flag = fasle;
mutex mutex_;
condition_variable cond_;

void threadFunc()
{
  unique_lock<mutex> lk(mutex_); // 申请锁
  while(!flag)
    cond_.notify_all();
}

int main()
{
  thead thrd(threadFunc);
  unique_lock
}

```
