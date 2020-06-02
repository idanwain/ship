/**
* This header represents a module that involves in reading and parsing(extracting) files or information given in
* any constellation.
*
*      *******      Functions      *******
* extractPortNumFromFile    - extracts the number from port file
* extractArgsForBlocks      - extracts the arguments for container blocks
* extractCraneInstruction   - extracts crane instruction from crane instruction file
* extractContainersData     - extracts container data from cargo_data file line
* extractTravelRoute        - extracts the travel route parameters
* extractArgsForShip        - extracts the whole arguments to build ship map
* setBlocksByLine           - setting container blocks by line from file
* getDimensions             - getting the dimensions from a line
* portAlreadyExist          - checks if port already occured previously
* extractRawDataFromPortFile - extracts the whole data from the cargo_data file into map
* parseDataToPort           - parsing the data from cargo_data file
* extractShipPlan           - extracts the ship plan
* extractPortNameFromFile   - extracts port name from port file
 */
#ifndef PARSER_HEADER
#define PARSER_HEADER

class Common;

#define NUM_OF_ERRORS 19


#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Common.h"
#include "../interfaces/ErrorsInterface.h"
#include "../simulator/Travel.h"

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;


int extractPortNumFromFile(const string& fileName);
int extractArgsForBlocks(std::unique_ptr<Ship>& ship, const std::string& file_path, std::shared_ptr<Travel> *travel);
int extractArgsForBlocks(std::unique_ptr<Ship>& ship,const std::string& filePath);
void extractCraneInstruction(string &toParse, string& instruction, string& id, vector<int> &coordinates);
void extractContainersData(const std::string& line, std::string &id, int &weight, std::shared_ptr<Port>& dest, std::unique_ptr<Ship>& ship);
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath);//Overload
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath, std::shared_ptr<Travel> *travel);
std::unique_ptr<Ship> extractArgsForShip(std::shared_ptr<Travel> &travel, SimulatorObj &simulator);
pair<string,int> setBlocksByLine(std::string &str, std::unique_ptr<Ship> &ship,int lineNumber);
void getDimensions(std::array<int,3> &arr, std::istream &inFile,string str);
int portAlreadyExist(std::vector<std::shared_ptr<Port>>& vec,string &str);
void extractRawDataFromPortFile(std::map<string,list<string>>& map, string &inputPath,SimulatorObj* sim);
bool parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output,
                     std::unique_ptr<Ship>& ship, std::shared_ptr<Port>& port, std::set<std::string>& idSet, std::array<bool,NUM_OF_ERRORS>& errorCodes, bool lastPort);
int extractShipPlan(const std::string& filePath, std::unique_ptr<Ship>& ship);
string extractPortNameFromFile(const string& fileName);

#endif