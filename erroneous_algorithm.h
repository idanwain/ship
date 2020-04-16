
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
