
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

void SimulatorObj::resetOutputLists(){
    this->currTravelErrors.clear();
    this->currTravelGeneralErrors.clear();
    this->algInfo.clear();

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

std::array<bool,NUM_OF_SIM_ERRORS>& SimulatorObj:: getSimErrors(){
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
    vector<std::shared_ptr<Port>> route = this->simShip->getRoute();
    string algOpFolder = createAlgorithmOutDirectory(alg.first,mainOutputPath,travelName);
    for(int portNum = 0; portNum < (int)route.size(); portNum++){
        string portName = route[portNum]->get_name();
        int visitNumber = visitNumbersByPort[portName];
        fs::path portPath(this->inputFiles[travelName][portName][visitNumber]);
        runCurrentPort(portName,portPath,portNum,alg,simCurrAlgErrors,algOpFolder,++visitNumbersByPort[portName]);
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
void SimulatorObj::runCurrentPort(string &portName,fs::path &portPath,int portNum,pair<string,std::unique_ptr<AbstractAlgorithm>> &alg,
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
    if(!result) return; //case there was an error in validateAlgorithm

    /*Incrementing the instructions count and errors count*/
    intAndError = result.value();
    instructionsCount = std::get<0>(intAndError);
    errorsCount = std::get<1>(intAndError);
    if(algInfo.find(alg.first) == algInfo.end()){
        algInfo.insert(make_pair(alg.first,pair<int,int>()));
    }
    std::get<0>(algInfo[alg.first]) += instructionsCount;
    std::get<1>(algInfo[alg.first]) += errorsCount;
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

void SimulatorObj::initCalc(const string& file_path) {
    simCalc.readShipPlan(file_path);
}

WeightBalanceCalculator SimulatorObj::getCalc() {
    return simCalc;
}
