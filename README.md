# 概述
尝试从0开始书写一个线程池， 一个C语言版， 一个C++11版本。
# 更新列表
- [x] C版队列完成
- [x] C版线程池完成
- [x] C版线程池死锁问题解决
- [x] C++版线程池
# 目录
- [概述](#概述)
- [更新列表](#更新列表)
- [目录](#目录)
- [思路](#思路)
- [实例](#实例)
- [经验教训](#经验教训)

# 思路
基本思路如下:
1. 需要一个队列数据结构
2. 需要一个互斥量对队列的操作进行锁定
3. 需要一个条件变量，通知等待线程任务到达
4. 需要一个终止标志位， 以便线程不再等待任务
5. 终止线程池， 需等待任务队列全部出队。再置位

![此处缺一张逻辑图]()


# 实例

C版线程池实例:

```c
#include "threadpool.h"


void task(void* data)
{
    int *p = (int*)(data);
    sleep(*p);
    fprintf(stderr, "do %s...already sleep %d second.\n",__FUNCTION__, *p);
    *p = 1;
}


int main(int argc, char const *argv[])
{
    void* threadpool = threadpool_init(4);
    int sleep_second_3 = 3;
    threadpool_push_task(threadpool,task, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool_push_task(threadpool,task, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool_push_task(threadpool,task, &sleep_second_2);

    threadpool_free(threadpool, true);
    return 0;
}
```

C++ 版线程池实例：

```c++
#include "threadpool.hpp"

int main(int argc, char const *argv[])
{
    threadpool::ThreadPool threadpool(4);
    int sleep_second_3 = 3;
    threadpool.PushTask(task, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool.PushTask(task, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool.PushTask(task, &sleep_second_2);

    threadpool.StopAll(true);
    return 0;
}
```
# 经验教训
- 死锁问题

    在编写C语言版本的线程池过程中， 出现了死锁。 缘由是因为在循环中的`pthread_mutex_unlock` 与 `pthread_mutex_unlock` 之间使用了`break` 

