
#ifndef TRAVEL_H
#define TRAVEL_H

#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>

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
    map<string,vector<fs::path>> portPaths;
public:
    Travel(string travelName,map<string,vector<fs::path>> paths):name(travelName),portPaths(paths){}
    string getName();

};


#endif //MYSHIP_TRAVEL_H
