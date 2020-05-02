
#ifndef SIMULATOROBJ_H
#define SIMULATOROBJ_H

#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include "ship.h"
#include <fstream>
#include "AbstractAlgorithm.h"
#include "common.h"
#include "outputHandler.h"

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;
#define NUM_OF_ERRORS 19
#define NUM_OF_SIM_ERRORS 2

class SimulatorObj {

    map<string,map<string,pair<int,int>>> outputResultsInfo;
    map<string,map<string,list<string>>> outputErrorsInfo;
    map<string,map<string,vector<fs::path>>> inputFiles;
    map<string,list<string>> currTravelErrors;
    list<strings> currTravelGeneralErrors;
    std::array<bool,NUM_OF_ERRORS> commonErrorCodes{false};
    std::array<bool,NUM_OF_SIM_ERRORS> simErrorCodes{false};
    std::unique_ptr<Ship> simShip = nullptr;

    string mainOutputPath;
    string mainTravelPath;
public:
    SimulatorObj(string mainTravelPath, string outputPath): mainOutputPath(outputPath), mainTravelPath(mainTravelPath){
        initListDirectories(mainTravelPath);
    };
    static void insertPortFile(map<string,vector<fs::path>> &travelMap,string &portName, int portNum, const fs::path &entry);
    void initListDirectories(string &path);
    void setShip(std::unique_ptr<Ship> &getShip);
    void addErrorsInfo(string &travelName,map<string,list<string>> &ErrorsToAdd);
    void addResultsInfo(string &travelName,map<string,pair<int,int>> &algInfo);
    void addOutputInfo(string& travelName,map<string,pair<int,int>> &algResultInfo,map<string,list<string>> &ErrorsToAdd);
    void createResultsFile(string path);
    void createErrorsFile(string path);
    void runCurrentAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,string &travelName,map<string,list<string>> &simCurrTravelErrors,map<string,pair<int,int>> &algInfo);
    void runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                                      list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber,map<string,pair<int,int>> &algInfo);
    void addNewTravelListErrors(list<string> &listErrors,string errListName);
    void addNewErrorToGeneralErrors(string msg);
    void clearCurrTravelErrorsList();
    void updateSimulatorArrayOfCodes(int num);
    map<string,map<string,list<string>>>& getErrorsInfo();
    map<string,map<string,pair<int,int>>>& getResultsInfo();
    map<string,map<string,vector<fs::path>>>& getInputFiles();
    std::array<bool,NUM_OF_ERRORS>& getCommonErrors();
    std::array<bool,NUM_OF_SIM_ERRORS>& getSimErrors();
    std::unique_ptr<Ship>& getShip();


};

/*----------------------Non Simulator object functions-------------------*/
string createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName);

enum simErrorCodes {
    S_NoPortFile    = (1 << 0),/*Simulator didn't find PortFile*/
    S_NoRouteFile   = (1 << 1) /*Simulator didn't find RouteFile*/
};

#endif
