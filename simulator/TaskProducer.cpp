#include "TaskProducer.h"

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
    std::cout << "TaskProducer::getTask: vector size = " << numTasks << std::endl;
    auto task_index = next_task_index();
    if(task_index) {
        return [task_index, this]{
            std::lock_guard g{m};
            std::cout << std::this_thread::get_id() << std::endl;
            auto& tuple = travelForAlgs.at(task_index.value());
            auto& travel = std::get<0>(tuple);
            auto& algName = std::get<1>(tuple);
            auto& alg = std::get<2>(tuple);

            std::cout << "in task: travel name: " << travel->getName() << " alg: " << algName << std::endl;

            SimulatorObj simulator{};

            if(!travel->isShipInit()){
                travel->getShip() = extractArgsForShip(travel, simulator);
                travel->setInitShip();
            }
            if(travel->getShip() != nullptr && !travel->isErroneous()){
                int errCode1 = 0, errCode2 = 0;
                    WeightBalanceCalculator algCalc;
                    try {
                        errCode1 = alg->readShipPlan(travel->getPlanPath().string());
                        errCode2 = alg->readShipRoute(travel->getRoutePath().string());
                        errCode1 |= algCalc.readShipPlan(travel->getPlanPath().string());
                    }
                    catch(...) {
                        travel->setAlgCrashError(algName);
                    }
                    alg->setWeightBalanceCalculator(algCalc);
                    simulator.updateErrorCodes(errCode1 + errCode2, "alg");
                    simulator.setShipAndCalculator(travel->getShip(), travel->getPlanPath().string());
                    std::pair algPair = std::make_pair(algName, std::move(alg));
                    simulator.runAlgorithm(algPair, travel);
                }
            else
                travel->setErroneousTravel();

            std::this_thread::yield();
        };
    }
    else return {};
}
