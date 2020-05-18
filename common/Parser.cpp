#include "Parser.h"

/**
 * This function gets the number from the <port_symbol>_<num>.<filetype> deceleration
 * @param fileName
 * @return the number
 */
int extractPortNumFromFile(const string& fileName){
    if(fileName.size() < 5) return 0;
    int dot = fileName.find(".");
    int dash = (int)fileName.find("_") + 1;
    string numPort = fileName.substr(dash, dot - dash);
    return atoi(numPort.data());

}

/**
 * This function get a travel folder map:= <portName,list of files with same portName>
 * and assigns at list[portNum] the given entry which corresponds to map[portName][portNum-1] --> portName_portNum.cargo_data
 * @param travelMap - the travel_name map
 * @param portName - the current portName file
 * @param portNum - the current portName number
 * @param entry - the entry path
 */
string extractPortNameFromFile(const string& fileName){
    int index = fileName.find_first_of("_");
    string portName = fileName.substr(0,index);
    return portName;
}

/**
 * This function checks if port already exist in the vector list, if exists it return 0 but
 * sets a pointer to the same port at the the end of the vector.
 * @param vec - the vector of ports
 * @param str - the string of the port to be check
 * @return 0 iff exist already port with same name
 */
int portAlreadyExist(std::vector<std::shared_ptr<Port>>& vec,string &str){
    for(const auto &element : vec ){
        if(element->get_name() == str){
            vec.emplace_back(element);
            return 1;
        }
    }
    return 0;
}

/**
 * This function parsing the dimensions of a ship/container location from file/string
 * @param arr - the array to store the dimensions in
 * @param inFile - the file we are working on to parse the data from
 * @param str - byFile means we parse from file, otherwise parse from the str itself
 */
void getDimensions(std::array<int,3> &arr, std::istream &inFile,string str){
    std::cout << "getDimensions: start" << std::endl;
    vector<string> vec;
    /*if we read from file then we want to get first line which is not comment*/
    if(str == "byFile"){
        str = "";
        while(getline(inFile,str)) {
            if(!isCommentLine(str)) break; //comment symbol
        }
    }
    std::cout << "getDimensions: after while loop" << std::endl;
    vec = stringSplit(str,delim);
    std::cout << "getDimensions: after stringSplit" << std::endl;

    if((int)vec.size() != 3) {
        arr[0] = -1; /*indicates caller function that bad line format*/
        return;
    }
    std::cout << "getDimensions: before for loop (has atoi)" << std::endl;
    for(int i = 0; i < 3; i++){
        if(isValidInteger(vec[i]))
            arr[i] = atoi(vec[i].data());
        else
            arr[i] = -1;
        }
    std::cout << "getDimensions: end" << std::endl;
}


/**
 * This function gets a string(a line from the file) parse the line to get 3 ints (x,y,z) such ship.getMap()[x][y][z]
 * will be initialized to be block container.
 * @param str - the string to parse
 * @param ship - the ship to get it's map from.
 * @param lineNumber - current line number reading from file optional --> algorithm might
 * @return returns empty string iff no error happened
 */
pair<string,int> setBlocksByLine(string &str,std::unique_ptr<Ship>& ship,int lineNumber) {
    std::cout << "setBlocksByLine: start" << std::endl;
    auto &map = ship->getMap();
    std::ifstream inFile;
    std::array<int,3> dim{};
    pair<string,int> pair;
    getDimensions(dim,inFile,str);
    std::cout << "setBlocksByLine: after getDimensions" << std::endl;

    if(dim[0] >= ship->getAxis("x") || dim[1] >= ship->getAxis("y") || dim[2] >= ship->getAxis("z")){
        if(dim[0] >= ship->getAxis("x") || dim[1] >= ship->getAxis("y")){
            std::cout << "setBlocksByLine: before Plan_XYError" << std::endl;
            std::get<1>(pair) = Plan_XYError;
        }
        if(dim[2] >= ship->getAxis("z")){
            std::cout << "setBlocksByLine: before Plan_ZError" << std::endl;
            std::get<1>(pair) = Plan_ZError;
        }

        std::get<0>(pair) = ERROR_XYZ_DIM(lineNumber);
        return pair;
    }

    else if(dim[0] < 0 || dim[1] < 0 || dim[2] < 0){
        std::cout << "setBlocksByLine: before ERROR_BAD_LINE" << std::endl;
        std::get<0>(pair) = ERROR_BAD_LINE(lineNumber);
        std::get<1>(pair) = Plan_BadLine;
    }
    else if(!(map)[dim[0]][dim[1]].empty()){
        if(static_cast<int>((map)[dim[0]][dim[1]].size()) != ship->getAxis("z")-dim[2]) {
            std::cout << "setBlocksByLine: before ERROR_DIFF_VALUE" << std::endl;
            std::get<0>(pair) = ERROR_DIFF_VALUE(lineNumber, dim[0], dim[1]);
            std::get<1>(pair) = Plan_Con;
        }
        else {
            std::cout << "setBlocksByLine: before ERROR_SAME_VALUE" << std::endl;
            std::get<0>(pair) = ERROR_SAME_VALUE(lineNumber, dim[0], dim[1]);
            std::get<1>(pair) = Plan_BadLine;
        }
    }
    else{
        std::cout << "setBlocksByLine: final else (before for loop)" << std::endl;
        for(int i = 0; i < ship->getAxis("z")-dim[2]; i++){
            std::cout << "setBlocksByLine: ************* in for loop **********" << std::endl;
            std::cout << "setBlocksByLine: dim[0] = " << dim[0] << std::endl;
            std::cout << "setBlocksByLine: dim[1] = " << dim[1] << std::endl;
            std::cout << "setBlocksByLine: dim[2] = " << dim[2] << std::endl;
            std::cout << "setBlocksByLine: map size = " << map.size() << std::endl;
            std::cout << "setBlocksByLine: map dim[0] size = " << map[dim[0]].size() << std::endl;
            std::cout << "setBlocksByLine: map dim[0][1] size = " << (map)[dim[0]][dim[1]].size() << std::endl;
            (map)[dim[0]][dim[1]].push_back(Container("block"));
            ship->updateFreeSpace(-1);
        }
    }
    std::cout << "setBlocksByLine: end" << std::endl;
    return pair;
}

