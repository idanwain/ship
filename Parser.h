/**
* This header represents a module that involves in reading and parsing(extracting) files or information given in
* any constellation.
*
*      *******      Functions      *******
* getPortNumFile            - gets the number from the port file, <port_symbol>_<num>.<filetype>.
* isValidPortExpressionFile - checks if the port expression file is valid
* isValidPortName           - check if the port name given in the route file is valid name
* isValidTravelName         - check if the name of the travel folder is valid name
* portAlreadyExists         - checks in a given list of ports if the given port already exists, and pushes it back if so
* orderListDirectories      - arranging the list of the travel directories with the following format map,route,port1,port2 etc..
* initListDirectories       - initialize the list of the directories from a given root path and store it in a data structure
* validateSequenceDirectories - validating the each travel folder has a all valid files exists
* setActualSize             - gets a vector and "shrink" it to it's actual size
* extractArgsForShip        - given ship_map and ship_route files it's builds a ship with a map and route from them
* extractArgsForBlocks      - used by above function and it sets the block containers in the ship map from file information
* setBlocksByLine           - used as a utility function of extractArgsForBlocks
* getTravelRoute            - reads the ship_route file and extract the ship route and set it to the ship route member
* getDimensions             - reads a line of 3 number <x y z> and parse the numbers out of it
*

 */
#ifndef PARSER_HEADER
#define PARSER_HEADER
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include "ship.h"
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
namespace fs = std::filesystem;
#define OP_MAIN_DIRECTORY "\\output"
#define FAIL_TO_READ_PATH "Failed to read from this file path "


int getPortNumFile(const string& file_name);
bool isValidPortExpressionFile(const string& file_name);
std::vector<std::vector<fs::path>> orderListOfDir(std::list<std::list<fs::path>> &unOrdered);
void initListDirectories(string &path,std::vector<std::vector<fs::path>> &vecOfPaths);
void validateSequenceDirectories(std::vector<std::vector<fs::path>> &direct);
bool isValidPortName(const string& name);
bool isValidTravelName(const string& name);
void setActualSize(std::vector<std::vector<fs::path>> &direct);
Ship* extractArgsForShip(std::vector<fs::path> &folder);
void extractArgsForBlocks(Ship* &ship, std::istream &inFile);
void setBlocksByLine(std::string &str, Ship* &ship);
void getTravelRoute(Ship* &ship, std::istream &inFile);
void getDimensions(std::array<int,3> &arr, std::istream &inFile,string str);
int portAlreadyExist(std::vector<Port*> &vec,string &str);
void parseDataFromPortFile(std::map<string,Container*>& map, string &inputPath,Ship* simulatorShip);
string* getPortNameFromFile(string &filePath);

#endif // !PARSER_HEADER