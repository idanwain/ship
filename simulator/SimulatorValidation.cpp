#include "SimulatorValidation.h"


/**
 * This function manages to validate the whole algorithm cranse instruction at given port
 * @param outputPath - the path of the crane instructions file
 * @param contAtPortPath - the input path of the port data of containers to be tested
 * @param simShip - the simulator ship
 * @param portNumber - the current port number(to get reference were we are at the route)
 * @param currAlgErrors - the algorithm errors list to update
 */
std::optional<pair<int,int>> SimulatorValidation::validateAlgorithm(string &outputPath, string &contAtPortPath,
                                               list<string>& currAlgErrors,string& portName,int visitNumber){
    std::cout << "validateAlgorithm: start" << std::endl;
    std::ifstream instructionsFile;
    string line,id,instruction;
    pair<string,string> idAndInstruction;
    int errorsCount = 0,instructionsCount = 0;
    extractRawDataFromPortFile(rawDataFromPortFile, contAtPortPath);
    initLoadedListAndRejected();
    initPriorityRejected();

    instructionsFile.open(outputPath);
    if(instructionsFile.fail()) {
        P_ERROR_READPATH(outputPath);
        return std::nullopt;
    }
    while(getline(instructionsFile, line)){
        vector<int> coordinates;
        extractCraneInstruction(line, idAndInstruction, coordinates);
        instruction = std::get<0>(idAndInstruction);
        id = std::get<1>(idAndInstruction);
        /*if the below statement pass test, then we can execute instruction or if it's reject then do nothing as we need to reject*/
        if(validateInstruction(instruction, id, coordinates)){
            if(instruction == "R"){
                rawDataFromPortFile.erase(id);
                continue;
            }
            coordinate one = std::tuple<int,int>(coordinates[0],coordinates[1]);
            std::unique_ptr<Container> cont = createContainer(sim,rawDataFromPortFile,id,instruction,portName);
            if(instruction == "L") {
                execute(sim->getShip(), instruction.at(0), cont, one, std::forward_as_tuple(-1, -1), sim->getPort());
            }
            else if(instruction == "U"){
                execute(sim->getShip(), instruction.at(0), cont, one, std::forward_as_tuple(-1, -1), sim->getPort());
                cont.reset(nullptr);
            }
            else if(instruction == "M"){
                coordinate two = std::tuple<int,int>(coordinates[3],coordinates[4]);
                execute(sim->getShip(), instruction.at(0), cont, one, two, sim->getPort());
                cont.reset(nullptr);
            }
            instructionsCount++;
            eraseFromRawData(line,id);
        }
        else{
            currAlgErrors.emplace_back(ERROR_CONTLINEINSTRUCTION(portName,id,visitNumber,instruction));
            errorsCount = -1;
            break;
        }
    }
    instructionsFile.close();
    /*Final checks*/
    if(errorsCount != -1)
        errorsCount  = finalChecks(currAlgErrors,portName,visitNumber);

    return {std::pair<int,int>(instructionsCount,errorsCount)};
}

/**
 * This function manages to valid all types of instructions given by algorithm to port crane
 * @param instruction - current instruction
 * @param id
 * @param coordinates
 * @param ship
 * @param portContainers
 * @param portNum
 * @return true iff the validation of instruction went successfully
 */
bool SimulatorValidation::validateInstruction(string &instruction,string &id, vector<int> &coordinates){
    bool isValid;
    std::cout << "before extractKgToValidate" << std::endl;
    int kg = extractKgToValidate(rawDataFromPortFile,sim,id);
    std::cout << "id " + id + " inst " + instruction << " kg found " << kg << endl; //TODO DELETE
    if(instruction == "L"){
        isValid =  validateLoadInstruction(coordinates,kg,id);
    }
    else if(instruction == "U"){
        isValid =  validateUnloadInstruction(coordinates);
    }
    else if(instruction == "R"){
        isValid = validateRejectInstruction(id,kg);
    }
    else {
        isValid = validateMoveInstruction(coordinates);
    }
    return isValid;
}

