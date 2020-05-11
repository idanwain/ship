
#ifndef SIMULATOROBJ_H
#define SIMULATOROBJ_H

class Common;

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

    vector<std::unique_ptr<Travel>> TravelsVec;
    std::array<bool,NUM_OF_ERRORS> algErrorCodes{false};
    std::array<bool,NUM_OF_ERRORS> simErrorCodes{false};
    std::unique_ptr<Ship> simShip = nullptr;
    std::shared_ptr<Port> pPort; /*holds the current port*/
    WeightBalanceCalculator simCalc;
    string mainOutputPath;
    string mainTravelPath;

public:
    SimulatorObj(string mainTravelPath, string outputPath): mainOutputPath(outputPath), mainTravelPath(mainTravelPath){
        initListOfTravels(mainTravelPath);
    };
    void initListOfTravels(string &path);
    void setShipAndCalculator(std::unique_ptr<Ship> &getShip,const string& file_path);
    void createResultsFile(string path);
    void createErrorsFile(string path);
    void runCurrentAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, std::unique_ptr<Travel> &travel);
    int  runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                                      list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber,std::unique_ptr<Travel> &travel);
    void updateArrayOfCodes(int num, string type);
    void prepareForNewTravel();
    void createErrorsFromArray();
    int  checkIfFatalErrorOccurred(string type);
    void compareFatalAlgErrsAndSimErrs(list<string> &simCurrAlgErrors);
    void compareIgnoredAlgErrsVsSimErrs(string &portName, int visitNumber, list<string> &simCurrAlgErrors);
    void initOutputMap(map<string,map<string,pair<int,int>>>& outputMap);
    bool isResultsEmpty();
    bool isErrorsEmpty();
    WeightBalanceCalculator getCalc();
    vector<std::unique_ptr<Travel>>& getTravels();
    std::array<bool,NUM_OF_ERRORS>& getCommonErrors();
    std::array<bool,NUM_OF_ERRORS>& getSimErrors();
    std::unique_ptr<Ship>& getShip();
    std::shared_ptr<Port> getPort();

/*----------------------static functions-------------------*/
    static int checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string& portName, int visitNum);
    static void insertPortFile(std::unique_ptr<Travel> &currTravel,string &portName, int portNum, const fs::path &entry);
    static string createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName);
    static fs::path getPathOfCurrentPort(std::unique_ptr<Travel> &travel,string& portName,int visitNumber);
    static void sortAlgorithms(map<string,map<string,pair<int,int>>>&outputInfo,list<string> &algorithm);
    static void compareRoutePortsVsCargoDataPorts(std::unique_ptr<Ship>& ship,std::unique_ptr<Travel> &travel);



};




#endif
