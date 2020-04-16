/**
 * This module represents an erroneous algorithm.
 * The algorithm rejects every container needed to be load.
 * First, it unloads all the containers needed to port;
 * Second, it rejects all the containers that were unloaded from the ship but their destination != this port.
 * Finally, it rejects the containers as mentioned in input file.
 * this type of action creates errors in algorithm and lets "validation algorithm" find errors in algorithm.
 */
#ifndef ERRONEUOS_ALGORITHM_H
#define ERRONEUOS_ALGORITHM_H


#include "stowage_algorithm.h"


class Erroneous_algorithm: public Algorithm {
    const std::string name = "Erroneous_algorithm";
public:
    explicit Erroneous_algorithm(Ship* ship): Algorithm(ship){}
    ~Erroneous_algorithm() = default;
    void getInstructionsForCrane(std::ofstream &output) override;
    const std::string getTypeName() const override;
    void unloadContainers(std::ofstream& output) override;
    void loadContainers(char list_category, std::ofstream &output) override;
    void unloadSingleContainer(std::ofstream &output, Container& con, char vecType, coordinate coor);
};

#endif