/**
 * This function validates reject crane instruction given by the algorithm
 * @pre assuming this function can't be reached if it's the last port in the route validation
 * @param portContainers - this list contains the current port file data(raw data)
 * @param id - the container id we want to check why rejected
 * @param ship - to get the ship map
 * @param portNum - the current port number
 * @return true iff one of the tests failes
 */
bool SimulatorValidation::validateRejectInstruction(string& id,int kg){
    string line;
    auto &ship = sim->getShip();
    string portName = extractPortNameToValidate(rawDataFromPortFile,sim,id);
    std::tuple<int,int,int> tup = sim->getShip()->getCoordinate(id);
    if(rawDataFromPortFile.find(id) != rawDataFromPortFile.end())
        line = rawDataFromPortFile[id].front();
    /*Case the data is not validate / duplicate Id / idExist on ship*/
    if(!line.empty() && mustRejected[id].find(line) != mustRejected[id].end()){
        mustRejected[id].erase(line);
        return true;
    }
    /*Case the container with this didn't prioritize by algorithm*/
    if(!line.empty() && priorityRejected.find(id) !=  priorityRejected.end())
        return true;
    /*Case the algorithm decided to not load this container*/
    if(!line.empty() && possiblePriorityReject.find(id) != possiblePriorityReject.end())
        return true;
    /*Case there is no free space in the ship map*/
    if(ship->getFreeSpace() == 0)
        return true;
    /*Case there is an weight balance problem*/
    if(!checkIfBalanceWeightIssue(sim,kg,tup))
        return true;

    return false;
}

/**
 * This function validates load crane instruction given by the algorithm
 * @param coordinates - given coordinates that algorithm did load to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool SimulatorValidation::validateLoadInstruction(vector<int> &coordinates,int kg,string& id){
    int x = coordinates[0],y = coordinates[1], z = coordinates[2];
    auto &ship = sim->getShip();
    auto map = ship->getMap();
    /*Check if the position of the x,y axis is out of bounds*/
    if((x < 0 || x > ship->getAxis("x")) && (y < 0 || y > ship->getAxis("y"))) return false;
    /*Check if the position of z axis is out of bounds*/
    if((int)(*map).at(x).at(y).size() != z) return false;
    /*Check if algorithm loaded un prioritized container*/
    if(priorityRejected.find(id) != priorityRejected.end())
        return false;
    /*Check if the weight balance is approved*/
    WeightBalanceCalculator calc = sim->getCalc();
    return calc.tryOperation('L', kg, x, y) == APPROVED;
}

/**
 * This function validate's unload crane instruction. given by the algorithm
 * @param coordinates - given coordinates that algorithm did unload to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool SimulatorValidation::validateUnloadInstruction(vector<int> &coordinates){
    int x = coordinates[0],y = coordinates[1], z = coordinates[2];
    auto &ship = sim->getShip();
    auto &map = *ship->getMap();
    /*Check if the position of the x,y axis is out of bounds*/
    if((x < 0 || x > ship->getAxis("x")) && (y < 0 || y > ship->getAxis("y"))) return false;
    /*Check if the position of z axis is out of bounds*/
    if((int)(map).at(x).at(y).size() != z+1) return false;
    /*Check if the container at x,y,z is block container*/
    if((map).at(x).at(y).at(z).getId() == "block") return false;
        /*Check if weight balance is approved*/
    else {
        int kg = map.at(x).at(y).at(z).getWeight();
        return sim->getCalc().tryOperation('U', kg, x, y) == APPROVED;
    }
}


/**
 * This function checks if algorithm move crane instruction is valid, logically it checks if the unload operation
 * and the load operation of the move are legal
 * @param coordinates - the vector of coordinates to unload and load , x1-z1 are unload coordinates.
 * @param map - ship's map
 * @return true iff it passed all tests
 */
