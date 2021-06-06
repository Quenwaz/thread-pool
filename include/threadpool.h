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

/**
 * @brief 任务函数指针定义
 * 
 */
typedef int(*FnTask)(void*);

/**
 * @brief 初始化线程池大小
 * 
 * @param size 线程池大小， 建议2^n大小
 */
void init_threadpool(unsigned int size);

/**
 * @brief 添加任务线程池的任务队列中
 * 
 * @param task 任务函数地址
 * @param pData 附加数据
 */
void push_task(FnTask task, void* pData);

#endif // __h_threadpoll_included__
