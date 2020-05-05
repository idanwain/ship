
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
#include "Common.h"
#include <algorithm>

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

    map<string,map<string,pair<int,int>>> outputResultsInfo;
    map<string,map<string,list<string>>> outputErrorsInfo;
    map<string,map<string,vector<fs::path>>> inputFiles;
    map<string,pair<int,int>> algInfo; /*Including algorithm name, pair<instructions count, errors count>*/
    map<string,list<string>> currTravelErrors; /*Including algorithm name, list of errors found by simulator*/
    list<string> currTravelGeneralErrors;/*General errors per certain travel*/
    std::array<bool,NUM_OF_ERRORS> algErrorCodes{false};
    std::array<bool,NUM_OF_ERRORS> simErrorCodes{false};
    std::unique_ptr<Ship> simShip = nullptr;
    WeightBalanceCalculator simCalc;

    string mainOutputPath;
    string mainTravelPath;
public:
    SimulatorObj(string mainTravelPath, string outputPath): mainOutputPath(outputPath), mainTravelPath(mainTravelPath){
        initListOfTravels(mainTravelPath);
    };
    static void insertPortFile(map<string,vector<fs::path>> &travelMap,string &portName, int portNum, const fs::path &entry);
    void initListOfTravels(string &path);
    void initCalc(const string& file_path);
    void setShip(std::unique_ptr<Ship> &getShip);
    void addErrorsInfo(string &travelName);
    void addResultsInfo(string &travelName);
    void addOutputInfo(string& travelName);
    void createResultsFile(string path);
    void createErrorsFile(string path);
    void runCurrentAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, string &travelName);
    int  runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                                      list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber);
    void addNewTravelListErrors(list<string> &listErrors,string errListName);
    void addNewErrorToGeneralErrors(string msg);
    void addListOfGeneralErrors(list<string> &generalErrors);
    void updateArrayOfCodes(int num, string type);
    void prepareForNewTravel();
    void createErrorsFromArray();
    int checkIfFatalErrorOccurred();
    static int checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string& portName, int visitNum);
    void compareFatalAlgErrsAndSimErrs(list<string> &simCurrAlgErrors);
    void compareIgnoredAlgErrsVsSimErrs(string &portName, int visitNumber, list<string> &simCurrAlgErrors);
    WeightBalanceCalculator getCalc();
    map<string,map<string,list<string>>>& getErrorsInfo();
    map<string,map<string,pair<int,int>>>& getResultsInfo();
    map<string,map<string,vector<fs::path>>>& getInputFiles();
    std::array<bool,NUM_OF_ERRORS>& getCommonErrors();
    std::array<bool,NUM_OF_ERRORS>& getSimErrors();
    std::unique_ptr<Ship>& getShip();
};

/*----------------------Non Simulator object functions-------------------*/
string createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName);
void sortAlgorithms(map<string,map<string,pair<int,int>>> &outputInfo,list<string> &algorithm);



#endif