bool SimulatorValidation::validateMoveInstruction(vector<int> &coordinates){
    int x1 = coordinates[0],y1 = coordinates[1],z1 = coordinates[2];
    int x2 = coordinates[3],y2 = coordinates[4],z2 = coordinates[5];
    auto &map = *(sim->getShip()->getMap());
    int kg = 0;
    /*Check if the position of z axis is out of bounds*/
    if((int) (map.at(x1).at(y1).size()) != z1 + 1 || (int) (map.at(x2).at(y2).size()) != z2) return false;
    /*Check if weight balance approved for unload*/
    kg = map.at(x1).at(y1).at(z1).getWeight();
    return sim->getCalc().tryOperation('U', kg, x1, y1) == APPROVED && sim->getCalc().tryOperation('L', kg, x2, y2) == APPROVED;
}

/**
 * This function checks a reject instruction happened because of weight issue
 * if coordinates(x,y,z) are != -1 then --> container is on ship so we want to unload this
 * else container is on port and we want to load it
 * @param sim
 * @param kg
 * @param coordinates
 * @return true iff the simulator verified ship will be imbalanced if we load/unload
 */
bool SimulatorValidation::checkIfBalanceWeightIssue(SimulatorObj* sim, int kg,std::tuple<int,int,int> &coordinates){
    if(std::get<0>(coordinates) >= 0){
        return sim->getCalc().tryOperation('U',kg,std::get<0>(coordinates),std::get<1>(coordinates)) != BalanceStatus::APPROVED;
    }
    else{
        for(int i = 0; i < sim->getShip()->getAxis("x"); i++){
            for(int j = 0; j < sim->getShip()->getAxis("y"); j++){
                if((int)sim->getShip()->getMap()[i][j].size() < sim->getShip()->getAxis("z"))
                    if(sim->getCalc().tryOperation('L',kg,i,j) == BalanceStatus::APPROVED)
                        /*Found a position that the container can be loaded at*/
                        return false;
            }
        }
    }
    return true;
}

int SimulatorValidation::extractKgToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id){
    vector<string> parsedInfo;
    int kg = -1;
    bool found = false;
    /*Check if container id exist in the container at port file*/
    if(rawData.find(id) != rawData.end()){
        parsedInfo = stringSplit(rawData[id].front(),delim);
        if(isValidInteger(parsedInfo.at(1))){
            kg = atoi(parsedInfo.at(1).data());
        }
    }
    /*Check is container exist in the priority list --> it was on ship and algorithm unloaded it*/
    else{
        for(auto &cont : *(sim->getPort()->getContainerVec(Type::PRIORITY))){
            if(cont.getId() == id){
                kg = cont.getWeight();
                found = true;
            }
        }
    }
    /*Check if container is on ship map --> case we extract kg to unload operation*/
    if(!found){
        for(auto &vX : *(sim->getShip()->getMap()))
            for(auto &vY : vX)
                for(auto &cont : vY)
                    if(cont.getId() == id){
                        kg = cont.getWeight();
                    }
    }
    return kg;
}

string SimulatorValidation::extractPortNameToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id){
    vector<string> parsedInfo;
    string portName;
    if(rawData.find(id) != rawData.end()){
        parsedInfo = stringSplit(rawData.find(id)->second.front(),delim);
        portName = parsedInfo.at(2);
    } else{
        for(auto &cont : *(sim->getPort()->getContainerVec(Type::PRIORITY))){
            if(cont.getId() == id){
                portName = cont.getDest()->get_name();
            }
        }
    }
    return portName;
}

int SimulatorValidation::checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string &portName,int visitNum) {
    int err = 0;
    for(auto& idInstruction : idAndRawLine){
        if(!idInstruction.second.empty()){
            for(auto &line : idInstruction.second){
                currAlgErrors.emplace_back(ERROR_IDNOTHANDLE(idInstruction.second.front(),portName,visitNum));
            }
            err= -1;
        }
    }
    return err;
}


