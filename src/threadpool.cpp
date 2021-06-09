#include "threadpool.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <cassert>


namespace threadpool{

/**
 * @brief 任务对象
 * 
 */
struct Task{
    /// 任务地址
    FnTask fnTask_{nullptr};

    /// 任务附加数据
    void* pData_{nullptr};
};

struct ThreadPool::Impl
{
    /// 停止任务标志
    bool stop_all_{false};

    /// 线程列表
    std::vector<std::thread> threads_;

    /// 任务队列资源锁
    std::mutex mtx_;

    /// 条件变量， 通知任务入队 
    std::condition_variable cv_;

    /// 任务队列
    std::queue<Task> task_queue_;
};


void ThreadFunc(ThreadPool::Impl* pData)
{
    for (;;)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(pData->mtx_);
            // 等待直到有任务 或者 停止任务
            pData->cv_.wait(lock, [&]()->bool{
                return !pData->task_queue_.empty() || pData->stop_all_;
            });

            if (!pData->task_queue_.empty()){
                task = pData->task_queue_.front();
                pData->task_queue_.pop();
            }
        }
        // 通知主线程的等待， 主线程等待任务队列清空
        pData->cv_.notify_one();

        // 仅当staopall为true时，才会发生以下逻辑
        if (task.fnTask_ == nullptr)
            break;
        
        // 执行任务
        task.fnTask_(task.pData_);
        
    }
}

bool CreateThreads(size_t nTh, std::vector<std::thread>& threads, threadpool::ThreadPool::Impl* pData)
{
    if (nTh==0){
        return false;
    }

    for(size_t i = 0;i < nTh; ++i)
    {
        threads.emplace_back(std::thread(ThreadFunc, pData));
    }

    return true;
}

}


threadpool::ThreadPool::ThreadPool(size_t nTh)
    : m_pImpl(new Impl)
{
    assert(CreateThreads(nTh, m_pImpl->threads_, m_pImpl));
}


threadpool::ThreadPool::~ThreadPool()
{
    if (!m_pImpl->stop_all_){
        this->StopAll();
    }

    delete m_pImpl;
    m_pImpl = nullptr;
}


bool threadpool::ThreadPool::PushTask(FnTask fnTask, void* pData)
{
    if (fnTask == NULL || m_pImpl->stop_all_){
        return false;
    }

    // enqueue
    {
        std::unique_lock<std::mutex> lock(m_pImpl->mtx_);
        threadpool::Task task = {fnTask, pData};
        m_pImpl->task_queue_.emplace(task);
    }
    m_pImpl->cv_.notify_one();
    return true;
}

void threadpool::ThreadPool::StopAll(bool bWait)
{
    {
        std::unique_lock<std::mutex> lock(m_pImpl->mtx_);
        m_pImpl->stop_all_ = true;
        m_pImpl->cv_.wait(lock, [&]()->bool{
            return m_pImpl->task_queue_.empty();
        });
    }
    m_pImpl->cv_.notify_all();

    for(auto& th: m_pImpl->threads_)
    {
        if (bWait)
            th.join();
        else th.detach();
    }
}