#include "SimulatorObj.h"

void SimulatorObj::setShip(std::unique_ptr<Ship> &getShip){
    this->simShip = std::make_unique<Ship>(*getShip);
}

void SimulatorObj::addNewTravelListErrors(list<string> &listErrors,string errListName){
    this->currTravelErrors.insert(make_pair(errListName,listErrors));
}

void SimulatorObj::addNewErrorToGeneralErrors(string msg){
    this->currTravelGeneralErrors.emplace_back(msg);
}


void SimulatorObj::initListDirectories(string &path){
    string msg = " only sub folders allowed in main folder, file won't be included in the program";
    auto& directories = this->inputFiles;
    for(const auto &entry : fs::directory_iterator(path)){
        if(!entry.is_directory()){
            std::cerr << "Error: "  << entry.path().filename().string()  << msg << std::endl;
            continue;
        }
        string travelName = entry.path().filename().string();
        if(!isValidTravelName(travelName)) continue;
        directories.insert(make_pair(travelName,map<string,vector<fs::path>>()));
        for(const auto &deep_entry : fs::directory_iterator(entry.path())){
            string fileName = deep_entry.path().filename().string();
            if(isValidPortFileName(fileName)){
                string portName = extractPortNameFromFile(fileName);
                int portNum = extractPortNumFromFile(fileName);
                insertPortFile(directories[travelName],portName,portNum,deep_entry.path());

            }
            else if(isValidShipRouteFileName(fileName)){
                vector<fs::path> vec(1);
                vec.emplace_back(deep_entry.path());
                directories[travelName].insert(make_pair(ROUTE,vec));
            }
            else if(isValidShipMapFileName(fileName)){
                vector<fs::path> vec(1);
                vec.emplace_back(deep_entry.path());
                directories[travelName].insert(make_pair(PLAN,vec));
            }
        }

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
void SimulatorObj::insertPortFile(map<string, vector<fs::path> > &travelMap, string &portName, int portNum, const fs::path &entry) {
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
void SimulatorObj::addErrorsInfo(string &travelName){
    this->outputErrorsInfo.insert(make_pair(travelName,currTravelErrors));
    this->outputErrorsInfo[travelName].insert(make_pair("general",this->currTravelGeneralErrors));
}

void SimulatorObj::addResultsInfo(string &travelName){
    this->outputResultsInfo.insert(make_pair(travelName,algInfo));
}

map<string,map<string,list<string>>>& SimulatorObj::getErrorsInfo(){
    return this->outputErrorsInfo;
}

map<string,map<string,pair<int,int>>>& SimulatorObj::getResultsInfo(){
    return this->outputResultsInfo;
}
void SimulatorObj::addOutputInfo(string& travelName){
    if(!currTravelErrors.empty()){
        this->addErrorsInfo(travelName);
    }
    if(!algInfo.empty()){
        this->addResultsInfo(travelName);
    }
}

map<string,map<string,vector<fs::path>>>& SimulatorObj::getInputFiles(){
    return this->inputFiles;
}

std::array<bool,NUM_OF_ERRORS>& SimulatorObj::getCommonErrors(){
    return this->algErrorCodes;
}

std::array<bool,NUM_OF_ERRORS>& SimulatorObj:: getSimErrors(){
    return this->simErrorCodes;
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
void SimulatorObj::createResultsFile(string path){
    std::ofstream inFile;
    int sumInstructions = 0,sumErrors = 0;
    const char comma = ',';
    list<string> travels;
    list<string> algNames;
    auto &output_map = this->outputResultsInfo;
    if(output_map.empty())
        return;
    path.append(PATH_SEPARATOR);
    path.append("simulation.results");
    inFile.open(path); //Default mode is writing to a file
    if(inFile.fail()){
        std::cerr << "Error: failed to create results file" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(auto &map : output_map)//Get travel Names
        travels.emplace_back(map.first);
    for(auto &outterPair : output_map[travels.front()])//Get algNames
        algNames.emplace_back(outterPair.first);

    sortAlgorithms(output_map,algNames);

    inFile << "RESULTS" << comma;
    for(string &travel_name : travels)
        inFile << travel_name << comma;
    inFile << "Sum" << comma << "Errors" <<  '\n';

    for(auto& algName : algNames){
        inFile << algName << comma;
        for(auto &travelName : travels){
            sumInstructions += output_map[travelName][algName].first;
            sumErrors += output_map[travelName][algName].second;
            inFile << output_map[travelName][algName].first << comma;
        }
        inFile << sumInstructions << comma << sumErrors << '\n';
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
void SimulatorObj::createErrorsFile(string path) {
    const string spaces = "      ";//6spaces
    std::ofstream inFile;
    auto &simErrors = this->outputErrorsInfo;
    if(simErrors.empty())
        return;
    path.append(PATH_SEPARATOR);
    path.append("simulation.errors");
    inFile.open(path);
    if (inFile.fail()) {
        std::cerr << "Error: failed to create errors file" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (auto &outterPair : simErrors) {
        inFile << outterPair.first << " Errors" << '\n'; //Travel name
        for (auto &innerPair : simErrors[outterPair.first]) {
            inFile << spaces << innerPair.first + ":" << '\n'; //Algorithm name
            for (string &msg : innerPair.second) {
                inFile << spaces + spaces << msg << '\n'; //error msg list for this alg in this travel
            }
        }
    }
    inFile.close();
}

/**
 * This function create an algorithm output directory per algorithm and travel in the main output directory
 * @param algName
 * @param outputDirectory
 * @param travelName
 * @return the full path of this directory
 */
string createAlgorithmOutDirectory(const string &algName,const string &outputDirectory,const string &travelName){
    string algOutDirectory = outputDirectory + PATH_SEPARATOR + algName + "_" + travelName + "_" + "crane_instructions";
    fs::path directoryPath(algOutDirectory);
    fs::path parentDirectory(outputDirectory);
    if(!fs::exists(directoryPath)){
        fs::create_directory(directoryPath,parentDirectory);
    }
    return algOutDirectory;
}

void SimulatorObj::runCurrentAlgorithm(pair<string,std::unique_ptr<AbstractAlgorithm>> &alg, string &travelName){
    list<string> simCurrAlgErrors;
    map<string,int> visitNumbersByPort;
    int res = 0;
    vector<std::shared_ptr<Port>> route = this->simShip->getRoute();
    string algOpFolder = createAlgorithmOutDirectory(alg.first,mainOutputPath,travelName);
    for(int portNum = 0; portNum < (int)route.size() && res != -1; portNum++){
        string portName = route[portNum]->get_name();
        int visitNumber = visitNumbersByPort[portName];
        fs::path portPath(this->inputFiles[travelName][portName][visitNumber]);
        res = runCurrentPort(portName,portPath,portNum,alg,simCurrAlgErrors,algOpFolder,++visitNumbersByPort[portName]);
    }
    currTravelErrors.insert(make_pair(alg.first, simCurrAlgErrors));
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
int SimulatorObj::runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
                    list<string> &simCurrAlgErrors,string &algOutputFolder,int visitNumber){

    string inputPath,outputPath;
    int instructionsCount, errorsCount, algReturnValue;
    std::optional<pair<int,int>> result;
    pair<int,int> intAndError;
    if(portPath.empty())
        inputPath = "";
    else
        inputPath =  portPath.string();

    outputPath = algOutputFolder + PATH_SEPARATOR + portName + "_" + std::to_string(visitNumber) + ".crane_instructions";
    algReturnValue = alg.second->getInstructionsForCargo(inputPath,outputPath);
    this->updateArrayOfCodes(algReturnValue,"alg");

    result = validateAlgorithm(outputPath,inputPath,portNum,simCurrAlgErrors,this);
    if(!result) return -1; //case there was an error in validateAlgorithm

    /*Incrementing the instructions count and errors count*/
    intAndError = result.value();
    instructionsCount = std::get<0>(intAndError);
    errorsCount = std::get<1>(intAndError);
    if(algInfo.find(alg.first) == algInfo.end()){
        algInfo.insert(make_pair(alg.first,pair<int,int>()));
    }
    std::get<0>(algInfo[alg.first]) += instructionsCount;
    std::get<1>(algInfo[alg.first]) += errorsCount;
    return errorsCount;
}

void SimulatorObj::updateArrayOfCodes(int num, string type){
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

void SimulatorObj::addListOfGeneralErrors(list<string> &generalErrors){
    for(string &msg : generalErrors){
        this->currTravelGeneralErrors.emplace_back(msg);
    }
}

void SimulatorObj::createErrorsFromArray(){
    auto &arr = simErrorCodes;
    list<string> errList;
    if(arr[0])
        errList.emplace_back("ship plan: a position has an equal number of floors, or more, "
                             "than the number of floors provided in the first line (ignored)");
    if(arr[1])
        errList.emplace_back("ship plan: a given position exceeds the X/Y ship limits (ignored)");
    if(arr[2])
        errList.emplace_back("ship plan: bad line format after first line or duplicate x,y appearance with same data (ignored)");
    if(arr[3])
        errList.emplace_back("ship plan: travel error - bad first line or file cannot be read altogether (cannot run this travel)");
    if(arr[4])
        errList.emplace_back("ship plan: travel error - duplicate x,y appearance with different data (cannot run this travel)");
    if(arr[5])
        errList.emplace_back("travel route: a port appears twice or more consecutively (ignored)");
    if(arr[6])
        errList.emplace_back("travel route: bad port symbol format (ignored)");
    if(arr[7])
        errList.emplace_back("travel route: travel error - empty file or file cannot be read altogether (cannot run this travel)");
    if(arr[8])
        errList.emplace_back("travel route: travel error - file with only a single valid port (cannot run this travel)");
    if(arr[9])
        /*reserved*/
    if(arr[10])
        errList.emplace_back("containers at port: duplicate ID on port (ID rejected)");
    if(arr[11])
        errList.emplace_back("containers at port: ID already on ship (ID rejected)");
    if(arr[12])
        errList.emplace_back("containers at port: bad line format, missing or bad weight (ID rejected)");
    if(arr[13])
        errList.emplace_back("containers at port: bad line format, missing or bad port dest (ID rejected)");
    if(arr[14])
        errList.emplace_back("containers at port: bad line format, ID cannot be read (ignored)");
    if(arr[15])
        errList.emplace_back("containers at port: illegal ID check ISO 6346 (ID rejected)");
    if(arr[16])
        errList.emplace_back("containers at port: file cannot be read altogether (assuming no cargo to be loaded at this port)");
    if(arr[17])
        errList.emplace_back("containers at port: last port has waiting containers (ignored)");
    if(arr[18])
        errList.emplace_back("containers at port: total containers amount exceeds ship capacity (rejecting far containers)");

    /*TODO need to add the list to general errors or something*/

}

void SimulatorObj:: initNewTravel() {
    this->algErrorCodes = std::array<bool,NUM_OF_ERRORS>{false};
    this->simErrorCodes = std::array<bool,NUM_OF_ERRORS>{false};
    this->currTravelGeneralErrors.clear();
    this->currTravelErrors.clear();
    this->algInfo.clear();
}

/**
 * This function sorts the algorithms output info list and assigning the order to the algorithms list
 * such that:
 * 1.sorting is based on --> 0 errors count(0 before -1..), then lowest instructions count
 * @param outputInfo
 * @param algorithms
 */
void sortAlgorithms(map<string,map<string,pair<int,int>>> &outputInfo,list<string> &algorithms){
    map<string,pair<int,int>> algAndValues;
    vector<std::tuple<string,int,int>> sortedVec;
    for(auto &alg : algorithms){
        algAndValues.insert(make_pair(alg,pair<int,int>()));
    }

    /*Gather the overall score of travel X algorithms*/
    for(auto &algName : algorithms){
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
            return std::get<1>(t1) > std::get<1>(t2);
        } else
            return std::get<2>(t1) < std::get<2>(t2);
    });
    algorithms.clear();
    for(auto& alg : sortedVec){
        algorithms.emplace_back(std::get<0>(alg));
    }

}