int SimulatorValidation::checkIfContainersLeftOnPort(SimulatorObj* sim , list<string> &currAlgErrors){
    auto currPort = sim->getPort();
    auto &simShip = sim->getShip();
    string currPortName = currPort->get_name();
    int err = 0;
    for(auto &cont : *currPort->getContainerVec(Type::PRIORITY)){
        err = -1;
        string id = cont.getId();
        string dstPortName = cont.getDest()->get_name();
        currAlgErrors.emplace_back(ERROR_CONTNOTINDEST(id,currPortName,dstPortName));
    }
    /*Case there is still space on ship*/
    if(sim->getShip()->getFreeSpace() > 0 && err != -1){
        for(auto& cont : *currPort->getContainerVec(Type::LOAD)){
            err = -1;
            string id = cont.getId();
            string dstPortName = cont.getDest()->get_name();
            currAlgErrors.emplace_back(ERROR_CONTLEFTONPORT(id,currPortName,dstPortName));
        }
    }
    return err;
}

/**
 * This function iterates over the current ship map and checks if
 * at the end of this port stop we have containers left on ship map such that for container x
 * that left on ship it's destination is this port stop but it didnt unloaded.
 * @param sim
 * @param currAlgErrors
 * @return -1 iff there exist at least 1 container
 */
int  SimulatorValidation::checkForContainersNotUnloaded(SimulatorObj* sim, list<string> &currAlgErrors){
    auto currPort = sim->getPort();
    auto &shipMap = *sim->getShip()->getMap();
    int err = 0;
    for(auto& cont : sim->getShip()->getContainersByPort()[currPort]){
        currAlgErrors.emplace_back(ERROR_CONTLEFTONSHIP(cont.getId()));
        err = -1;
    }
    return err;
}


void SimulatorValidation::initLoadedListAndRejected() {
    auto &shipMap = sim->getShip();
    auto currPort = sim->getPort();
    for (auto &outterPair : rawDataFromPortFile) {
        bool foundValid = false;
        for (auto &info : outterPair.second) {
            VALIDATION reason = VALIDATION::Valid;
            string id;
            int weight = -1;
            std::shared_ptr<Port> dest;
            std::pair<string, VALIDATION> p;
            vector<string> parsedInfo = stringSplit(info, delim);
            validateContainerDataForReject(info, reason, sim);
            if (reason != VALIDATION::Valid) {
                p.first = info;
                p.second = reason;
                mustRejected[parsedInfo[0]].insert({info,reason});
            } else if(!foundValid) {
                extractContainersData(info, id, weight, dest, shipMap);
                Container con(id, weight, currPort, dest);
                currPort->addContainer(con, Type::LOAD);
                foundValid = true;
            }
            else{
                reason = VALIDATION::DuplicatedIdOnPort;
                mustRejected[id].insert({info,reason});
            }
        }
    }

}

void SimulatorValidation::validateContainerDataForReject(string& line,VALIDATION &reason,SimulatorObj* sim){
    auto parsedInfo = stringSplit(line,delim);
    auto& simShip = sim->getShip();
    auto srcPort = sim->getPort();
    int portNum = sim->getPortNum();
    if(parsedInfo.size() != 3){
        reason = VALIDATION::InvalidNumParameters;
        return;
    }
    else {
        if(!isValidId(parsedInfo[0])){
            reason = VALIDATION ::InvalidID;
            return;
        }
        else if(!isValidInteger(parsedInfo[1])){
            reason = VALIDATION ::InvalidWeight;
            return;
        }
        else if(!isValidPortName(parsedInfo[2])){
            reason = VALIDATION ::InvalidPort;
            return;
        }
        else if(!isPortInRoute(parsedInfo[2],sim->getShip()->getRoute(),portNum)){
            reason = VALIDATION :: InvalidPort;
            return;
        }
        else if(idExistOnShip(parsedInfo[0],simShip)){
            reason = VALIDATION :: ExistID;
            return;
        }
    }
    reason = VALIDATION :: Valid;
}

