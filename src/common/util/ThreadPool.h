//
// Created by thallock on 2/18/20.
//

// https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h


#ifndef PLAYING_THREADPOOL_H
#define PLAYING_THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    bool runNextTask();
public:
    ThreadPool(size_t);

    void enqueue(std::function<void(void)>&& f);

    ~ThreadPool();
};


#endif //PLAYING_THREADPOOL_H
