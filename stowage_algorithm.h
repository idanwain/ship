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
#include <stack>
#include <set>
#include <iostream>
#include <fstream>
#define CONTAINER_FILE_ERROR "The data in this file is not valid!"
#define CONTAINER_NOT_IN_ROUTE "This container's destination is not in the ship's route"
#define CONTAINER_NOT_VALID "This container's data is not in the right format!"


class Algorithm {
    inline static int portNum = 0;
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
    void extract_containers_data(const std::string& line, std::string &id, int &weight, Port** dest);
    virtual void get_instructions_for_crane(std::ofstream& outfile) = 0;
    virtual const std::string getTypeName() const = 0;
    //virtual void load_containers(std::ofstream& output) = 0;
    //virtual void unload_containers(std::ofstream& output, std::vector<Container>& priority_to_load) = 0;
    virtual void unload_containers(std::ofstream& output) = 0;
    virtual void load_containers(char list_category, std::ofstream &output) = 0;
    void increase_instruction_counter(int instructionsAdded = 1);
    int getPortNum();

    /***********WANT TO MOVE TO PARSER*********/
    bool parse_data_to_port(const std::string& input_full_path_and_file_name);
    static bool validate_id(const std::string& str);
    static std::vector<std::string> string_split(std::string s, const char* delimiter);
    static bool validate_container_data(const std::string& line);

    static void write_to_output(std::ofstream &output, const string &command, const string &id,
                    const std::tuple<int, int, int> &pos,
                    const std::tuple<int,int,int>& moved_to);

    bool isPortInRoute(Port *pPort);
};

#endif //SHIP1_STOWAGE_ALGORITHM_H
