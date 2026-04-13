#include "thread_pool.hpp"

void ThreadPool::WorkerThread(ThreadPool *master)
{
    while (master->alive)
    {
        Task *task = master->getTask();
        if (task != nullptr)
        {
            task->run();
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count)
{
    alive = true;
    if (thread_count == 0)
    {
        thread_count = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        threads.push_back(std::thread(ThreadPool::WorkerThread, this));
    }
}

ThreadPool::~ThreadPool()
{
    // 等待任务清空，否则会直接析构
    while (!tasks.empty())
    {
        std::this_thread::yield(); // 把线程控制权交给os
    }
    alive = false;
    for (auto &thread : threads)
    {
        thread.join();
    }
    threads.clear();
}

void ThreadPool::addTask(Task *task)
{
    std::lock_guard<std::mutex> guard(lock);
    tasks.push_back(task);
}

Task *ThreadPool::getTask()
{
    std::lock_guard<std::mutex> guard(lock);
    if (tasks.empty())
    {
        return nullptr;
    }
    else
    {
        std::this_thread::yield();
    }
    Task *task = tasks.front();
    tasks.pop_front();
    return task;
}