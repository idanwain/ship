#ifndef COMMON_H
#define COMMON_H

#include "Parser.h"

enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};

void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
bool validateContainerData(const std::string& line, VALIDATION& reason, std::string& id, Ship* ship);
std::vector<std::string> stringSplit(std::string s, const char* delimiter);
bool is_number(const std::string& s);
bool isValidPortName(const std::string& name);
bool validateId(const std::string& str);
bool idExistOnShip(const std::string& id, Ship* ship);
bool isPortInRoute(Port *pPort, const std::vector<Port*>& route, int portNum);
void validateAlgorithm(string &outputPath,string &inputPath, Ship* simulatorShip, int portNumber);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,Ship* ship,std::map<string,string> &portContainers);
bool validateLoadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateUnloadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateMoveInstruction(vector<int> coordinates, vector<vector<vector<Container>>>& map);
bool validateRejectInstruction(std::map<string,string>& portContainers, string& id,Ship* ship);
void getTravelRoute(Ship* &ship, std::istream &inFile);
void parseInstruction(string &toParse,std::pair<string,string>& idAndInstruction,vector<int> &coordinates);

#endif
