#include "SimulatorObj.h"

void SimulatorObj::setShipAndCalculator(std::unique_ptr<Ship> &getShip,const string& file_path){
    this->simShip = std::make_unique<Ship>(*getShip);
    simCalc.readShipPlan(file_path);
}


/**
 * This function iterates over the travel cargo_data files and checks if there are ports
 * with cargo_data files that doesn't exist in the route.
 * also it compares the number of cargo files in this travel vs the route length and notify
 * if num of cargo files > route length.
 * @param travelName - current travel
 */
void SimulatorObj::compareRoutePortsVsCargoDataPorts(std::unique_ptr<Ship> &ship,std::unique_ptr<Travel> &travel){
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


void SimulatorObj::initListOfTravels(string &path){
    string msg = " only sub folders allowed in main folder, file won't be included in the program";
    for(const auto &entry : fs::directory_iterator(path)){
        if(!entry.is_directory()){
            this->generalErrors.emplace_back(ERROR_NOT_DIRECTORY(entry, msg));
            continue;
        }
        string travelName = entry.path().filename().string();
        if(!isValidTravelName(travelName)) {
            this->generalErrors.emplace_back(ERROR_TRAVEL_NAME(travelName));
            continue;
        }
        std::unique_ptr<Travel> currTravel = std::make_unique<Travel>(travelName);
        map<string,vector<fs::path>> currPorts;
        for(const auto &deep_entry : fs::directory_iterator(entry.path())){
            string fileName = deep_entry.path().filename().string();
            if(isValidPortFileName(fileName)){
                string portName = extractPortNameFromFile(fileName);
                int portNum = extractPortNumFromFile(fileName);
                insertPortFile(currTravel,portName,portNum,deep_entry.path());
            }
            else if(isValidShipRouteFileName(fileName)){
                    if(!currTravel->getRoutePath().empty())
                        currTravel->setNewGeneralError(ERROR_ROUTE_MANY_FILES(fileName));
                    else
                        currTravel->setRoutePath(deep_entry.path());
            }
            else if(isValidShipMapFileName(fileName)){
                if(!currTravel->getPlanPath().empty())
                    currTravel->setNewGeneralError(ERROR_PLAN_MANY_FILES(fileName));
                else
                    currTravel->setPlanPath(deep_entry.path());
            }
            else{
                currTravel->setNewGeneralError(ERROR_INVALID_FILE(fileName));
            }
        }
        this->TravelsVec.emplace_back(std::move(currTravel));

    }
}

/**
 * This function get a travel folder map:= <portName,list of files with same portName>
 * and assigns at list[portNum] the given entry which corresponds to map[portName][portNum-1] --> portName_portNum.cargo_data
 * @param travelMap - the travel_name map
 * @param portName - the current portName file
 * @param portNum - the current portName number
 * @param entry - the entry path
 */
void SimulatorObj::insertPortFile(std::unique_ptr<Travel> &currTravel, string &portName, int portNum, const fs::path &entry) {
    auto &travelMap = currTravel->getMap();
    if(travelMap.find(portName) != travelMap.end()){
        if((int)travelMap[portName].size() < portNum){
            travelMap[portName].resize(portNum);
        }
    }
    else{/*case no such entry found in the map, then it's first time this portName occurs*/
        vector<fs::path> vec(portNum);
        travelMap.insert(make_pair(portName,vec));
    }

    travelMap[portName].at(portNum-1) = entry;
}


vector<std::unique_ptr<Travel>>& SimulatorObj::getTravels(){
    return this->TravelsVec;
}

std::unique_ptr<Ship>& SimulatorObj::getShip(){
    return this->simShip;
}

/**
 * This function creates a file that shows the result of the cartesian multiplication of the simulator
 * @param output_map - stores results information
 * @param travels - list of travel names
 * @param path - the file path to create the file in
 */
void SimulatorObj::createResultsFile(){
    std::ofstream inFile;
    int sumInstructions = 0,sumErrors = 0;
    const char comma = ',';
    list<string> travels;
    list<string> algorithmNames;
    string path = mainOutputPath;
    map<string,map<string,pair<int,int>>> output_map;

    if(isResultsEmpty()){
        NO_RESULT_FILE;
        return;
    }
    initOutputMap(output_map);
    path.append(PATH_SEPARATOR);
    path.append("simulation.results");
    inFile.open(path);
    if(inFile.fail()){
        ERROR_RESULTS_FILE;
        exit(EXIT_FAILURE);
    }
    for(auto &travel : this->TravelsVec) {//Get travel Names
        if(!travel->isErroneous())
            travels.emplace_back(travel->getName());
    }
    for(auto &algName : this->TravelsVec.front()->getAlgResultsMap())//Get algorithm names
        algorithmNames.emplace_back(algName.first);

    SimulatorObj::sortAlgorithmsForResults(output_map, algorithmNames);

    inFile << "RESULTS" << comma;
    for(string &travel_name : travels)
        inFile << travel_name << comma;
    inFile << "Sum" << comma << "Errors" <<  '\n';

    for(auto& algName : algorithmNames){
        inFile << algName << comma;
        for(auto &travelName : travels){
            if(output_map[travelName][algName].second == -1)
                inFile << output_map[travelName][algName].second << comma;
            else{
                inFile << output_map[travelName][algName].first << comma;
                sumInstructions += output_map[travelName][algName].first;
            }
            sumErrors += output_map[travelName][algName].second;
        }
        inFile << sumInstructions << comma << (sumErrors*-1) << '\n';
        sumInstructions = 0;
        sumErrors = 0;
    }
    inFile.close();
}

/**
 * This function creates a file that shows all the errors existed in the simulator run,
 * errors list - container didn't arrived to it's destination, container didn't picked up as the destination of
 * it isn't doesn't exist in any following ports
 * @param errors_vec - stores errors information
 * @param path - the file path to create the file in
 */
void SimulatorObj::createErrorsFile() {
    const string spaces = "      ";//6spaces
    const string lineSep = "====================================================================================================";
    std::ofstream inFile;
    string path = mainOutputPath;
    if(isErrorsEmpty()){
        NO_ERROR_FILE;
        return;
    }
    path.append(PATH_SEPARATOR);
    path.append("simulation.errors");
    inFile.open(path);
    if (inFile.fail()) {
        ERROR_ERRORS_FILE;
        return;
    }
    if(!generalErrors.empty()){
        inFile << "Simulator General Errors:" << '\n';
        for(auto &msg : generalErrors){
            inFile << spaces << msg << '\n';
        }
    }
    inFile << lineSep+lineSep << '\n';
    for(auto &travel : TravelsVec){
        if(!travel->getGeneralErrors().empty() || !travel->getErrorsMap().empty()){
            inFile << travel->getName() << " Errors:" << '\n'; //Travel name
            if(!travel->getGeneralErrors().empty()){
                inFile << spaces << "General:" << '\n';
                for(string &msg : travel->getGeneralErrors()){
                    inFile << spaces + spaces << msg << '\n';
                }
            }
            for(auto &pair : travel->getErrorsMap()){
                if(!pair.second.empty()){
                    inFile << spaces << pair.first + ":" << '\n'; //algorithm name
                    for(string &msg : pair.second){
                        inFile << spaces + spaces << msg << '\n'; //error msg list for this alg in this travel
                    }
                }
            }
            inFile << lineSep+lineSep << '\n';
        }
    }
    inFile.close();
}


void SimulatorObj::initOutputMap(map<string,map<string,pair<int,int>>>& outputMap){
    for(auto& travel : this->TravelsVec){
        if(!travel->isErroneous())
            outputMap.insert(make_pair(travel->getName(),travel->getAlgResultsMap()));
    }
}

/**
 * This function checks if the results are empty --> if true simulation.results wont be created
 */
bool SimulatorObj:: isResultsEmpty() {
    for(auto& travel : this->TravelsVec)
        if(!travel->getAlgResultsMap().empty())
            return false;

    return true;
}

/**
 * This function checks if the errors list are empty --> if true simulation.errors wont be created
 */
bool SimulatorObj:: isErrorsEmpty(){
    for(auto& travel: this->TravelsVec)
        if(!travel->getGeneralErrors().empty() || !travel->getErrorsMap().empty())
            return false;
    if(!generalErrors.empty())
        return false;
    return true;
}


void SimulatorObj::runAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, std::unique_ptr<Travel> &travel){
    list<string> simCurrAlgErrors;
    map<string,int> visitNumbersByPort;
    int res = 0;
    vector<std::shared_ptr<Port>> route = this->simShip->getRoute();
    res = checkIfFatalErrorOccurred("alg");
    if(res != -1) {
        string algInstructionsFolder = SimulatorObj::createAlgorithmOutDirectory(alg.first, mainOutputPath,travel->getName());
        for (int portNum = 0; portNum < (int) route.size() && res != -1; portNum++) {
            string portName = route[portNum]->get_name();
            pPort = this->simShip->getPortByName(portName);
            this->currPortNum = portNum;
            int visitNumber = visitNumbersByPort[portName];
            fs::path portPath = getPathOfCurrentPort(travel,portName,visitNumber);
            cout << "======================== iterates over this port + " << portName << " at visit number " + std::to_string(visitNumber+1) +" ============================" << endl;
            res = runCurrentPort(portName, portPath, alg, simCurrAlgErrors, algInstructionsFolder,
                                 ++visitNumbersByPort[portName],travel);
            compareIgnoredAlgErrsVsSimErrs(portName, visitNumber, simCurrAlgErrors);
        }
    }
    compareFatalAlgErrsVsSimErrs(simCurrAlgErrors);
    travel->getErrorsMap().insert(make_pair(alg.first,simCurrAlgErrors));
}

