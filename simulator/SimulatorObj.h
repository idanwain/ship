#ifndef SIMULATOROBJ_H
#define SIMULATOROBJ_H

/**
* This header is a module of the functions used by the simulator to validate the algortihms instructions
*
*      *******      Functions      *******
* setShipAndCalculator          - sets the ship of the simulator and the calculator
* createResultsFile             - creates the results file simulation.results
* createErrorsFile              - creates the errors file simulation.errors
* runAlgorithm                  - run the current algorithm on current travel
* runCurrentPort                - run the current algorithm in the current travel on the current port
* updateErrorCodes              - updates sim array of codes or alg array of codes
* prepareNextIteration          - reset the relevant data members to next iteration
* checkIfFatalErrorOccurred     - checks if fatal error occures in simulator
* compareFatalAlgErrsVsSimErrs  - comparing alg array and sim array
* compareIgnoredAlgErrsVsSimErrs    - comparing alg array and sim array
* sortContainersByPriority          - sort the loaded list containers by priority of destination distance
* insertPortFile                    - inserts the port file to travel
* createAlgorithmOutDirectory       - creates the algorithm output directory
* getPathOfCurrentPort              - getting the path of the current port cargo_data file
* sortAlgorithmsForResults          - sorts the algorithm results list
* compareRoutePortsVsCargoDataPorts - comparing amount of route ports vs amount of cargo_data files
 */

class Common;
class SimulatorValidation;

#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/Common.h"
#include <algorithm>
#include "../interfaces/WeightBalanceCalculator.h"
#include "Travel.h"
#include "SimulatorValidation.h"



#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif


using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;

#define NUM_OF_ERRORS 19

class SimulatorObj {

    std::array<bool,NUM_OF_ERRORS> algErrorCodes{false};
    std::array<bool,NUM_OF_ERRORS> simErrorCodes{false};
    std::unique_ptr<Ship> simShip = nullptr;
    std::shared_ptr<Port> pPort = nullptr; /*holds the current port*/
    WeightBalanceCalculator simCalc;
    string mainOutputPath;
    int currPortNum = 0;

public:
    SimulatorObj(){};
    SimulatorObj(string mainOutputPath):mainOutputPath(mainOutputPath){};
    void setShipAndCalculator(std::unique_ptr<Ship> &getShip,const string& file_path);
    void runAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, std::shared_ptr<Travel> &travel);
    int  runCurrentPort(string &portName, fs::path &portPath, pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                        list<string> &simCurrAlgErrors, string &algOutputFolder, int visitNumber,
                        std::shared_ptr<Travel> &travel);
    void updateErrorCodes(int num, string type);
    void prepareNextIteration();
    int  checkIfFatalErrorOccurred(string type);
    void compareFatalAlgErrsVsSimErrs(list<string> &simCurrAlgErrors);
    void compareIgnoredAlgErrsVsSimErrs(string &portName, int visitNumber, list<string> &simCurrAlgErrors);
    void sortContainersByPriority(vector<Container>* &priorityVec);
    void addAlgErrCodes(list<string>& simCurrAlgErrors);
    int getPortNum();
    WeightBalanceCalculator getCalc();
    std::unique_ptr<Ship>& getShip();
    std::shared_ptr<Port> getPort();
    /*----------------------static functions-------------------*/
    static string createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName);
    static fs::path getPathOfCurrentPort(std::shared_ptr<Travel> &travel, string& portName, int visitNumber);
    static void sortAlgorithmsForResults(map<string,map<string,pair<int,int>>>&outputInfo, list<string> &algorithm);
    static void compareRoutePortsVsCargoDataPorts(std::unique_ptr<Ship>& ship, std::shared_ptr<Travel> &travel);

};



#endif