/**
 * This module represents an unsorted "lifo algorithm".
 * The algorithm doesn't takes into consideration the order of the ports in ship's route.
 * First, it unloads all the containers needed to port;
 * Second, it loads all the containers that were unloaded from the ship but their destination != this port.
 * Finally, it loads te containers as mentioned in input file under the trivial constraint like space, weight etc.
 */

#ifndef SHIP1_UNSORTED_LIFO_ALGORITHM_H
#define SHIP1_UNSORTED_LIFO_ALGORITHM_H

#include <memory>
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/Ship.h"
#include "../common/Parser.h"
#include "../common/Common.h"
#include "../interfaces/AlgorithmRegistration.h"
#include "../interfaces/WeightBalanceCalculator.h"
#include "../common/Port.h"



class _313263204_b: public AbstractAlgorithm {
    const std::string name = "_313263204_b";
    int portNum = 0;
    std::unique_ptr<Ship> pShip;
    std::shared_ptr<Port> pPort;
    WeightBalanceCalculator calc;
    std::array<bool,NUM_OF_ERRORS> errorCodes{false};
    std::set<std::string> idSet;
public:
    explicit _313263204_b(): AbstractAlgorithm(){}
    ~_313263204_b() override= default;
    /*-------------- Overriding Methods --------------*/
    int readShipPlan(const std::string& full_path_and_file_name) override;
    int readShipRoute(const std::string& full_path_and_file_name) override;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;
    int getInstructionsForCargo(
            const std::string& input_full_path_and_file_name,
            const std::string& output_full_path_and_file_name) override;
    /*-------------- Supporting Methods --------------*/
    void unloadContainers(std::ofstream &output);
    void unloadSingleContainer(std::ofstream &output, Container &con, Type vecType, coordinate coor);
    void loadContainers(Type list_category, std::ofstream &output);
    int getPortNum();
    void handleColumn(coordinate coor, vector<Container> *column, int lowest_floor, vector<Container> *containersToUnload,
                      std::ofstream &output);
    int resetAndReturn();
};

#endif
