#include "Travel.h"

const string& Travel::getName() {
    this->name;
}

void Travel::setPlanPath(const fs::path& plan) {
    this->planPath = plan;
}

void Travel::setRoutePath(const fs::path& route) {
    this->routePath = route;
}


void Travel::setNewGeneralError(const string& msg){
    this->generalErrors.emplace_back((msg));
}
fs::path& Travel::getRoutePath(){
    return this->routePath;
}

fs::path& Travel::getPlanPath(){
    return this->planPath;
}

map<string,vector<fs::path>>& Travel::getMap(){
    return this->portPaths;
}

map<string,list<string>>& Travel::getErrorsMap() {
    return this->errors;
}

list<string>& Travel::getGeneralErrors(){
    return this->generalErrors;
}

map<string,pair<int,int>>& Travel::getAlgResultsMap(){
    return this->algorithmsResults;
}
