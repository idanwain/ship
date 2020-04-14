#ifndef SHIP1_UNSORTED_LIFO_ALGORITHM_H
#define SHIP1_UNSORTED_LIFO_ALGORITHM_H
#include "stowage_algorithm.h"

class Unsorted_Lifo_Algorithm: public Algorithm {
    const std::string name = "Unsorted_Lifo_Algorithm";
public:
    explicit Unsorted_Lifo_Algorithm(Ship* ship): Algorithm(ship){}
    ~Unsorted_Lifo_Algorithm() = default;
    void getInstructionsForCrane(std::ofstream &output) override;
    const std::string getTypeName() const override;
    void unloadContainers(std::ofstream& output) override;
    void loadContainers(char list_category, std::ofstream &output) override;
    void unloadSingleContainer(std::ofstream &output, Container& con, char vecType, coordinate coor);
};

#endif //SHIP1_UNSORTED_LIFO_ALGORITHM_H
