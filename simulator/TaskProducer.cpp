#include "TaskProducer.h"
#include "SimulatorObj.h"

std::optional<int> TaskProducer::next_task_index() {
    if(task_counter < numTasks) {
        int next_counter = ++task_counter;
        if(next_counter <= numTasks) {
            return {next_counter - 1};
        }
        else {
            --task_counter;
        }
    }
    return {};
}

std::optional<std::function<void(void)>> TaskProducer::getTask() {
    auto task_index = next_task_index();
    if(task_index) {
        return [task_index, this]{
            std::lock_guard g{m};
            auto& tuple = travelForAlgs.at(task_index.value());
            auto& travel = std::get<0>(tuple);
            auto& alg = std::get<1>(tuple);

            SimulatorObj simulator{};

            if(!travel->isShipInit()){
                travel->getShip() = extractArgsForShip(travel, simulator);
                travel->setInitShip();
            }
            if(travel->getShip() != nullptr && !travel->isErroneous()){
                int errCode1 = 0, errCode2 = 0;
                    WeightBalanceCalculator algCalc;
                    try {
                        errCode1 = alg.second->readShipPlan(travel->getPlanPath().string());
                        errCode2 = alg.second->readShipRoute(travel->getRoutePath().string());
                        errCode1 |= algCalc.readShipPlan(travel->getPlanPath().string());
                    }
                    catch(...) {
                        travel->setAlgCrashError(alg.first);
                    }
                    alg.second->setWeightBalanceCalculator(algCalc);
                    simulator.updateErrorCodes(errCode1 + errCode2, "alg");
                    simulator.setShipAndCalculator(travel->getShip(), travel->getPlanPath().string());
                    simulator.runAlgorithm(alg, travel);
                }
            else
                travel->setErroneousTravel();

            std::this_thread::yield();
        };
    }
    else return {};
}
