#ifndef COMMON_H
#define COMMON_H

#include "parser.h"
#include "stowage_algorithm.h"


static void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
void parseInstruction(string &toParse,std::pair<string,string> idAndInstruction,vector<int> &coordinates);
void validateAlgorithm(string &path,Ship* simulatorShip);
bool validateInstruction(string &instruction,vector<int> &coordinates,Ship* ship);
bool validateLoadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateUnloadInstruction(int x, int y, int z,vector<vector<vector<Container>>>& map);
bool validateMoveInstruction(vector<int> &coordinates,vector<vector<vector<Container>>>& map);
#endif
