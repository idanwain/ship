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
    void unloadSingleContainer(std::ofstream &output, Container& con, char vecType, coordinate coor);
};


#endif //SHIP1_LIFO_ALGORITHM_H

