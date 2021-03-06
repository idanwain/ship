#include "Util.h"

/**
 * This function initialize the list of travels from given -travel_path path
 */
void initListOfTravels(string &path,list<string> &generalErrors,vector<std::shared_ptr<Travel>> &TravelsVec,string  mainOpPath){
    string msg = " only sub folders allowed in main folder, file won't be included in the program";
    for(const auto &entry : fs::directory_iterator(path)){
        if(!entry.is_directory()){
            generalErrors.emplace_back(ERROR_NOT_DIRECTORY(entry, msg));
            continue;
        }
        string travelName = entry.path().filename().string();
        std::unique_ptr<Travel> currTravel = std::make_unique<Travel>(travelName,mainOpPath);
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
        TravelsVec.emplace_back(std::move(currTravel));

    }
}

/**
 * This function get a travel folder map:= <portName,list of files with same portName>
 * and assigns at list[portNum] the given entry which corresponds to map[portName][portNum-1] --> portName_portNum.cargo_data
 */
void insertPortFile(std::unique_ptr<Travel> &currTravel, string &portName, int portNum, const fs::path &entry) {
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

/**
 * This function creates a file that shows the result of the cartesian multiplication of the simulator
 */
void createResultsFile(string &mainOutputPath, vector<std::shared_ptr<Travel>> &TravelsVec,
                       map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>> &algMap) {
    std::ofstream inFile;
    int sumInstructions = 0,sumErrors = 0;
    const char comma = ',';
    list<string> travels;
    list<string> algorithmNames;
    string path = mainOutputPath;
    map<string,map<string,pair<int,int>>> output_map;

    if(isResultsEmpty(TravelsVec)){
        NO_RESULT_FILE;
        return;
    }
    initOutputMap(output_map,TravelsVec);
    path.append(PATH_SEPARATOR);
    path.append("simulation.results");
    inFile.open(path);
    if(inFile.fail()){
        ERROR_RESULTS_FILE;
        exit(EXIT_FAILURE);
    }
    for(auto &travel : TravelsVec) {//Get travel Names
        if(!travel->isErroneous())
            travels.emplace_back(travel->getName());
    }
    for(auto &algName : algMap)//Get algorithm names
        algorithmNames.emplace_back(algName.first);

    SimulatorObj::sortAlgorithmsForResults(output_map, algorithmNames);

    inFile << "RESULTS" << comma;
    for(string &travel_name : travels)
        inFile << travel_name << comma;
    inFile << "Sum" << comma << "Num Errors" <<  '\n';

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
 */
void createErrorsFile(string& mainOutputPath, vector<std::shared_ptr<Travel>> &TravelsVec, list<string> &generalErrors) {
    const string spaces = "      ";//6spaces
    const string lineSep = "====================================================================================================";
    std::ofstream inFile;
    string path = mainOutputPath;
    if(isErrorsEmpty(TravelsVec,generalErrors)){
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
        inFile << lineSep+lineSep << '\n';
    }
    for(auto &travel : TravelsVec){
        if(travel->isErrorsExists()){
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


/**
 * This function initialize the output result map
 */
void initOutputMap(map<string,map<string,pair<int,int>>>& outputMap,vector<std::shared_ptr<Travel>> &TravelsVec){
    for(auto& travel : TravelsVec){
        if(!travel->isErroneous())
            outputMap.insert(make_pair(travel->getName(),travel->getAlgResultsMap()));
    }
}

/**
 * This function checks if the results are empty --> if true simulation.results wont be created
 */
bool isResultsEmpty(vector<std::shared_ptr<Travel>> &TravelsVec) {
    for(auto& travel : TravelsVec)
        if(!travel->getAlgResultsMap().empty())
            return false;

    return true;
}

/**
 * This function checks if the errors list are empty --> if true simulation.errors wont be created
 */
bool isErrorsEmpty(vector<std::shared_ptr<Travel>> &TravelsVec,list<string> &generalErrors){
    for(auto& travel: TravelsVec) {
        if (!travel->getGeneralErrors().empty())
            return false;
        for (auto &p : travel->getErrorsMap())
            if(!p.second.empty())
                return false;
    }
    if(!generalErrors.empty())
        return false;
    return true;
}

/**
 * This function creates the algorithmXTravel tasks that the thread executer will get task from
 */
void initTasksContainer(tasksContainer& tasks,map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>> &mapVec,vector<std::shared_ptr<Travel>>& TravelsVec){
    int numTravels = TravelsVec.size();
    int numAlgorithms = mapVec.size();
    tasks.reserve(numTravels*numAlgorithms);
    for(auto& travel : TravelsVec){
        for(auto& alg : mapVec){
            pair<string,std::unique_ptr<AbstractAlgorithm>> p(alg.first,alg.second());
            std::unique_ptr<AbstractAlgorithm> newAlg = alg.second();
            string algName = alg.first;
            std::tuple<std::shared_ptr<Travel>,string,std::unique_ptr<AbstractAlgorithm>> tup(travel,algName,std::move(newAlg));
            tasks.emplace_back(std::move(tup));
        }
    }
}

/**
 * This function gets the algorithms.so files from the mainAlgorithms path (if given or from current path)
 * and saves the paths in the given vector.
 */
void getAlgSoFiles(vector<fs::path> &algPaths){
    std::regex reg(".*\\.so");
    for(const auto &entry : fs::directory_iterator(mainAlgorithmsPath)) {
        if (!entry.is_directory()) {
            if (std::regex_match(entry.path().filename().string(), reg)) {
                algPaths.emplace_back(entry);
            }
        }
    }
}

/**
 * This function gets the information from the flags and assign them to relevant variables
 */
void handleFlags(int argc, char** argv){
    string basePath = fs::current_path().string();
    const string travelFlag = "-travel_path";
    const string outputFlag = "-output";
    const string algorithmFlag = "-algorithm_path";
    const string threadFlag = "-num_threads";

    for(int i = 1; i+1 < argc; i++){
        if(argv[i] == travelFlag)
            mainTravelPath = argv[i+1];
        else if(argv[i] == outputFlag)
            mainOutputPath = argv[i+1];
        else if(argv[i] == algorithmFlag)
            mainAlgorithmsPath = argv[i+1];
        else if(argv[i] == threadFlag)
            threadNum = atoi(argv[i+1]);
    }
    if(mainOutputPath.empty() || !fs::exists(mainOutputPath))
        mainOutputPath = basePath;
    if(mainAlgorithmsPath.empty() || !fs::exists(mainAlgorithmsPath))
        mainAlgorithmsPath = basePath;
    if(mainTravelPath.empty() || !fs::exists(mainTravelPath)) {
        NO_TRAVEL_PATH;
        exit(EXIT_FAILURE);
    }
}

/**
 * This function creates a vector of algorithms that the simulator willing to test on
 */
vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> initAlgorithmList(map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map){
    vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> algList;
    for(auto &entry: map){
        algList.emplace_back(make_pair(entry.first,entry.second()));
    }
    return algList;
}



