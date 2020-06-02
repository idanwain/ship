#include "ThreadPoolExecuter.h"

bool ThreadPoolExecuter::start() {
    bool running_status = false;
    if(!running.compare_exchange_strong(running_status, true)) {
        return false;
    }
    for(int i=0; i<numThreads; ++i) {
        workers.push_back(std::thread([this]{worker_function();}));
    }
    return true;
}

void ThreadPoolExecuter::wait_till_finish() {
    for(auto& t : workers) {
        t.join();
    }
}

void ThreadPoolExecuter::worker_function() {
    while(!stopped) {
        auto task = producer.getTask();
        if(!task) break;
        (*task)();
    }
}