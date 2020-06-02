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
#include "Util.h"

class TaskProducer {
    tasksContainer travelForAlgs;
    const int numTasks = -1; //travel X alg vec size
    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

public:
    TaskProducer(tasksContainer& _travelForAlgs) : travelForAlgs(std::move(_travelForAlgs)), numTasks(travelForAlgs.size()) {};

    TaskProducer(TaskProducer&& other)
            : travelForAlgs(std::move(other.travelForAlgs)), numTasks(other.numTasks), task_counter(other.task_counter.load()) {};

    std::optional<std::function<void(void)>> getTask();
};

#endif