/**
 * This function parse line by line from the file, and initialized the block containers in the shipmap
 * @param ship - the ship object to get it's shipMap and update it
 * @param inFile - the file pointer
 * @return 0 if succeeded, specified return code otherwise.
 */
int extractArgsForBlocks(std::unique_ptr<Ship>& ship,const string& filePath, std::unique_ptr<Travel>* travel){
    std::cout << "extractArgsForBlocks: start" << std::endl;

    string line;
    int lineNumber = 2, returnStatement = 0,num;
    std::ifstream inFile;
    pair<string,int> pair;

    inFile.open(filePath);
    if (inFile.fail()) {
        ERROR_READ_PATH(filePath);
        returnStatement = Plan_Fatal;
        std::cout << "extractArgsForBlocks: in fail" << std::endl;
    }
    else {
        getline(inFile,line); /*first line is ship dimensions we already got them*/
        while (getline(inFile, line)){
            std::cout << "extractArgsForBlocks: **************** while loop iteration ***************" << std::endl;
            if(!isCommentLine(line)) {/*if not commented line*/
                std::cout << "extractArgsForBlocks: after isCommentLine" << std::endl;
                pair = setBlocksByLine(line, ship, lineNumber);
                std::cout << "extractArgsForBlocks: after setBlocksByLine" << std::endl;
                num = std::get<1>(pair);
                if (num != 0) {
                    updateErrorNum(&returnStatement, num);
                    std::cout << "extractArgsForBlocks: after updateErrorNum" << std::endl;
                    if(travel != nullptr){
                        (*travel)->setNewGeneralError(std::get<0>(pair));
                        std::cout << "extractArgsForBlocks: after setNewGeneralError" << std::endl;
                    }
                }
            }
            lineNumber++;
        }
    }
    inFile.close();
    std::cout << "extractArgsForBlocks: end" << std::endl;
    return returnStatement;
}

/**
 * overloaded function without the errors for the algorithm
 */
int extractArgsForBlocks(std::unique_ptr<Ship>& ship,const std::string& filePath) {
    list<string> tempListForAlg;
    return extractArgsForBlocks(ship, filePath,nullptr);
}

int extractShipPlan(const std::string& filePath, std::unique_ptr<Ship>& ship){
    std::cout << "extractShipPlan: start" << std::endl;
    std::array<int, 3> dimensions{};
    std::ifstream inFile;
    int returnStatement = 0;

    inFile.open(filePath);
    if (inFile.fail()) {
        ERROR_READ_PATH(filePath);
        returnStatement = Plan_Fatal;
        std::cout << "extractShipPlan: in fail" << std::endl;
    }
    else {
        getDimensions(dimensions, inFile,"byFile");
        std::cout << "extractShipPlan: after getDimension" << std::endl;
        if(dimensions[0] < 0 || dimensions[1] < 0 || dimensions[2] < 0) {
            returnStatement = Plan_Fatal;
        } else {
            ship = std::make_unique<Ship>(dimensions[1], dimensions[2], dimensions[0]);
        }
    }

    inFile.close();
    std::cout << "extractShipPlan: end" << std::endl;
    return returnStatement;
}

/**
 * This function parse the ship map and the ship route files and init a new ship object with
 * the information it parsed.
 * @param folder - the folder that contains ship_route, ship_plan files
 * @return the constructed ship iff folder is not empty.
 */
