#ifndef WINDOWSSHIP_TASKPRODUCER_H
#define WINDOWSSHIP_TASKPRODUCER_H

#include <atomic>
#include <mutex>
#include <optional>
#include <functional>
#include <iostream>
#include <thread>
#include "Travel.h"
#include "../interfaces/AbstractAlgorithm.h"

template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class NumTasks: public Named<int> {
    using Named<int>::Named;
};

class TaskProducer {
    std::vector<std::pair<std::unique_ptr<Travel>, vector<pair<string,std::unique_ptr<AbstractAlgorithm>>>>> travelForAlgs;
    const int numTasks = -1; //num of Travels
    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

public:
    TaskProducer(std::vector<std::pair<std::unique_ptr<Travel>, vector<pair<string,std::unique_ptr<AbstractAlgorithm>>>>>& _travelForAlgs) : travelForAlgs(_travelForAlgs), numTasks(_travelForAlgs.size()) {};

    TaskProducer(TaskProducer&& other)
            : travelForAlgs(other.travelForAlgs), numTasks(other.numTasks), task_counter(other.task_counter.load()) {};

    std::optional<std::function<void(void)>> getTask();
};

#endif