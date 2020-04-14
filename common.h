#ifndef COMMON_H
#define COMMON_H

#include <regex>
#include "ship.h"
enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};
#include "stowage_algorithm.h"


void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
bool validateContainerData(const std::string& line, VALIDATION& reason, std::string& id, Ship* ship);
std::vector<std::string> stringSplit(std::string s, const char* delimiter);
bool is_number(const std::string& s);
bool isValidPortName(const std::string& name);
bool validateId(const std::string& str);
bool idExistOnShip(const std::string& id, Ship* ship);
bool isPortInRoute(Port *pPort, const std::vector<Port*>& route, int portNum);
static void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
void parseInstruction(string &toParse,std::pair<string,string> idAndInstruction,vector<int> &coordinates);
void validateAlgorithm(string &path,Ship* simulatorShip);
bool validateInstruction(string &instruction,vector<int> &coordinates,Ship* ship);
bool validateLoadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateUnloadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateMoveInstruction(vector<int> &coordinates,vector<vector<vector<Container>>>& map);
#endif
