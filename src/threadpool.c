#include "threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include "qqueue.h"

#ifdef DEBUG
#include <stdio.h>
#include <time.h>
#define __LOG(level, format, ...) \
{\
    time_t rawtime;\
    struct tm * ltime;\
    time ( &rawtime );\
    ltime = localtime (&rawtime );\
    char szDatetime[32] = {0};\
	strftime(szDatetime, sizeof szDatetime, "%Y-%m-%d %H:%M:%S", ltime);\
    fprintf(stderr, "[%s] ["#level"] [%s:(%d)] "format"\n", szDatetime, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}
#define LOGI(format, ...) __LOG(Info, format)
#define LOGW(format, ...) __LOG(Warn, format)
#define LOGE(format, ...) __LOG(Error, format)
#endif // DEBUG


struct Task{
    FnTask fnTask_;
    void* pData;
};


/**
 * @brief 线程池对象
 * 记录线程池相关资源
 */
struct ThreadPool{
    /// 停止工作标志
    uint8_t stopall_;

    // 工作线程id列表
    pthread_t * tpool_;

    // 工作线程个数
    uint32_t tpool_szie_;

    // 任务队列
    struct Queue* taskqueue_;

    // 互斥量
    pthread_mutex_t mtx_;

    // 条件变量
    pthread_cond_t cond_; 
};

static void* threadfunc(void * pData)
{
    struct ThreadPool* threadpool = (struct ThreadPool*)pData;
    for(;;)
    {
        struct Task task;
        task.fnTask_ = NULL;
        task.pData = NULL;
        pthread_mutex_lock(&threadpool->mtx_);
        while (0 != queue_empty(threadpool->taskqueue_) && threadpool->stopall_ == 0)
        {
            // 任务队列为空且未停止线程池， 则等待
            // 返回非0处理
            pthread_cond_wait(&threadpool->cond_, &threadpool->mtx_);
        }

        if (queue_empty(threadpool->taskqueue_) == 0)
        {
            struct Task* __p_task = queue_pop(threadpool->taskqueue_);
            task.fnTask_ = __p_task->fnTask_;
            task.pData = __p_task->pData;
            // 释放， 由于线程池对象释放时， 会等待任务队列置空。所以可保证内存不泄露
            free(__p_task);
        }
        pthread_mutex_unlock(&threadpool->mtx_);

        // 通知threadpool_free, 任务出队了
        pthread_cond_signal(&threadpool->cond_);

        // 任务队列为空且停止所有线程， 则退出当前线程
        if ( task.fnTask_ == NULL)
            break;
             
        // 执行任务
        task.fnTask_(task.pData);
    }

    return pData;
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
    if (threadpool_ == NULL){
        return;
    }

    struct ThreadPool* threadpool = (struct ThreadPool*)threadpool_;
    pthread_mutex_lock(&threadpool->mtx_);
    if (threadpool->stopall_){
        return;
    }
    struct Task* task =(struct Task*)malloc(sizeof(struct Task));
    task->fnTask_ = fntask;
    task->pData = pData;
    queue_push(threadpool->taskqueue_, task);
    // 通知线程有新任务来了
    pthread_mutex_unlock(&threadpool->mtx_);
    pthread_cond_signal(&threadpool->cond_);
}

void threadpool_free(void* threadpool_, bool wait)
{
    if (threadpool_ == NULL){
        return;
    }

    struct ThreadPool* threadpool = (struct ThreadPool*)threadpool_;
    pthread_mutex_lock(&threadpool->mtx_);

    // 告知所有线程，该退出了。 但是无法终止正工作中的线程， 强行终止可能导致内存泄漏
    threadpool->stopall_ = 1;

    // 等待所有任务出队之后， 再将stopall标志位置1
    while (0 == queue_empty(threadpool->taskqueue_))
    {
        pthread_cond_wait(&threadpool->cond_, &threadpool->mtx_);
    }
    pthread_mutex_unlock(&threadpool->mtx_);

    // 广告所有等待线程， 无需再等待工作
    pthread_cond_broadcast(&threadpool->cond_); 
    for(uint32_t i = 0;i < threadpool->tpool_szie_; ++i){
        if (wait)
            pthread_join(threadpool->tpool_[i], NULL);
        else{
            pthread_detach(threadpool->tpool_[i]);
        }
    }
    // release resource
    free(threadpool->tpool_);
    queue_free(&threadpool->taskqueue_);
    pthread_mutex_destroy(&threadpool->mtx_);
    pthread_cond_destroy(&threadpool->cond_);
}
