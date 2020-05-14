#include "SimulatorValidation.h"


/**
 * This function manages to validate the whole algorithm cranse instruction at given port
 * @param outputPath - the path of the crane instructions file
 * @param contAtPortPath - the input path of the port data of containers to be tested
 * @param simShip - the simulator ship
 * @param portNumber - the current port number(to get reference were we are at the route)
 * @param currAlgErrors - the algorithm errors list to update
 */
std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &contAtPortPath,
                                               int portNumber, list<string>& currAlgErrors,SimulatorObj* simulator,string& portName,int visitNumber){
    std::cout << "validateAlgorithm: start" << std::endl;
    std::ifstream instructionsFile;
    string line,id,instruction;
    pair<string,string> idAndInstruction;
    map<string,list<string>> linesFromPortFile;
    int errorsCount = 0,instructionsCount = 0;

    extractRawDataFromPortFile(linesFromPortFile, contAtPortPath);
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
        if(validateInstruction(instruction, id, coordinates, simulator, linesFromPortFile, portNumber)){
            if(instruction == "R"){
                linesFromPortFile.erase(id);
                continue;
            }
            coordinate one = std::tuple<int,int>(coordinates[0],coordinates[1]);
            std::unique_ptr<Container> cont = createContainer(simulator,linesFromPortFile,id,instruction,portName);
            if(instruction == "L") {
                execute(simulator->getShip(), instruction.at(0), cont, one, std::forward_as_tuple(-1, -1), simulator->getPort());
            }
            else if(instruction == "U"){
                execute(simulator->getShip(), instruction.at(0), cont, one, std::forward_as_tuple(-1, -1), simulator->getPort());
                cont.reset(nullptr);
            }
            else if(instruction == "M"){
                coordinate two = std::tuple<int,int>(coordinates[3],coordinates[4]);
                execute(simulator->getShip(), instruction.at(0), cont, one, two, simulator->getPort());
                cont.reset(nullptr);
            }
            instructionsCount++;
            linesFromPortFile.erase(id);
        }
        else{
            currAlgErrors.emplace_back(ERROR_CONTLINEINSTRUCTION(portName,visitNumber,instruction));
            errorsCount = -1;
            break;
        }
    }
    instructionsFile.close();
    /*Final checks*/
    if(errorsCount != -1){
        //simulator->sortContainersByPriority(portNumber);
        errorsCount += checkIfContainersLeftOnPort(simulator,currAlgErrors);
        errorsCount += checkForContainersNotUnloaded(simulator, currAlgErrors);
        /*Final check, if there are any containers were on containers at port file that the algorithm didnt handle properly*/
        if(portNumber != (int)simulator->getShip()->getRoute().size() - 1)
            errorsCount += checkContainersDidntHandle(linesFromPortFile,currAlgErrors,portName,visitNumber);

    }
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
bool validateInstruction(string &instruction,string &id, vector<int> &coordinates,SimulatorObj* sim,std::map<string,list<string>>& portContainers,int portNum){
    bool isValid;
    std::cout << "before extractKgToValidate" << std::endl;
    int kg = extractKgToValidate(portContainers,sim,id);
    std::cout << "id " + id + " inst " + instruction << " kg found " << kg << endl; //TODO DELETE
    if(instruction == "L"){
        isValid =  validateLoadInstruction(coordinates, sim,kg);
    }
    else if(instruction == "U"){
        isValid =  validateUnloadInstruction(coordinates, sim);
    }
    else if(instruction == "R"){
        isValid = validateRejectInstruction(portContainers, id, sim, portNum,kg);
    }
    else {
        isValid = validateMoveInstruction(coordinates, sim);
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
bool validateRejectInstruction(map<string,list<string>>& portContainers, string& id,SimulatorObj* sim,int portNum,int kg){
    string line;
    auto &ship = sim->getShip();
    VALIDATION reason = VALIDATION::Valid;
    string portName = extractPortNameToValidate(portContainers,sim,id);
    std::tuple<int,int,int> tup = sim->getShip()->getCoordinate(id);

    if(portContainers.find(id) != portContainers.end())
        line = portContainers[id].front();
    /*Case the data is not validate*/
    if(!line.empty() && !validateContainerData(line,reason,id,ship))
        return true;
    /*Case there is a container with same id on the ship already then suppose to reject the container awaiting at port*/
    if(!line.empty() && std::get<0>(tup) >= 0)
        return true;
    /*Case the dest of the container equals is the src*/
    if(ship->getRoute().at(portNum)->get_name() == portName)
        return true;
    /*Case the dest port is not in route*/
    if(!isPortInRoute(portName,ship->getRoute(),portNum))
        return true;
    /*Case there is no free space in the ship map*/
    if(ship->getFreeSpace() == 0)
        return true;
    /*Case there are 2 lines with the same id*/
    if(portContainers[id].size() > 1)
        return true;
    /*Case there is an weight balance problem*/
    if(checkIfBalanceWeightIssue(sim,kg,tup))
        return true;

    return false;
}

/**
 * This function validates load crane instruction given by the algorithm
 * @param coordinates - given coordinates that algorithm did load to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool validateLoadInstruction(vector<int> &coordinates,SimulatorObj* sim,int kg){
    int x = coordinates[0],y = coordinates[1], z = coordinates[2];
    auto &ship = sim->getShip();
    auto map = ship->getMap();
    /*Check if the position of the x,y axis is out of bounds*/
    if((x < 0 || x > ship->getAxis("x")) && (y < 0 || y > ship->getAxis("y"))) return false;
    /*Check if the position of z axis is out of bounds*/
    if((int)(*map).at(x).at(y).size() != z) return false;
    /*Check if the weight balance is approved*/
    std::cout << "validateLoadInstruction: before tryOperation" << std::endl;
    WeightBalanceCalculator calc = sim->getCalc();
    std::cout << "calc address: " << &calc << std::endl;
    return calc.tryOperation('L', kg, x, y) == APPROVED;
}

/**
 * This function validate's unload crane instruction. given by the algorithm
 * @param coordinates - given coordinates that algorithm did unload to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool validateUnloadInstruction(vector<int> &coordinates,SimulatorObj* sim){
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
bool validateMoveInstruction(vector<int> &coordinates, SimulatorObj* sim){
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
bool checkIfBalanceWeightIssue(SimulatorObj* sim, int kg,std::tuple<int,int,int> &coordinates){
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

int extractKgToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id){
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

string extractPortNameToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id){
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

int checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string &portName,int visitNum) {
    int err = 0;
    for(auto& idInstruction : idAndRawLine){
        if(!idInstruction.second.empty()){
            currAlgErrors.emplace_back(ERROR_IDNOTHANDLE(idInstruction.second.front(),portName,visitNum));
            err= -1;
        }
    }
    return err;
}


int checkIfContainersLeftOnPort(SimulatorObj* sim , list<string> &currAlgErrors){
    auto currPort = sim->getPort();
    string currPortName = currPort->get_name();
    int err = 0;
    for(auto &cont : *currPort->getContainerVec(Type::PRIORITY)){
        err = -1;
        string id = cont.getId();
        string dstPortName = cont.getDest()->get_name();
        currAlgErrors.emplace_back(ERROR_CONTNOTINDEST(id,currPortName,dstPortName));
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
int checkForContainersNotUnloaded(SimulatorObj* sim, list<string> &currAlgErrors){
    auto currPort = sim->getPort();
    auto &shipMap = *sim->getShip()->getMap();
    int err = 0;
    for(auto& vX : shipMap)
        for(auto& vY : vX)
            for(auto& cont : vY)
                if(cont.getDest() == currPort) {
                    currAlgErrors.emplace_back(ERROR_CONTLEFTONSHIP(cont.getId()));
                    err = -1;
                }

    return err;
}



