#include "SimulatorObj.h"

/**
 * This function sets the ship map of the simulator and the calculator ship map
 */
void SimulatorObj::setShipAndCalculator(std::unique_ptr<Ship> &getShip,const string& file_path){
    this->simShip = std::make_unique<Ship>(*getShip);
    simCalc.readShipPlan(file_path);
}


/**
 * This function iterates over the travel cargo_data files and checks if there are ports
 * with cargo_data files that doesn't exist in the route.
 * also it compares the number of cargo files in this travel vs the route length and notify
 * if num of cargo files > route length.
 */
void SimulatorObj::compareRoutePortsVsCargoDataPorts(std::unique_ptr<Ship> &ship, std::shared_ptr<Travel> &travel){
    auto vecPorts = ship->getRoute();
    int numOfCargoFiles = 0;
    map<string,vector<fs::path>> travelCargoDataFiles(travel->getMap());
    /*Erase all ports that exist in the travel map as their cargo files are necessary*/
    for(auto &port : vecPorts){
        if(travelCargoDataFiles.find(port->get_name()) != travelCargoDataFiles.end())
            travelCargoDataFiles.erase(port->get_name());
    }
    for(auto &pair : travelCargoDataFiles)
        travel->setNewGeneralError(ERROR_NO_PORT_EXIST_IN_TRAVEL(pair.first));

    /*Count how many valid cargo files exist in this travel folder*/
    for(auto &pair : travel->getMap())
        numOfCargoFiles += (int)pair.second.size();

    if(numOfCargoFiles > (int)ship->getRoute().size())
        travel->setNewGeneralError(ERROR_TOO_MANY_CARGO_FILES);
}

std::unique_ptr<Ship>& SimulatorObj::getShip(){
    return this->simShip;
}

/**
 * This function runs the current algorithm on the current travel
 */
void SimulatorObj::runAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, std::shared_ptr<Travel> &travel){
    list<string> simCurrAlgErrors;
    map<string,int> visitNumbersByPort;
    int res = 0;
    vector<std::shared_ptr<Port>> route = simShip->getRoute();
    res = checkIfFatalErrorOccurred("alg");
    if(res != -1) {
        string algInstructionsFolder = SimulatorObj::createAlgorithmOutDirectory(alg.first, travel->getOutputPath(),travel->getName());
        for (int portNum = 0; portNum < (int) route.size() && res != -1; portNum++) {
            string portName = route[portNum]->get_name();
            pPort = simShip->getPortByName(portName);
            currPortNum = portNum;
            int visitNumber = visitNumbersByPort[portName];
            fs::path portPath = getPathOfCurrentPort(travel,portName,visitNumber);
            res = runCurrentPort(portName, portPath, alg, simCurrAlgErrors, algInstructionsFolder,
                                 ++visitNumbersByPort[portName],travel);
            compareIgnoredAlgErrsVsSimErrs(portName, visitNumber, simCurrAlgErrors);
        }
    }
    /*Case there's a fatal error indicated by the algorithm and not by the simulator*/
    else{
        std::get<1>(travel->getAlgResultsMap()[alg.first]) = -1;
    }
    compareFatalAlgErrsVsSimErrs(simCurrAlgErrors);
    travel->getErrorsMap().insert(make_pair(alg.first,simCurrAlgErrors));
    prepareNextIteration();
}

/**
 * This function gets the full file path of the given port in the given travel at the X time we visit there
 * Note* some ports in the ship route might not have a files,then it creates an empty path for them.
 */
fs::path SimulatorObj::getPathOfCurrentPort(std::shared_ptr<Travel> &travel, string& portName, int visitNumber){
    auto &vec = travel->getMap()[portName];
    fs::path result;
    if(vec.empty()) {
        vec.resize(1);
        result = fs::path();
    }
    else{
        try{
            result = vec.at(visitNumber);
        }
        catch(const std::exception& e){
            result = fs::path();
        }
    }
    /*Case there is no file --> create empty file for algorithm*/
    if(result.empty()){
        string newPath = travel->getPlanPath().parent_path().string() + PATH_SEPARATOR + portName + "_" + std::to_string(visitNumber+1) + ".cargo_data";
        result = fs::path(newPath);
        std::ofstream inFile;
        inFile.open(newPath);
        inFile.close();
    }
    return result;
}

