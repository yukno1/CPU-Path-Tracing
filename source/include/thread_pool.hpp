#pragma once

#include "spin_lock.hpp"
#include <functional>
#include <queue>
#include <thread>
#include <vector>


class Task
{
public:
    virtual void run() = 0;
};

class ThreadPool
{
public:
    static void WorkerThread(ThreadPool* master);

    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();

    void parallelFor(size_t width, size_t height,
                     const std::function<void(size_t, size_t)>& lambda);
    void wait() const;

    void  addTask(Task* task);
    Task* getTask();

    std::atomic<bool> alive;

private:
    std::vector<std::thread> threads;
    std::queue<Task*>        tasks;
    SpinLock                 spin_lock{};
};