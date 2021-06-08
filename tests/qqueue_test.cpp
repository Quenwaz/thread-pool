#include "gtest/gtest.h"
#define QueueValueType int
#include "qqueue.h"
#undef QueueValueType



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
    EXPECT_EQ(queue_pop(queue), 1);
    EXPECT_EQ(queue_pop(queue), 0);
    queue_free(&queue);
    ASSERT_TRUE(queue == NULL);
}

TEST(qqueue_test, test3)
{
    struct Queue* queue = queue_new();
    struct QueueNode* node = queue_push(queue, 1);
    node = queue_push(queue, 2);
    node = queue_push(queue, 3);
    node = queue_push(queue, 4);
    node = queue_push(queue, 5);
    EXPECT_EQ(queue_pop(queue), 1);
    EXPECT_EQ(queue_pop(queue), 2);
    EXPECT_EQ(queue_pop(queue), 3);
    EXPECT_EQ(queue_pop(queue), 4);
    EXPECT_EQ(queue_pop(queue), 5);
    EXPECT_EQ(queue_pop(queue), 0);
    EXPECT_EQ(queue_empty(queue), 1);

    queue_free(&queue);
    ASSERT_TRUE(queue == NULL);
}
