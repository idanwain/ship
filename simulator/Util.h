
#ifndef WINDOWSSHIP_UTIL_H
#define WINDOWSSHIP_UTIL_H

/**
* This header holds helper functions of the main, including the results logger
*
*/


#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../common/Common.h"
#include <algorithm>
#include "Travel.h"


#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

/*------------------------------Global Variables---------------------------*/

extern string mainTravelPath;
extern string mainAlgorithmsPath;
extern string mainOutputPath;
extern int threadNum;


using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;

typedef std::vector<std::tuple<std::shared_ptr<Travel>,std::string,std::unique_ptr<AbstractAlgorithm>>> tasksContainer;

void insertPortFile(std::unique_ptr<Travel> &currTravel,string &portName, int portNum, const fs::path &entry);
void initListOfTravels(string &path, list<string> &generalErrors, vector<std::shared_ptr<Travel>> &TravelsVec,string mainOpPath);
void initOutputMap(map<string,map<string,pair<int,int>>>& outputMap,vector<std::shared_ptr<Travel>> &TravelsVec);
void createResultsFile(string& mainOutputPath, vector<std::shared_ptr<Travel>> &TravelsVec);
void createErrorsFile(string& mainOutputPath, vector<std::shared_ptr<Travel>> &TravelsVec, list<string> &generalErrors);
bool isResultsEmpty(vector<std::shared_ptr<Travel>> &TravelsVec);
bool isErrorsEmpty(vector<std::shared_ptr<Travel>> &TravelsVec,list<string> &generalErrors);
void initTasksContainer(tasksContainer& tasks,map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>> &map,vector<std::shared_ptr<Travel>>& TravelsVec);
void getAlgSoFiles(vector<fs::path> &algPaths);
void handleFlags(int argc, char** argv);
vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> initAlgorithmList(map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map);





#endif
