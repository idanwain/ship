/**
 * This module represents an abstract stowage algorithm.
 * The Algorithm class is the base class and each "concrete" algorithms will implement the get_instructions_for_cargo function.
 */

#ifndef SHIP1_STOWAGE_ALGORITHM_H
#define SHIP1_STOWAGE_ALGORITHM_H

#include "ship.h"
#include "container.h"
#include "port.h"
#include <stack>
#include <iostream>
#include <fstream>
#define CONTAINER_FILE_ERROR "The data in this file is not valid!"
const char delim[] = {',','\t','\r',' '};


class Algorithm {
    inline static int portNum = 0;
    //std::map<std::string, Error> errors; //TODO create a map of containers ID and Error

protected:
    Port* port = nullptr;
    Ship* ship = nullptr;
public:

    explicit Algorithm(Ship* ship): ship(ship) {}

    bool operator()(const std::string& input_full_path_and_file_name,
                    const std::string& output_full_path_and_file_name);
    bool parse_data_to_port(const std::string& input_full_path_and_file_name);
    static std::vector<std::string> string_split(std::string s, const char* delimiter);
    void extract_containers_data(const std::string& line, std::string &id, int &weight, Port** dest);
    virtual void get_instructions_for_crane(std::ofstream& outfile) = 0;
    static bool validate_container_data(const std::string& line);
};

#endif //SHIP1_STOWAGE_ALGORITHM_H
