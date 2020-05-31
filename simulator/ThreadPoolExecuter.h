#ifndef WINDOWSSHIP_THREADPOOLEXECUTER_H
#define WINDOWSSHIP_THREADPOOLEXECUTER_H

#include <vector>
#include <thread>
#include <iostream>
#include <atomic>

template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class NumThreads: public Named<int> {
    using Named<int>::Named;
};

template<typename Producer>
class ThreadPoolExecuter {
    Producer producer;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running = false;
    std::atomic_bool stopped = false;

    void worker_function();

public:
    ThreadPoolExecuter(Producer producer, NumThreads numThreads)
            : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }

    bool start();
    void wait_till_finish();
};

#endif