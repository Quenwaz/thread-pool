#include "threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

struct Task{
    FnTask fnTask_;
    void* pData;
};

#define QueueValueType struct Task
#include "qqueue.h"
#undef QueueValueType

struct ThreadPool{
    uint8_t stopall_;
    pthread_t * tpool_;
    uint32_t tpool_szie_;
    struct Queue* taskqueue_;
    pthread_mutex_t mtx_;
    pthread_cond_t cond_; //559
};


static void* threadfunc(void * pData)
{
    struct ThreadPool* threadpool = (struct ThreadPool*)pData;
    for(;;)
    {
        struct Task task;
        pthread_mutex_lock(&threadpool->mtx_);
        while (0 != queue_empty(threadpool->taskqueue_) && threadpool->stopall_ == 0)
        {
            // 返回非0处理
            pthread_cond_wait(&threadpool->cond_, &threadpool->mtx_);
        }
        if (threadpool->stopall_ != 0){
            break;
        }
        task = queue_pop(threadpool->taskqueue_);
        pthread_mutex_unlock(&threadpool->mtx_);
        task.fnTask_(task.pData);
    }
    return NULL;
}

void* threadpool_init(unsigned int size)
{
    if(size == 0){
        return NULL;
    }

    struct ThreadPool* threadpool = (struct ThreadPool*)malloc(sizeof(struct ThreadPool));
    threadpool->stopall_ = 0;
    threadpool->tpool_szie_ = size;
    threadpool->tpool_ = (pthread_t*)malloc(sizeof(pthread_t) * size);
    threadpool->taskqueue_ = queue_new();
    pthread_mutex_init(&threadpool->mtx_, NULL);
    pthread_cond_init(&threadpool->cond_, NULL);
    for (uint32_t i = 0; i< threadpool->tpool_szie_; ++i)
    {
        pthread_create(&threadpool->tpool_[i], NULL, threadfunc, threadpool);
    }
    return threadpool;
}


void threadpool_push_task(void* threadpool_, FnTask fntask, void* pData)
{
    struct ThreadPool* threadpool = (struct ThreadPool*)threadpool_;
    pthread_mutex_lock(&threadpool->mtx_);
    struct Task task;
    task.fnTask_ = fntask;
    task.pData = pData;
    queue_push(threadpool->taskqueue_, task);
    // 通知线程有新任务来了
    pthread_mutex_unlock(&threadpool->mtx_);
    pthread_cond_signal(&threadpool->cond_);
}

#include <stdio.h>
void threadpool_free(void* threadpool_)
{
    struct ThreadPool* threadpool = (struct ThreadPool*)threadpool_;
    pthread_mutex_lock(&threadpool->mtx_);
    threadpool->stopall_ = 1;
    pthread_mutex_unlock(&threadpool->mtx_);
    pthread_cond_broadcast(&threadpool->cond_); 
    for(uint32_t i = 0;i < threadpool->tpool_szie_; ++i){
        pthread_join(threadpool->tpool_[i], NULL);
        fprintf(stderr, "exit\n");
        pthread_cond_broadcast(&threadpool->cond_); 
    }
    free(threadpool->tpool_);
    queue_free(&threadpool->taskqueue_);
    pthread_mutex_destroy(&threadpool->mtx_);
    pthread_cond_destroy(&threadpool->cond_);
}
