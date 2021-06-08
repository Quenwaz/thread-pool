/**
 * @file threadpool.h
 * @author your name (you@domain.com)
 * @brief C语言版线程池接口声明
 * @version 0.1
 * @date 2021-06-06
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */

#ifndef __h_threadpoll_included__
#define __h_threadpoll_included__
#include <stdbool.h>
#ifdef __cplusplus
extern "C"{
#endif


/**
 * @brief 任务函数指针定义
 * 
 */
typedef void(*FnTask)(void*);

/**
 * @brief 初始化线程池大小
 * 
 * @param size 线程池大小， 建议2^n大小
 * @return void* 返回线程池对象
 */
void* threadpool_init(unsigned int size);

/**
 * @brief 添加任务线程池的任务队列中
 * 
 * @param task 任务函数地址
 * @param pData 附加数据
 */
void threadpool_push_task(void* threadpool, FnTask task, void* pData);

/**
 * @brief 释放线程池， 关闭所有线程
 * 
 * @param threadpool 线程池对象
 * @param wait 非0则等待所有工作线程退出(join)， 0则不等待(detach)
 */
void threadpool_free(void* threadpool, bool wait);



#ifdef __cplusplus
}
#endif
#endif // __h_threadpoll_included__
