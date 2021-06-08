#include "qqueue.h"
#include <stdlib.h>


struct Queue* queue_new()
{
    struct Queue* queue =  (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

struct QueueNode * queue_push(struct Queue * queue,  QueueValueType val)
{
    if(queue == NULL){
        return NULL;
    }

    struct QueueNode* node = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    node->value = val;
    node->next = NULL;
    if ( queue->front == NULL){
        queue->front = node;
    }else{
        queue->rear->next = node;
    }
    queue->rear= node;
    return node;
}

QueueValueType queue_pop(struct Queue * queue)
{
    if (queue->front == NULL){
        return 0;
    }

    struct QueueNode* front = queue->front;
    queue->front = front->next;
    QueueValueType ret = front->value;
    free(front);
    return ret;
}

QueueValueType queue_top(struct Queue * queue)
{
    return queue->front->value;
}

void queue_free(struct Queue ** queue)
{
    if (queue == NULL ||  (*queue) == NULL){
        return;
    }

    struct QueueNode * root = (*queue)->front;
    while (root != NULL)
    {
        void* wiilbefree = root;
        root= root->next;
        free(wiilbefree);
    }
    free((*queue));
    (*queue) = NULL;
}

int8_t queue_empty(struct Queue * node){
    return node->front == NULL;
}

unsigned int queue_size(struct Queue * queue)
{
    if (queue == NULL){
        return 0;
    }
    unsigned int length = 0;
    struct QueueNode * node = queue->front;
    while(node != NULL){
        ++length;
        node = node->next;
    }

    return length;
}

