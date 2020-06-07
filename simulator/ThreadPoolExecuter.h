#ifndef WINDOWSSHIP_THREADPOOLEXECUTER_H
#define WINDOWSSHIP_THREADPOOLEXECUTER_H

/**
* This header is a container of the thread pool executer, that manages producer
* and workers using the TaskProducer module
*/
#include "TaskProducer.h"
#include <vector>
#include <thread>
#include <iostream>
#include <atomic>


class ThreadPoolExecuter {
    TaskProducer producer;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running = false;
    std::atomic_bool stopped = false;

    void worker_function();

public:
    ThreadPoolExecuter(TaskProducer producer, int numThreads)
            : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }

    bool start();
    void wait_till_finish();
};

#endif