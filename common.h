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

#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "ship.h"
#include "Parser.h"
#include "SimulatorObj.h"
#include "AbstractAlgorithm.h"

/*----------------------Prefix variables-------------------*/

#define NUM_OF_ERRORS 19

/*----------------------Common Error Codes-------------------*/
#define Plan_ZError     (1 << 0) /*Error at ship plan Z axis*/
#define Plan_XYError    (1 << 1) /*Error at ship plan xy axis*/
#define Plan_BadLine    (1 << 2) /*Error bad line reading or duplicates*/
#define Plan_Fatal      (1 << 3) /*Fatal Error in ship plan file*/
#define Plan_Con        (1 << 4) /*Fatal Error already given x,y constraints with different Z coordinate*/
#define Route_PortTwice (1 << 5) /*Port occurs 2 or more consecutive times*/
#define Route_badPortS  (1 << 6) /*Bad port symbol*/
#define Route_Fatal     (1 << 7) /*Fatal Error in route file*/
#define Route_SingleP   (1 << 8) /*Single Port in the Route*/
#define Reserved2       (1 << 9)
#define C_DuplicateId   (1 << 10)/*2 containers with same Id*/
#define C_IdAlreadyOn   (1 << 11)/*Id already on ship*/
#define C_WeightIssue   (1 << 12)/*Any weight issue, bad weight/missing*/
#define C_PortIssue     (1 << 13)/*Missing port,port not in dest*/
#define C_IdCantRead    (1 << 14)/*Cant read Id??*/
#define C_IdIllegal     (1 << 15)/*Id is not illegal ISO 6346*/
#define C_FileCantRead  (1 << 16)/*Assuming no cargo to load at this port*/
#define C_LastPortCont  (1 << 17)/*Last port has awaiting containers*/
#define C_ExceedsCap    (1 << 18)/*total containers amount exceeds ship cap*/

enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};


/*----------------------Validate functions-------------------*/
bool isValidPortFileName(const string& fileName);
bool isValidShipMapFileName(const string& fileName);
bool isValidShipRouteFileName(const string& fileName);
bool isValidTravelName(const string& travelName);
bool isValidPortName(const string& portName);
bool isValidInteger(const string str);
bool isCommentLine(const string& line);
bool validateId(const string& str);
std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &contAtPortPath,int portNumber, list<string>& currAlgErrors,SimulatorObj* simulator,string& portName,int visitNumber);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,std::unique_ptr<Ship>& ship,std::map<string,list<string>> &portContainers,int portNum);
bool validateLoadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship);
bool validateUnloadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship);
bool validateMoveInstruction(vector<int> &coordinates, vector<vector<vector<Container>>>& map);
bool validateRejectInstruction(std::map<string,list<string>>& portContainers, string& id,std::unique_ptr<Ship>& ship,int portNum);
bool validateContainerData(const string& line, VALIDATION& reason, string& id, std::unique_ptr<Ship>& ship);
/*----------------------Extract functions-------------------*/
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath,list<string> &generalErrors);
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath);//Overload
void extractContainersData(const std::string& line, std::string &id, int &weight, std::shared_ptr<Port>& dest, std::unique_ptr<Ship>& ship);
void extractCraneInstruction(string &toParse, std::pair<string,string>& instruction, vector<int> &coordinates);
/*----------------------Rest of the functions-------------------*/
void execute(std::unique_ptr<Ship>& ship, char command,std::unique_ptr<Container>& container, coordinate origin, coordinate dest);
bool isNumber(const std::string& s);
bool idExistOnShip(const string& id, std::unique_ptr<Ship>& ship);
bool isPortInRoute(const string& portName, const vector<std::shared_ptr<Port>>& route, int portNum);
bool parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output,
                     std::unique_ptr<Ship>& ship, std::shared_ptr<Port>& port);
vector<string> stringSplit(string s, const char* delimiter);
void writeToOutput(std::ofstream& output,
                   AbstractAlgorithm::Action command, const std::string& id,
                   const std::tuple<int,int,int> pos = std::forward_as_tuple(-1,-1,-1),
                   const std::tuple<int,int,int>& movedTo = std::forward_as_tuple(-1,-1,-1));
void initArrayOfErrors(std::array<bool,NUM_OF_ERRORS> &arr,int num);
void updateErrorNum(int* currError,int newError);

#endif
