#ifndef COMMON_H
#define COMMON_H

#include "Parser.h"
#include "stowage_algorithm.h"

enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};

void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
void validateAlgorithm(string &outputPath,string &inputPath, Ship* simulatorShip, int portNumber);
void parseInstruction(string &toParse,std::pair<string,string> &instruction,vector<int> &coordinates);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,Ship* ship,std::map<string,string> &portContainers);
bool validateLoadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateUnloadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateMoveInstruction(vector<int> coordinates, vector<vector<vector<Container>>>& map);
bool validateRejectInstruction(std::map<string,string>& portContainers, string& id,Ship* ship);

#endif
