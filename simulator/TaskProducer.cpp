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
            auto& pair = travelForAlgs.at(task_index.value());
            auto& travel = pair.first;
            auto& algVec = pair.second;

            SimulatorObj simulator{}; //TODO: implement empty ctor if needed
            std::unique_ptr<Ship> mainShip = extractArgsForShip(travel, simulator);
            if(mainShip != nullptr){
                for (auto &alg : algVec) {
                    int errCode1 = 0, errCode2 = 0;
                    WeightBalanceCalculator algCalc;
                    try {
                        errCode1 = alg.second->readShipPlan(travel->getPlanPath().string());
                        errCode2 = alg.second->readShipRoute(travel->getRoutePath().string());
                        errCode1 |= algCalc.readShipPlan(travel->getPlanPath().string());
                    }
                    catch(...) {
                        travel->setAlgCrashError(alg.first);
                        continue;
                    }
                    alg.second->setWeightBalanceCalculator(algCalc);
                    simulator.updateErrorCodes(errCode1 + errCode2, "alg");
                    simulator.setShipAndCalculator(mainShip, travel->getPlanPath().string());
                    simulator.runAlgorithm(alg, travel);
                }
            }
            else
                travel->setErroneousTravel();

            //not sure if relevant in this context
            simulator.prepareNextIteration();

            std::this_thread::yield();
        };
    }
    else return {};
}

