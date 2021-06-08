/**
 * @file threadpool.hpp
 * @author your name (you@domain.com)
 * @brief 线程池接口声明Cpp版本
 * @version 0.1
 * @date 2021-06-06
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef _hpp_threadpool_included__
#define _hpp_threadpool_included__
#include <functional>

namespace threadpool{

/// 任务函数指针
typedef std::function<void(void*)> FnTask;

/**
 * @brief 线程池对象
 * 
 */
class ThreadPool{
public:
    /**
     * @brief 构建线程池对象
     * 
     * @param nTh 线程池大小， 建议大小为2^n
     */
    explicit ThreadPool(size_t nTh);

    /**
     * @brief 析构线程池对象
     * 若调用此方法之前未调用StopAll,
     * 将调用StopAll(true)。 阻塞至线程全部退出。
     */
    ~ThreadPool();

    /**
     * @brief 任务入队
     * 
     * @param fnTask 任务地址
     * @param pData 任务附加数据
     * @return 是否入队成功， 通常fnTask为nullptr或线程池关闭时会入队失败
     */
    bool PushTask(FnTask fnTask, void* pData);

    /**
     * @brief 停止所有任务
     *  此调用将会等待任务队列全部出队，同时将无法将任务入队，线程将会继续执行任务。
     *  若bWait为真， 将阻塞至所有线程执行完毕。否则， 将不阻塞调用。 但是线程仍再后台执行。
     * @param bWait 是否等待， true(join), false(detach)
     */
    void StopAll(bool bWait=true);

    struct Impl;
private:
    /// 编译防火墙
    Impl* m_pImpl;
};

}

#endif // _hpp_threadpool_included__