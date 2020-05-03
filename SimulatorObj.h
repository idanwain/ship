
#ifndef SIMULATOROBJ_H
#define SIMULATOROBJ_H

#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "AbstractAlgorithm.h"
#include "common.h"

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

#define NUM_OF_SIM_ERRORS 2
#define NUM_OF_ERRORS 19

class SimulatorObj {

    map<string,map<string,pair<int,int>>> outputResultsInfo;
    map<string,map<string,list<string>>> outputErrorsInfo;
    map<string,map<string,vector<fs::path>>> inputFiles;
    map<string,pair<int,int>> algInfo; /*Including algorithm name, pair<instructions count, errors count>*/
    map<string,list<string>> currTravelErrors; /*Including algorithm name, list of errors found by simulator*/
    list<string> currTravelGeneralErrors;/*General errors per certain travel*/
    std::array<bool,NUM_OF_ERRORS> algErrorCodes{false};
    std::array<bool,NUM_OF_ERRORS> simErrorCodes{false};
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
    void addErrorsInfo(string &travelName);
    void addResultsInfo(string &travelName);
    void addOutputInfo(string& travelName);
    void createResultsFile(string path);
    void createErrorsFile(string path);
    void runCurrentAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, string &travelName);
    void runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                                      list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber);
    void addNewTravelListErrors(list<string> &listErrors,string errListName);
    void addNewErrorToGeneralErrors(string msg);
    void updateArrayOfCodes(int num, string type);
    void resetOutputLists();
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