void SimulatorValidation::initPriorityRejected(){
    auto& simShip = sim->getShip();
    auto currPort = sim->getPort();
    int portNumber = sim->getPortNum();
    auto currPortLoadVec = currPort->getContainerVec(Type::LOAD);
    int amountToUnload = (int)simShip->getContainersByPort()[currPort].size();
    int amountToLoad = (int)currPortLoadVec->size();
    loadCapacity = simShip->getFreeSpace() + amountToUnload;
    if(loadCapacity < amountToLoad) {
        sim->sortContainersByPriority(currPortLoadVec);
        for (int i = loadCapacity; i < amountToLoad; i++) {
            string id = currPortLoadVec->at(i).getId();
            priorityRejected.insert({id, currPortLoadVec->at(i)});
        }
        if (loadCapacity > 0) {
            string suffixPort = (*currPortLoadVec->at(loadCapacity).getDest()).get_name();
            string prevPort = (*currPortLoadVec->at(loadCapacity - 1).getDest()).get_name();
            if (prevPort == suffixPort) {
                int i = 0;
                while ((*currPortLoadVec->at(i).getDest()).get_name() != suffixPort) {
                    i++;
                }
                while ((*currPortLoadVec->at(i).getDest()).get_name() == suffixPort) {
                    string id = currPortLoadVec->at(i).getId();
                    possiblePriorityReject.insert({id, currPortLoadVec->at(i)});
                    if (priorityRejected.find(id) != priorityRejected.end()) {
                        priorityRejected.erase(id);
                    }
                    i++;
                }
            }
        }
    }
}


void SimulatorValidation::eraseFromRawData(string &line, string &id) {
    if(rawDataFromPortFile[id].size() > 1)
        rawDataFromPortFile[id].remove(line);
    else
        rawDataFromPortFile.erase(id);
}

int SimulatorValidation::finalChecks(list<string> &currAlgErrors, string &portName, int visitNumber) {
    int errorsCount = 0;
    auto currPortPriority = sim->getPort()->getContainerVec(Type::PRIORITY);
    sim->sortContainersByPriority(currPortPriority);
    if(checkPrioritizedHandledProperly(currAlgErrors) == -1)
        return -1;
    if(checkIfContainersLeftOnPort(sim,currAlgErrors) == -1)
        return -1;
    else if(checkForContainersNotUnloaded(sim, currAlgErrors) == -1)
        return -1;
    /*Final check, if there are any containers were on containers at port file that the algorithm didnt handle properly*/
    else if(sim->getPortNum() != (int)sim->getShip()->getRoute().size() - 1)
            errorsCount = checkContainersDidntHandle(rawDataFromPortFile,currAlgErrors,portName,visitNumber);
    return errorsCount;
}

/**
 * This function executes the command on the simulator ship map if all validation passed
 * @param ship
 * @param command
 * @param container
 * @param origin
 * @param dest
 */
void SimulatorValidation::execute(std::unique_ptr<Ship>& ship, char command, std::unique_ptr<Container>& container, coordinate origin, coordinate dest, const std::shared_ptr<Port>& port) {
    string id;
    switch (command) {
        case 'L':
            id = container->getId();
            ship->addContainer(*container, origin);
            port->removeContainer(id,Type::LOAD);
            port->removeContainer(id,Type::PRIORITY);
            break;
        case 'U':
            if((container->getDest()->get_name() == port->get_name()))
                port->addContainer(*container, Type::ARRIVED);
            else
                port->addContainer(*container, Type::PRIORITY);
            ship->removeContainer(origin);
            break;
        case 'M':
            ship->moveContainer(origin, dest);
            break;
        default:
            cout << CLANG_TIDY << endl;
    }
}

int SimulatorValidation::checkPrioritizedHandledProperly(list<string> &currAlgErrors) {
    auto& simShip = sim->getShip();
    int err = 0;
    std::shared_ptr<Port> port;
    if(!possiblePriorityReject.empty() && loadCapacity > 0){
        for(auto& pair : possiblePriorityReject){
            port = pair.second.getDest();
            break;
        }
        auto &contOnShipByPort = simShip->getContainersByPort()[port];
        for(auto& pair : possiblePriorityReject){
            for(int i = 0; i < (int)contOnShipByPort.size(); i++){
                if(contOnShipByPort.at(i).getId() == pair.second.getId() && loadCapacity > 0){
                    loadCapacity--;
                }
            }
        }
        if(loadCapacity > 0){
            currAlgErrors.emplace_back(ERROR_LEFTPRIORITYONPORT);
            err = -1;
        }
    }
    return err;
}