/**
 * This function gets the full file path of the given port in the given travel at the X time we visit there
 * Note* some ports in the ship route might not have a files,then it creates an empty path for them.
 */
fs::path SimulatorObj::getPathOfCurrentPort(std::unique_ptr<Travel> &travel,string& portName,int visitNumber){
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

void SimulatorObj::compareIgnoredAlgErrsVsSimErrs(string &portName,int visitNumber,list<string> &simCurrAlgErrors){
    if(algErrorCodes[16] != simErrorCodes[16])
        simCurrAlgErrors.emplace_back(ERROR_NO_CARGO_TOLOAD(portName, visitNumber));

}

/**
 * This function handles current port and algorithm interaction
 * @param portName          - the current port name
 * @param portPath          - the full <port name>_<number>.cargo_data path
 * @param portNum           - tracking the current port in the route list
 * @param alg               - current alg
 * @param simShip           - the current simulator ship
 * @param simCurrAlgErrors  - the list of current algorithm errros
 * @param algOutputFolder   - the alg output folder to write the output file to
 * @param visitNumber       - current port visit number
 * @param algInfo           - stores the data of instructions count and errors count
 */
int SimulatorObj::runCurrentPort(string &portName,fs::path &portPath,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                    list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber,std::unique_ptr<Travel> &travel){

    string inputPath,outputPath;
    int instructionsCount, errorsCount, algReturnValue;
    std::optional<pair<int,int>> result;
    pair<int,int> intAndError;
    SimulatorValidation validator(this);
    inputPath =  portPath.string();

    outputPath = algOutputFolder + PATH_SEPARATOR + portName + "_" + std::to_string(visitNumber) + ".crane_instructions";
    algReturnValue = alg.second->getInstructionsForCargo(inputPath,outputPath);
    updateErrorCodes(algReturnValue, "alg");
    result = validator.validateAlgorithm(outputPath,inputPath,simCurrAlgErrors,portName,visitNumber);

    if(!result) return -1; //case there was an error in validateAlgorithm
    /*Incrementing the instructions count and errors count*/
    intAndError = result.value();
    instructionsCount = std::get<0>(intAndError);
    errorsCount = std::get<1>(intAndError);
    if(travel->getAlgResultsMap().find(alg.first) == travel->getAlgResultsMap().end()){
        travel->getAlgResultsMap().insert(make_pair(alg.first,pair<int,int>()));
    }
    std::get<0>(travel->getAlgResultsMap()[alg.first]) += instructionsCount;
    std::get<1>(travel->getAlgResultsMap()[alg.first]) += errorsCount;
    return errorsCount;
}



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

void SimulatorObj:: prepareForNewTravel() {
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
    /*sort by certain first will occur algorithms with 0 errors and lowest instructions*/
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
