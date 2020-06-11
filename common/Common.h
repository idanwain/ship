/**
* This header represents a module that contains functions that can be used by both simulator and algorithm.
*
*     *******      Functions      *******
* isValidPortFileName       - checks if it's valid port file name
* isValidShipMapFileName    - checks if it's valid ship map file name
* isValidShipRouteFileName  - checks if it's valid route file name
* isValidTravelName         - checks if it's a valid travel name
* isValidPortName           - checks if it's valid port name
* isValidInteger            - checks if it's an integer of type +-x
* isCommentLine             - checks if a given string is comment or whitespaces
* isValidId                 - checks if it's valid container id based on iso
* validateContainerData     - validates all container data given at line
* idExistOnShip             - checks if given id already exist on ship map
* isPortInRoute             - checks if a given port is already in route
* stringSplit               - split string to an array of string based on given delimiter
* writeToOutput             - write's data to output file
* createContainer           - creates container by searching it's id in the data
* trimSpaces                - trim spaces from left and from right of a given string
 */



#ifndef COMMON_H
#define COMMON_H

class SimulatorObj;

#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Ship.h"
#include "Parser.h"
#include "Port.h"
#include "../simulator/SimulatorObj.h"
#include "../interfaces/ErrorsInterface.h"
#include "../interfaces/WeightBalanceCalculator.h"



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
#define reserved2           9
#define duplicateId         10    /*2 containers with same Id*/
#define idAlreadyOn         11    /*Id already on ship*/
#define weightIssue         12    /*Any weight issue, bad weight/missing*/
#define portIssue           13    /*Missing port,port not in dest*/
#define idCantRead          14    /*Cant read Id*/
#define idIllegal           15    /*Id is illegal ISO 6346*/
#define fileCantRead        16    /*Assuming no cargo to load at this port*/
#define lastPortCont        17    /*Last port has awaiting containers*/
#define exceedsCap          18    /*total containers amount exceeds ship cap*/

enum class VALIDATION {InvalidID, InvalidWeight, ExistID, InvalidPort,Valid,InvalidNumParameters,DuplicatedIdOnPort};


/*----------------------Validate functions-------------------*/
bool isValidPortFileName(const string& fileName);
bool isValidShipMapFileName(const string& fileName);
bool isValidShipRouteFileName(const string& fileName);
bool isValidTravelName(const string& travelName);
bool isValidPortName(const string& portName);
bool isValidInteger(const string& str);
bool isCommentLine(const string& line);
bool isValidId(const string& str);
bool validateContainerData(const string& line, VALIDATION& reason, string& id, std::unique_ptr<Ship>& ship, std::array<bool,NUM_OF_ERRORS>& errorCodes);
bool idExistOnShip(const string& id, std::unique_ptr<Ship>& ship);
bool isPortInRoute(const string& portName, const vector<std::shared_ptr<Port>>& route, int portNum);

/*----------------------Rest of the functions-------------------*/
vector<string> stringSplit(string s, const char* delimiter);
void writeToOutput(std::ofstream& output,
                   AbstractAlgorithm::Action command, const std::string& id,
                   const std::tuple<int,int,int> pos = std::forward_as_tuple(-1,-1,-1),
                   const std::tuple<int,int,int>& movedTo = std::forward_as_tuple(-1,-1,-1));
void initArrayOfErrors(std::array<bool,NUM_OF_ERRORS> &arr,int num);
std::unique_ptr<Container> createContainer(SimulatorObj* sim,map<string,list<string>> &rawData,string& id, string& instruction,string& srcPortName);
void trimSpaces(string& toTrim);

#endif

