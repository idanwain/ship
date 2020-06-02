#include "Travel.h"

const string& Travel::getName() {
    return this->name;
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

bool Travel::isErroneous() {
    return this->erroneousTravel;
}

void Travel::setErroneousTravel() {
    this->erroneousTravel = true;
}

bool Travel::isErrorsExists() {
    for(auto& msg: this->generalErrors){
        if(msg.empty()){}
        return true;
    }
    for(auto& pair : this->errors){
        for(auto& msg : pair.second){
            if(msg.empty()){}
            return true;
        }
    }
    return false;
}

void Travel::setAlgCrashError(string &algName){
    list<string> lst;
    lst.emplace_back(ERROR_ALG_PLAN);
    this->errors.insert({algName,lst});
    this->algorithmsResults.insert({algName, {0, -1}});
}

bool Travel::isShipInit() {
    return initShip;
}

void Travel::setInitShip() {
    initShip = true;
}

std::unique_ptr<Ship>& Travel::getShip() {
    return mainShip;
}

std::string &Travel::getOutputPath() {
    return this->mainOutputPath;
}




