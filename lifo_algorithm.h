/**
 * This module represents a Lifo Algorithm.
 * That means, according to ship's space constrains, a container with farther destination
 * will be loaded to lower spot on deck.
 * First, it unloads containers relative with this port.
 * Second, it loads containers with destination != this port.
 * Finally, it loads containers given in input file.
 */

#ifndef SHIP1_LIFO_ALGORITHM_H
#define SHIP1_LIFO_ALGORITHM_H

//#include "stowage_algorithm.h"
#include <memory>
#include "AbstractAlgorithm.h"
#include "ship.h"
#include "Parser.h"
#include "common.h"

class Lifo_algorithm: public AbstractAlgorithm {
    const std::string name = "Lifo_algorithm";
    int portNum = 0;
    std::unique_ptr<Ship> pShip;
    std::shared_ptr<Port> pPort;
public:
    explicit Lifo_algorithm(): AbstractAlgorithm(){}
    ~Lifo_algorithm() override= default;
    int readShipPlan(const std::string& full_path_and_file_name) override;
    int readShipRoute(const std::string& full_path_and_file_name) override;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;
    int getInstructionsForCargo(
            const std::string& input_full_path_and_file_name,
            const std::string& output_full_path_and_file_name) override;
    void unloadContainers(std::ofstream &output);
    void unloadSingleContainer(std::ofstream &output, Container &con, char vecType, coordinate coor);
    const string getTypeName() const;
    int getPortNum();
    void loadContainers(char list_category, std::ofstream &output);
    void initContainersDistance(vector<Container> &vector);
};


//
//class Lifo_algorithm: public Algorithm {
//    const std::string name = "Lifo_algorithm";
//public:
//    explicit Lifo_algorithm(Ship* ship): Algorithm(ship){}
//    ~Lifo_algorithm() = default;
//    void getInstructionsForCrane(std::ofstream &output) override;
//    const std::string getTypeName() const override;
//    void unloadContainers(std::ofstream& output) override;
//    void loadContainers(char list_category, std::ofstream &output) override;
//    void unloadSingleContainer(std::ofstream &output, Container& con, char vecType, coordinate coor);
//};


#endif //SHIP1_LIFO_ALGORITHM_H