int SimulatorObj::checkIfFatalErrorOccurred(string type){
    if(type == "alg" && (algErrorCodes[3] || algErrorCodes[4] || algErrorCodes[7] || algErrorCodes[8]))
        return -1;
    else if(simErrorCodes[3] || simErrorCodes[4] || simErrorCodes[7] || simErrorCodes[8])
        return -1;
    else
        return 0;
}

/**
 *  This function compares the fatal errors between algorithm and simulator
 */
void SimulatorObj::compareFatalAlgErrsVsSimErrs(list<string> &simCurrAlgErrors){
    if(algErrorCodes[3] != simErrorCodes[3])
        simCurrAlgErrors.emplace_back(ERROR_PLAN_FATAL);
    if(algErrorCodes[4] != simErrorCodes[4])
        simCurrAlgErrors.emplace_back(ERROR_DUPLICATE_XY);
    if(algErrorCodes[7] != simErrorCodes[7])
        simCurrAlgErrors.emplace_back(ERROR_TRAVEL_FATAL);
    if(algErrorCodes[8] != simErrorCodes[8])
        simCurrAlgErrors.emplace_back(ERROR_TRAVEL_SINGLEPORT);
}

/**
 * This function compares ignored errors between algorithm and simulator
 */
void SimulatorObj::compareIgnoredAlgErrsVsSimErrs(string &portName,int visitNumber,list<string> &simCurrAlgErrors){
    if(algErrorCodes[16] != simErrorCodes[16])
        simCurrAlgErrors.emplace_back(ERROR_NO_CARGO_TOLOAD(portName, visitNumber));
}

/**
 * This function runs the current algorithm over the current port at the travel route
 */
int SimulatorObj::runCurrentPort(string &portName, fs::path &portPath, pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                                 list<string> &simCurrAlgErrors, string &algOutputFolder, int visitNumber,
                                 std::shared_ptr<Travel> &travel){

    string inputPath,outputPath;
    int instructionsCount, errorsCount, algReturnValue;
    std::optional<pair<int,int>> result;
    pair<int,int> intAndError;
    SimulatorValidation validator(this);
    inputPath =  portPath.string();
    outputPath = algOutputFolder + PATH_SEPARATOR + portName + "_" + std::to_string(visitNumber) + ".crane_instructions";

    try {
        algReturnValue = alg.second->getInstructionsForCargo(inputPath,outputPath);
    }
    catch(...){
        simCurrAlgErrors.emplace_back(ERROR_ALG_FAILED);
        return -1;
    }
    updateErrorCodes(algReturnValue, "alg");
    result = validator.validateAlgorithm(outputPath,inputPath,simCurrAlgErrors,portName,visitNumber);
    if(!result) return -1; //case there was an error in validateAlgorithm

    /*Incrementing the instructions count and errors count*/
    intAndError = result.value();
    instructionsCount = std::get<0>(intAndError);
    errorsCount = std::get<1>(intAndError);
    if(travel->getAlgResultsMap().find(alg.first) == travel->getAlgResultsMap().end())
        travel->getAlgResultsMap().insert(make_pair(alg.first,pair<int,int>()));
    std::get<0>(travel->getAlgResultsMap()[alg.first]) += instructionsCount;
    std::get<1>(travel->getAlgResultsMap()[alg.first]) += errorsCount;

    this->pPort->getContainerVec(Type::PRIORITY)->clear();
    this->pPort->getContainerVec(Type::LOAD)->clear();
    return errorsCount;
}


/**
 * This function updates the error codes
 */
void SimulatorObj::updateErrorCodes(int num, string type){
    std::array<bool,NUM_OF_ERRORS> numArr{false};
    initArrayOfErrors(numArr,num);
    for(int i = 0; i < NUM_OF_ERRORS; i++){
        if(numArr[i] && type == "alg")
            this->algErrorCodes[i] = true;
        else if(numArr[i] && type == "sim"){
            this->simErrorCodes[i] = true;
        }
    }
}

