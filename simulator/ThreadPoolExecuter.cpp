#include "ThreadPoolExecuter.h"

template<typename Producer>
bool ThreadPoolExecuter<Producer>::start() {
    bool running_status = false;
    if(!running.compare_exchange_strong(running_status, true)) {
        return false;
    }
    for(int i=0; i<numThreads; ++i) {
        workers.push_back(std::thread([this]{worker_function();}));
    }
    return true;
}

template<typename Producer>
void ThreadPoolExecuter<Producer>::wait_till_finish() {
    for(auto& t : workers) {
        t.join();
    }
}

template<typename Producer>
void ThreadPoolExecuter<Producer>::worker_function() {
    while(!stopped) {
        auto task = producer.getTask();
        if(!task) break;
        (*task)();
    }
}