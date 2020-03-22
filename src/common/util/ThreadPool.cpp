//
// Created by thallock on 2/18/20.
//

#include "ThreadPool.h"

bool ThreadPool::runNextTask() {
    std::function<void()> task;

    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->condition.wait(lock,
                             [this]{ return this->stop || !this->tasks.empty(); });
        if(this->stop && this->tasks.empty())
            return true;
        task = std::move(this->tasks.front());
        this->tasks.pop();
    }

    task();

    return false;
}

ThreadPool::ThreadPool(size_t threads) :
    stop{false}
{
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
                [this]
                {
                    while (runNextTask())
                        ;
                }
        );
}

void ThreadPool::enqueue(std::function<void(void)>&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("Enqueue on stopped ThreadPool");

        tasks.emplace(f);
    }
    condition.notify_one();
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
