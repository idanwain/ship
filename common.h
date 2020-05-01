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
#include <cmath>

enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};
/**
 * enum class for error codes as pre defined between algorithms and simulator
 * C - is identifier for containers at port
 * reserved - for future use if necessary
 */
enum class errorCodes {
    Plan_ZError     = (1 << 0), /*Error at ship plan Z axis*/
    Plan_XYError    = (1 << 1), /*Error at ship plan xy axis*/
    Plan_BadLine    = (1 << 2), /*Error bad line reading*/
    Plan_Fatal      = (1 << 3), /*Fatal Error in ship plan file*/
    Reserved        = (1 << 4),
    Route_PortTwice = (1 << 5), /*Port occurs 2 or more consecutive times*/
    Route_badPortN  = (1 << 6), /*Bad port Number*/
    Route_Fatal     = (1 << 7), /*Fatal Error in route file*/
    Route_SingleP   = (1 << 8), /*Single Port in the Route*/
    Reserved2       = (1 << 9),
    C_DuplicateId   = (1 << 10),/*2 containers with same Id*/
    C_IdAlreadyOn   = (1 << 11),/*Id already on ship*/
    C_WeightIssue   = (1 << 12),/*Any weight issue, bad weight/missing*/
    C_PortIssue     = (1 << 13),/*Missing port,port not in dest*/
    C_IdCantRead    = (1 << 14),/*Cant read Id??*/
    C_IdIllegal     = (1 << 15),/*Id is not illegal ISO 6346*/
    C_FileCantRead  = (1 << 16),/*Assuming no cargo to load at this port*/
    C_LastPortCont  = (1 << 17),/*Last port has awaiting containers*/
    C_ExceedsCap    = (1 << 18) /*total containers amount exceeds ship cap*/

};

/*----------------------Validate functions-------------------*/
bool isValidPortName(const string& portName);
bool validateId(const string& str);
std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &inputPath, std::unique_ptr<Ship>& simShip, int portNumber, list<string>& currAlgErrors);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,std::unique_ptr<Ship>& ship,std::map<string,string> &portContainers,int portNum);
bool validateLoadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship);
bool validateUnloadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship);
bool validateMoveInstruction(vector<int> &coordinates, vector<vector<vector<Container>>>& map);
bool validateRejectInstruction(std::map<string,string>& portContainers, string& id,std::unique_ptr<Ship>& ship,int portNum);
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
                   const std::string& command, const std::string& id,
                   const std::tuple<int,int,int>& pos, const std::tuple<int,int,int>& movedTo);

#endif
