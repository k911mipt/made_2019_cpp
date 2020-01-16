#pragma once
#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <utility>
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <future>
#include <condition_variable>
#include "common.h"

_MADE_BEGIN
_MULTITHREADING_BEGIN

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize);
    ~ThreadPool();

    template <class Func, class... Args>
    auto exec(Func func, Args... args)->std::future<decltype(func(args...))> {
        using task_type = decltype(func(args...));
        std::function<task_type()> binded_func = std::bind(func, std::forward<Args>(args)...);
        auto task = std::make_shared<std::packaged_task<task_type()>>(binded_func);
        {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            tasks_.push([task]() { (*task)(); });
        }
        tasks_notifier_.notify_one();
        return task->get_future();
    }
private:
    std::vector<std::thread> pool_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable tasks_notifier_;
    bool shutdown_ = false;
    void RunThreadLifeCycle();
};

_MULTITHREADING_END
_MADE_END

#endif // !THREAD_POOL_H_