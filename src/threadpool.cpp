#include "threadpool.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <cassert>


namespace threadpool{

struct Task{
    FnTask fnTask_;
    void* pData_;
};

struct ThreadPool::Impl
{
    bool stop_all_{false};
    std::condition_variable cv_;
    std::mutex mtx_;
    std::vector<std::thread> threads_;
    std::queue<Task> task_queue_;
};


void ThreadFunc(ThreadPool::Impl* pData)
{
    for (;;)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(pData->mtx_);
            pData->cv_.wait(lock, [&]()->bool{
                return !pData->task_queue_.empty() || pData->stop_all_;
            });

            if (pData->task_queue_.empty()){
                break;
            }
            task = pData->task_queue_.front();
            pData->task_queue_.pop();
        }
        // 通知主线程的等待， 主线程等待任务队列清空
        pData->cv_.notify_one();
        
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
        m_pImpl->cv_.wait(lock, [&]()->bool{
            return m_pImpl->task_queue_.empty();
        });
        m_pImpl->stop_all_ = true;
    }
    m_pImpl->cv_.notify_all();

    for(auto& th: m_pImpl->threads_)
    {
        if (bWait)
            th.join();
        else th.detach();
    }
}