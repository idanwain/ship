/**
* This header represents a module that contains functions that can be used by both simulator and algorithm.
*
*     *******      Functions      *******
* execute                   - executes an instruction given by algorithm to crane on the simulator map
* isNumber                  - checks if given string is a number
* extractTravelRoute        - extract travel route from ship_route file
* extractCraneInstruction   - given a crane instruction it's parsing it to get the information
* stringSplit               - given a string and a delimiter it returns a vector contain str1,str2,str3 that any delim appeared between them
* idExistOnShip             - check if given id already exist in ship map
* isPortInRoute             - check if given port name exist in the following ports according to ship route
* isValidPortName           - validating if given port name is in the right format (seaport code)
* validateId                - validating container id
* validateContainerData     - validate container data line given in a cargo_data file
* validateAlgorithm         - validating all algorithm crane instruction on a single port
* validateInstruction       - validating single instruction
* validateLoadInstruction   - validating load instruction
* validateUnloadInstruction - validating unload instruction
* validateMoveInstruction   - validating move instruction
* validateRejectInstruction - validating reject instruction
* enumClass VALIDATION      - holds the reason for invalid container data
 */


#ifndef COMMON_H
#define COMMON_H

#include "Parser.h"

enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};

void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
bool validateContainerData(const string& line, VALIDATION& reason, string& id, Ship* ship);
bool isNumber(const std::string& s);
bool isValidPortName(const string& portName);
bool validateId(const string& str);
bool idExistOnShip(const string& id, Ship* ship);
bool isPortInRoute(string portName, const vector<Port*>& route, int portNum);
void validateAlgorithm(string &outputPath, string &inputPath, Ship* simShip, int portNumber, list<string>& currAlgErrors);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,Ship* ship,std::map<string,string> &portContainers,int portNum);
bool validateLoadInstruction(vector<int> &coordinates,Ship* ship);
bool validateUnloadInstruction(vector<int> &coordinates,Ship* ship);
bool validateMoveInstruction(vector<int> &coordinates, vector<vector<vector<Container>>>& map);
bool validateRejectInstruction(std::map<string,string>& portContainers, string& id,Ship* ship,int portNum);
void extractTravelRoute(Ship* &ship, std::istream &inFile,list<string> &generalErrors);
void extractCraneInstruction(string &toParse, std::pair<string,string>& instruction, vector<int> &coordinates);
vector<string> stringSplit(string s, const char* delimiter);

#endif
