#include "gtest/gtest.h"
#include "qqueue.h"



TEST(qqueue_test, test1)
{

    struct Queue* queue = queue_new();

    struct QueueNode* node = queue_push(queue, 1);
    node = queue_push(queue, 2);
    ASSERT_EQ(queue_top(queue), 1);
    node = queue_push(queue, 3);
    ASSERT_EQ(queue_top(queue), 1);
    node = queue_push(queue, 4);
    ASSERT_EQ(queue_top(queue), 1);
    node = queue_push(queue, 5);
    ASSERT_EQ(queue_top(queue), 1);

    EXPECT_EQ(queue_size(queue), 5) << "队列值计算错误";
    queue_free(&queue);
    
    ASSERT_TRUE(queue == NULL);
}

TEST(qqueue_test, test2)
{
    struct Queue* queue = queue_new();
    struct QueueNode* node = queue_push(queue, 1);
    node = queue_push(queue, 2);
    node = queue_push(queue, 3);
    node = queue_push(queue, 4);
    node = queue_push(queue, 5);
    ASSERT_EQ(queue_pop(queue), 1);
    ASSERT_EQ(queue_pop(queue), 2);
    ASSERT_EQ(queue_pop(queue), 3);
    ASSERT_EQ(queue_pop(queue), 4);
    ASSERT_EQ(queue_pop(queue), 5);
    ASSERT_EQ(queue_pop(queue), 0);
}
