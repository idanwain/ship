
#ifndef TRAVEL_H
#define TRAVEL_H

#include <string>
#include <utility>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../interfaces/ErrorsInterface.h"

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::map;
namespace fs = std::filesystem;

class Travel {

    string name;
    fs::path routePath;
    fs::path planPath;
    list<string> generalErrors;
    map<string,vector<fs::path>> portPaths;
    map<string,list<string>> errors;
    map<string,pair<int,int>> algorithmsResults; /*first int --> instructions count, second int --> errors count*/
    bool erroneousTravel = false;

public:
    explicit Travel(const string travelName):name(travelName){};
    void setRoutePath(const fs::path& route);
    void setPlanPath(const fs::path& plan);
    void setNewGeneralError(const string& msg);
    void setErroneousTravel();
    const string& getName();
    map<string,vector<fs::path>>& getMap();
    map<string,list<string>>& getErrorsMap();
    map<string,pair<int,int>>& getAlgResultsMap();
    list<string>& getGeneralErrors();
    fs::path& getRoutePath();
    fs::path& getPlanPath();
    bool isErroneous();
    bool isErrorsExists();
    void setAlgCrashError(string &algName);
};


#endif
