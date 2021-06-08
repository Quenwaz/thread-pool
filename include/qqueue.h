/**
 * @file queue.h
 * @author Quenwaz (404937333@qq.com)
 * @brief 队列实现
 * @version 0.1
 * @date 2021-06-06
 * 修改 QueueValueType 需重新编译源文件
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef _h_queue_included__
#define _h_queue_included__
#include <sys/types.h>

#ifdef __cplusplus
extern "C"{
#endif

// typedef int QueueValueType;
#ifndef QueueValueType
// #pragma message("Please define the value type of the queue.")
// #error "Please define the value type of the queue."
#define QueueValueType void*
#endif // QueueValueType

/**
 * @brief 队列链表节点
 * 
 */
struct QueueNode{
    QueueValueType value;
    struct QueueNode* next;
};

/**
 * @brief 队列对象
 * 
 */
struct Queue{
    struct QueueNode* front;
    struct QueueNode* rear;
};

/**
 * @brief 构建一个队列
 * 
 * @return struct Queue*  返回队列指针
 */
struct Queue* queue_new();

/**
 * @brief 入队
 * 
 * @param queue 队列根节点
 * @param val 值
 * @return struct QueueNode* 返回队尾指针
 */
struct QueueNode * queue_push(struct Queue * queue,  QueueValueType val);

/**
 * @brief 出队
 * 
 * @param queue 队列根节点
 * @return QueueValueType 返回被删除的节点 
 */
QueueValueType queue_pop(struct Queue * queue);

/**
 * @brief 获取队头值
 * 
 * @param queue  队列根节点
 * @return QueueValueType 
 */
QueueValueType queue_top(struct Queue * queue);

/**
 * @brief 释放队列
 * 
 * @param queue 队列根节点
 */
void queue_free(struct Queue ** queue);

/**
 * @brief 队列是否为空
 * 
 * @param queue 队列根节点
 * @return int8_t 0非空， 否则为空
 */
int8_t queue_empty(struct Queue * queue);

/**
 * @brief 获取队列长度
 * 
 * @param queue 队列跟节点
 * @return uint32_t 长度值
 */
unsigned int queue_size(struct Queue * queue);



#ifdef __cplusplus
}
#endif
#endif // _h_queue_included__