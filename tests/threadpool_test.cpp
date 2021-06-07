#include "gtest/gtest.h"
#include "threadpool.h"

int task1(void* data)
{
    printf(__FUNCTION__);
    return 0;
}

int task2(void* data)
{
    printf(__FUNCTION__);
    return 0;
}

int task3(void* data)
{
    printf(__FUNCTION__);
    return 0;
}

TEST(threadpool_test, test1)
{

    void* threadpool = threadpool_init(4);

    threadpool_push_task(threadpool,task1, NULL);
    threadpool_push_task(threadpool,task2, NULL);
    threadpool_push_task(threadpool,task3, NULL);

    threadpool_free(threadpool);
    ASSERT_EQ(1, 1);
}