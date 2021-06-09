#include "gtest/gtest.h"
#include "threadpool.h"
#include "threadpool.hpp"

void task1(void* data)
{
    int *p = (int*)(data);
    sleep(*p);
    fprintf(stderr, "do %s...already sleep %d second.\n",__FUNCTION__, *p);
    *p = 1;
}

void task2(void* data)
{
    int *p = (int*)(data);
    sleep(*p);
    fprintf(stderr, "do %s...already sleep %d second.\n",__FUNCTION__, *p);
    *p = 1;
}

void task3(void* data)
{
    int *p = (int*)(data);
    sleep(*p);
    fprintf(stderr, "do %s...already sleep %d second.\n",__FUNCTION__, *p);
    *p = 1;
}


TEST(threadpool_test, test_c_1)
{
    void* threadpool = threadpool_init(4);
    int sleep_second_3 = 3;
    threadpool_push_task(threadpool,task1, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool_push_task(threadpool,task2, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool_push_task(threadpool,task3, &sleep_second_2);

    // 必须等待所有线程结束， 否则无法正常验证
    threadpool_free(threadpool, true);
    EXPECT_EQ(sleep_second_1, 1);
    EXPECT_EQ(sleep_second_2, 1);
    EXPECT_EQ(sleep_second_3, 1);
}

TEST(threadpool_test, test_c_2)
{
    void* threadpool = threadpool_init(2);
    int sleep_second_3 = 3;
    threadpool_push_task(threadpool,task1, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool_push_task(threadpool,task2, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool_push_task(threadpool,task3, &sleep_second_2);

    // 必须等待所有线程结束， 否则无法正常验证
    threadpool_free(threadpool, true);
    EXPECT_EQ(sleep_second_1, 1);
    EXPECT_EQ(sleep_second_2, 1);
    EXPECT_EQ(sleep_second_3, 1);
}

TEST(threadpool_test, test_cpp_1)
{
    threadpool::ThreadPool threadpool(4);
    int sleep_second_3 = 3;
    threadpool.PushTask(task1, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool.PushTask(task2, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool.PushTask(task3, &sleep_second_2);

    // 必须等待所有线程结束， 否则无法正常验证
    threadpool.StopAll(true);
    EXPECT_EQ(sleep_second_1, 1);
    EXPECT_EQ(sleep_second_2, 1);
    EXPECT_EQ(sleep_second_3, 1);
}



TEST(threadpool_test, test_cpp_2)
{
    threadpool::ThreadPool threadpool(2);
    int sleep_second_3 = 3;
    threadpool.PushTask(task1, &sleep_second_3);
    int sleep_second_1 = 1;
    threadpool.PushTask(task2, &sleep_second_1);
    int sleep_second_2 = 2;
    threadpool.PushTask(task3, &sleep_second_2);

    // 必须等待所有线程结束， 否则无法正常验证
    threadpool.StopAll(true);
    EXPECT_EQ(sleep_second_1, 1);
    EXPECT_EQ(sleep_second_2, 1);
    EXPECT_EQ(sleep_second_3, 1);
}