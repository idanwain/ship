
#ifndef WINDOWSSHIP_UTIL_H
#define WINDOWSSHIP_UTIL_H

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


using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;

void insertPortFile(std::unique_ptr<Travel> &currTravel,string &portName, int portNum, const fs::path &entry);
void initListOfTravels(string &path,list<string> &generalErrors,vector<std::unique_ptr<Travel>> &TravelsVec);
void initOutputMap(map<string,map<string,pair<int,int>>>& outputMap,vector<std::unique_ptr<Travel>> &TravelsVec);
void createResultsFile(string& mainOutputPath,vector<std::unique_ptr<Travel>> &TravelsVec);
void createErrorsFile(string& mainOutputPath,vector<std::unique_ptr<Travel>> &TravelsVec, list<string> &generalErrors);
bool isResultsEmpty(vector<std::unique_ptr<Travel>> &TravelsVec);
bool isErrorsEmpty(vector<std::unique_ptr<Travel>> &TravelsVec,list<string> &generalErrors);




#endif
