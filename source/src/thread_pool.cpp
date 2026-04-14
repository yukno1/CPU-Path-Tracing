#include "thread_pool.hpp"

void ThreadPool::WorkerThread(ThreadPool* master)
{
    while (master->alive) {
        Task* task = master->getTask();
        if (task != nullptr) {
            task->run();
            master->pending_task_count--;
        }
        else {
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count)
{
    alive              = true;
    pending_task_count = 0;
    if (thread_count == 0) {
        thread_count = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < thread_count; i++) {
        threads.push_back(std::thread(ThreadPool::WorkerThread, this));
    }
}

/**
 * 目前只支持在主线程中添加任务
 */
ThreadPool::~ThreadPool()
{
    wait();
    alive = false;
    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();
}

class ParallelForTask : public Task
{
public:
    ParallelForTask(size_t x, size_t y, const std::function<void(size_t, size_t)>& lambda)
        : x(x)
        , y(y)
        , lambda(lambda)
    {}

    void run() override { lambda(x, y); }

private:
    size_t                              x, y;
    std::function<void(size_t, size_t)> lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height,
                             const std::function<void(size_t, size_t)>& lambda)
{
    // Guard guard(spin_lock);

    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            // tasks.push(new ParallelForTask(x, y, lambda));
            addTask(new ParallelForTask(x, y, lambda));
        }
    }
}

/**
 * 等待任务清空，否则会直接析构
 */
void ThreadPool::wait() const
{
    while (pending_task_count > 0) {
        std::this_thread::yield();   // 把线程控制权交给os
    }
}

void ThreadPool::addTask(Task* task)
{
    Guard guard(spin_lock);
    pending_task_count++;
    tasks.push(task);
}

Task* ThreadPool::getTask()
{
    Guard guard(spin_lock);
    if (tasks.empty()) {
        return nullptr;
    }
    else {
        std::this_thread::yield();
    }
    Task* task = tasks.front();
    tasks.pop();
    return task;
}