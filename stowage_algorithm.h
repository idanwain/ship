/**
 * This module represents an abstract stowage algorithm.
 * The Algorithm class is the base class and each "concrete" algorithms will implement the get_instructions_for_cargo function.
 */

#ifndef SHIP1_STOWAGE_ALGORITHM_H
#define SHIP1_STOWAGE_ALGORITHM_H

#include "ship.h"
#include "container.h"
#include "Parser.h"
#include "port.h"
#include "common.h"
#include <stack>
#include <set>
#include <iostream>
#include <fstream>
#define CONTAINER_FILE_ERROR "The data in this file is not valid!"
#define CONTAINER_NOT_IN_ROUTE "This container's destination is not in the ship's route"
#define ID_EXIST_ON_SHIP "This container's ID is not unique! rejecting container."


class Algorithm {
    int portNum = 0;
    int instructions = 0;
    //std::map<std::string, Error> errors; //TODO create a map of containers ID and Error

protected:
    Port* port = nullptr;
    Ship* ship = nullptr;
public:

    explicit Algorithm(Ship* ship): ship(ship) {}
    virtual ~Algorithm(){
      portNum = 0;
      delete ship;
    };
    bool operator()(const std::string& input_full_path_and_file_name,
                    const std::string& output_full_path_and_file_name);
    void extractContainersData(const std::string& line, std::string &id, int &weight, Port** dest);
    virtual void getInstructionsForCrane(std::ofstream& output) = 0;
    virtual const std::string getTypeName() const = 0;
    virtual void unloadContainers(std::ofstream& output) = 0;
    virtual void loadContainers(char list_category, std::ofstream &output) = 0;
    void increaseInstructionCounter(int instructionsAdded = 1);
    int getInstructionsCounter() const;
    Ship* getShip() const;
    int getPortNum();

    /***********WANT TO MOVE TO PARSER*********/
    bool parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output);
    static void writeToOutput(std::ofstream &output, const string &command, const string &id,
                    const std::tuple<int, int, int> &pos,
                    const std::tuple<int,int,int>& moved_to);
    void initContainersDistance(std::vector<Container> &vector);
};

#endif //SHIP1_STOWAGE_ALGORITHM_H
