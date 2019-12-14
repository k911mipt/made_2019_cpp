#include "thread_pool.hpp"
#include "common.h"

_MADE_BEGIN
_MULTITHREADING_BEGIN

ThreadPool::ThreadPool(size_t poolSize) {
    for (size_t i = 0; i < poolSize; ++i) {
        pool_.push_back(std::thread(&ThreadPool::RunThreadLifeCycle, this));
    }
}

ThreadPool::~ThreadPool() {
    shutdown_ = true;
    tasks_notifier_.notify_all();
    for (auto& thread : pool_)
        if (thread.joinable())
            thread.join();
}

void ThreadPool::RunThreadLifeCycle() {
    while (true) {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        while (!shutdown_ && tasks_.empty())
            tasks_notifier_.wait(lock);
        if (shutdown_)
            return;
        tasks_.front()();
        tasks_.pop();
    }
}

_MULTITHREADING_END
_MADE_END


