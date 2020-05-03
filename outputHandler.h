/**
* This header represents a module that handles the all interactions
* that including writing to files, and it's utility functions.
*
*      *******      Functions      *******
* saveResultsInfo          - saves given information to the data structure that holds the results information
* saveErrorsInfo           - saves given information to the data structure that holds the errors information
* createAlgListOfErrors    - saves all errors occurred in an algorithm certain run to a list
* saveOutputInformation    - this is an manging function that calls save results and save info functions
* createResultsFile        - writes all the information that the above functions gathered through the simulation
* createErrorsFile         - write all the information that the above functions gathered through the simulation
* createOutPutDirectories  - creating the out put directories
*
*/
#ifndef OUTPUTHANDLER_HEADER
#define OUTPUTHANDLER_HEADER

#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "ship.h"
#include <fstream>
#include <map>
#include <set>
#include "stowage_algorithm.h"
#include "lifo_algorithm.h"


using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;

void saveResultsInfo(std::map<string,list<int>> &results_map,vector<std::unique_ptr<AbstractAlgorithm>>& algVec);
void createErrorsFile(std::map<string,std::map<string,list<string>>> &simErrors,string path);
void createResultsFile(map<string,map<string,pair<int,int>>>& output_map,string path);

//list<string> createAlgListOfErrors(std::unique_ptr<AbstractAlgorithm>& alg);
void createOutputDirectories(vector<vector<fs::path>> &paths,char* mainDirectory);
void saveErrorsInfo(vector<pair<string,list<pair<string,list<string>>>>> &errors_vec,
                    vector<std::unique_ptr<AbstractAlgorithm>> &algVec,string &travelName);
void saveOutputInformation(std::map<string,list<int>> &results_map,
                           vector<pair<string,list<pair<string,list<string>>>>> &errors_vec,
                           vector<std::unique_ptr<AbstractAlgorithm>> &algVec, string &travelName);



#endif