/**
 * This function reset simulator parameters to next travel
 */
void SimulatorObj:: prepareNextIteration() {
    this->algErrorCodes = std::array<bool,NUM_OF_ERRORS>{false};
    this->simErrorCodes = std::array<bool,NUM_OF_ERRORS>{false};
    this->currPortNum = 0;
}

WeightBalanceCalculator SimulatorObj::getCalc() {
    return simCalc;
}

/**
 * This function sorts the algorithms output info list and assigning the order to the algorithms list
 * such that:
 * 1.sorting is based on --> 0 errors count(0 before -1..), then lowest instructions count
 */
void SimulatorObj::sortAlgorithmsForResults(map<string,map<string,pair<int,int>>>& outputInfo, list<string> &algorithm){
    map<string,pair<int,int>> algAndValues;
    vector<std::tuple<string,int,int>> sortedVec;
    for(auto &alg : algorithm){
        algAndValues.insert(make_pair(alg,pair<int,int>()));
    }
    /*Gather the overall score of travel X algorithms*/
    for(auto &algName : algorithm){
        for(auto &travel : outputInfo){
            if(travel.second.find(algName) != travel.second.end()){
                algAndValues[algName].first += travel.second[algName].first;
                algAndValues[algName].second += travel.second[algName].second;
            }
        }
    }
    for(auto &alg: algAndValues){
        std::tuple<string,int,int> tup;
        std::get<0>(tup) = alg.first;
        std::get<1>(tup) = alg.second.first;
        std::get<2>(tup) = alg.second.second;
        sortedVec.emplace_back(tup);
    }
    /*first will occur algorithms with 0 errors and lowest instructions*/
    std::sort(sortedVec.begin(),sortedVec.end(),[](const std::tuple<string,int,int> &t1,const std::tuple<string,int,int> &t2)
            ->bool {
        if(std::get<2>(t1) == std::get<2>(t2)){
            return std::get<1>(t1) < std::get<1>(t2);
        } else
            return std::get<2>(t1) > std::get<2>(t2);
    });
    algorithm.clear();
    for(auto& alg : sortedVec){
        algorithm.emplace_back(std::get<0>(alg));
    }

}

/**
 * This function create an algorithm output directory per algorithm and travel in the main output directory
 */
string SimulatorObj::createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName){
    string algOutDirectory = outputDirectory + PATH_SEPARATOR + algName + "_" + travelName + "_" + "crane_instructions";
    fs::path directoryPath(algOutDirectory);
    fs::path parentDirectory(outputDirectory);
    if(!fs::exists(directoryPath)){
        fs::create_directory(directoryPath,parentDirectory);
    }
    return algOutDirectory;
}

std::shared_ptr<Port> SimulatorObj::getPort() {
    return pPort;
}

/**
 * This function sorts the given vector of containers by the it's distance from it's destination
 * first occurences will be containers with lowest distance...
 */
void SimulatorObj::sortContainersByPriority(vector<Container>* &priorityVec){
    auto routeVec = this->getShip()->getRoute();
    map<string,int> portNamePriority;
    for(int i = currPortNum+1; i < (int)routeVec.size(); i++){
        if(portNamePriority.find((*routeVec[i]).get_name()) == portNamePriority.end())
            portNamePriority.insert({routeVec[i]->get_name(),i});
    }
    std::sort(priorityVec->begin(),priorityVec->end(),[&portNamePriority](Container& cont1,Container& cont2) -> bool
    {
        string cont1PortDst = cont1.getDest()->get_name();
        string cont2PortDst = cont2.getDest()->get_name();
        if(portNamePriority.find(cont1PortDst) == portNamePriority.end())
            portNamePriority.insert({cont1PortDst,INT_MAX});
        if(portNamePriority.find(cont2PortDst) == portNamePriority.end())
            portNamePriority.insert({cont2PortDst,INT_MAX});
        return portNamePriority[cont1PortDst] < portNamePriority[cont2PortDst];

    });

}

int SimulatorObj::getPortNum(){
    return this->currPortNum;
}
