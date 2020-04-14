/**
 * This module represents a Lifo Algorithm: Last In First Out.
 * First, we UNLOAD all the containers needed to be UNLOADED, with probably some extra unrelated containers, to port.
 * All the containers that this port is not their destination would be pushed into a STACK,
 * according to the order of their destination port init the ship's route.
 * Then, the containers in the stack would pop one by one into the ship until there is no space or weight problems.
 */

#ifndef SHIP1_LIFO_ALGORITHM_H
#define SHIP1_LIFO_ALGORITHM_H

#include "stowage_algorithm.h"


class Lifo_algorithm: public Algorithm {
    const std::string name = "Lifo_algorithm";
public:
    explicit Lifo_algorithm(Ship* ship): Algorithm(ship){}
    ~Lifo_algorithm() = default;
    void getInstructionsForCrane(std::ofstream &output) override;
    const std::string getTypeName() const override;
    void unloadContainers(std::ofstream& output) override;
    void loadContainers(char list_category, std::ofstream &output) override;
    void unloadSingleContainer(std::ofstream &output, Container& con, char vecType);
};


#endif //SHIP1_LIFO_ALGORITHM_H