std::unique_ptr<Ship> extractArgsForShip(std::unique_ptr<Travel> &travel,SimulatorObj &simulator) {
    string file_path;
    vector<std::shared_ptr<Port>> travelRoute;
    std::unique_ptr<Ship> ship;

    if(travel->getPlanPath().empty() || travel->getRoutePath().empty()){
        if(travel->getRoutePath().empty())
            travel->setNewGeneralError(ERROR_LACK_ROUTE);
        else
            travel->setNewGeneralError(ERROR_LACK_PLAN);
        return nullptr;
    }
    /*Handle ship plan file*/
    file_path = travel->getPlanPath().string();
    int resultInt = extractShipPlan(file_path,ship);
    if(resultInt == 0){
        resultInt = extractArgsForBlocks(ship,file_path,&travel);
        simulator.updateErrorCodes(resultInt, "sim");
        if(simulator.checkIfFatalErrorOccurred("sim") == -1)
            return nullptr;
    }
    else {
        travel->setNewGeneralError(ERROR_FATA_LPLAN);
        simulator.updateErrorCodes(resultInt, "sim");
        return nullptr;
    }
    /*Handle ship route file*/
    file_path = travel->getRoutePath().string();
    resultInt = extractTravelRoute(ship,file_path,&travel);
    if(resultInt == Route_Fatal || ship->getRoute().size() <= 1){
        if(resultInt == Route_Fatal)
            travel->setNewGeneralError(ERROR_FATAL_ROUTE);
        else
            travel->setNewGeneralError(ERROR_NOT_ENOUGH_PORTS);
        return nullptr;
    }
    SimulatorObj::compareRoutePortsVsCargoDataPorts(ship,travel);
    simulator.updateErrorCodes(resultInt, "sim");
    return ship;
}


/**
 * This function parses the data from a port file, it saves it by container id and the data line of this id in a map
 * @param map - the given map to save instruction by port id
 * @param inputPath - the input path of the port file
 */
void extractRawDataFromPortFile(std::map<string,list<string>>& map, string& inputPath){
    std::ifstream inFile;
    string line;
    if(inputPath.empty()) return;

    inFile.open(inputPath);
    if(inFile.fail()){
        ERROR_READ_PATH(inputPath);
        return;
    }
    while(getline(inFile,line)){
        if(isCommentLine(line))continue;
        vector<string> parsedInfo = stringSplit(line,delim);
        string contID = parsedInfo.at(0);
        if(map.find(contID) == map.end()){
            list<string> ls;
            map.insert(make_pair(contID,ls));
        }
        map[contID].emplace_back(line);
    }
    inFile.close();
}


/**
 * Parses the containers data and connecting it to the "load" list of the port
 * @param input_full_path_and_file_name
 */
bool parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output,
                     std::unique_ptr<Ship>& ship, std::shared_ptr<Port>& port) {
    std::string line;
    std::ifstream input;
    std::cout << "parseDataToPort: input path is " << inputFullPathAndFileName << std::endl;
    std::shared_ptr<Container> con;
    std::shared_ptr<Port> dest;
    if(inputFullPathAndFileName.empty()) return true;

    input.open(inputFullPathAndFileName);
    if(input.fail()){
        ERROR_READ_PATH(inputFullPathAndFileName);
        return false;
    }

    while(getline(input,line)){
        std::cout << "parseDataToPort: *********** while loop iteration ***********" << line << std::endl;
        std::cout << "parseDataToPort: line =  " << line << std::endl;
        if(isCommentLine(line)) continue; //comment symbol
        std::string id; int weight;
        VALIDATION reason = VALIDATION::Valid;
        std::cout << "parseDataToPort: after VALIDATION ENUM init" << line << std::endl;
        if(validateContainerData(line, reason, id, ship)) {
            std::cout << "parseDataToPort: after validateContainerData" << line << std::endl;
            extractContainersData(line, id, weight, dest, ship);
            std::cout << "parseDataToPort: after extractContainersData" << line << std::endl;
            if(dest != nullptr && !(*dest == *port) && dest->get_name() != "NOT_IN_ROUTE") {
                std::cout << "parseDataToPort: in if (before addContainer)" << line << std::endl;
                port->addContainer(id,weight,port,dest, Type::LOAD);
                std::cout << "parseDataToPort: in if (after addContainer)" << line << std::endl;
                std::cout << " load vector size after adding is " << port->getContainerVec(Type::LOAD)->size();
            }
            else {
                writeToOutput(output,AbstractAlgorithm::Action::REJECT, id);
                std::cout << "parseDataToPort: after writeToOutput 1" << line << std::endl;
            }
        }
        else {
            if(reason != VALIDATION::Valid){
                writeToOutput(output, AbstractAlgorithm::Action::REJECT, id);
                std::cout << "parseDataToPort: after writeToOutput 2" << line << std::endl;
            }
        }
    }
    input.close();
    std::cout << "parseDataToPort: end" << std::endl;
    return true;
}
