/**
 * This module represents a Lifo algorithm.
 * That means, according to ship's space constrains, a container with farther destination
 * will be loaded to lower spot on deck.
 * First, it unloads containers relative with this port.
 * (Containers that not getting in the way between containers.destination == this.port to the port are not unloaded! no unnecessary action).
 * Second, it loads containers with destination != this port.
 * Finally, it loads containers given in input file.
 */

#ifndef SHIP1_LIFO_ALGORITHM_H
#define SHIP1_LIFO_ALGORITHM_H

#include <memory>
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/Ship.h"
#include "../common/Parser.h"
#include "../common/Common.h"
#include "../interfaces/AlgorithmRegistration.h"
#include "../interfaces/WeightBalanceCalculator.h"
#include "../common/Port.h"

class _313263204_a: public AbstractAlgorithm {
    const std::string name = "_313263204_a";
    int portNum = 0;
    std::unique_ptr<Ship> pShip;
    std::shared_ptr<Port> pPort;
    WeightBalanceCalculator calc;
    std::array<bool,NUM_OF_ERRORS> errorCodes{false};
    std::set<std::string> idSet;
public:
    explicit _313263204_a(): AbstractAlgorithm(){}
    ~_313263204_a() override= default;

    /*-------------- Overriding Methods --------------*/
    int readShipPlan(const std::string& full_path_and_file_name) override;
    int readShipRoute(const std::string& full_path_and_file_name) override;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) override;

    /*-------------- Supporting Methods --------------*/
    void unloadContainers(std::ofstream &output);
    void handleColumn(coordinate coor, vector<Container> *column, int lowest_floor, vector<Container> *containersToUnload, std::ofstream &output);
    void unloadSingleContainer(std::ofstream &output, Container &con, Type vecType, coordinate coor);
    void loadContainers(Type list_category, std::ofstream &output);
    void initContainersDistance(vector<Container> &vector);
    int getPortNum();
    int resetAndReturn();
};

#endif