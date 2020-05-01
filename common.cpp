#include "common.h"

/**
 * This function executes the command on the simulator ship map if all validation passed
 * @param ship
 * @param command
 * @param container
 * @param origin
 * @param dest
 */
void execute(std::unique_ptr<Ship>& ship, char command, std::unique_ptr<Container>& container, coordinate origin, coordinate dest) {
    switch (command) {
        case 'L':
            ship->addContainer(*container, origin);
            break;
        case 'U':
            ship->removeContainer(origin);
            container.reset(nullptr);
            break;
        case 'M':
            ship->moveContainer(origin, dest);
            break;
        default:
            std::cout << "Invalid command, please insert L/U/M commands." << std::endl;
    }
}

/**
 * This function manages to validate the whole algorithm cranse instruction at given port
 * @param outputPath - the path of the crane instructions file
 * @param inputPath - the input path of the port data of containers to be tested
 * @param simShip - the simulator ship
 * @param portNumber - the current port number(to get reference were we are at the route)
 * @param currAlgErrors - the algorithm errors list to update
 */
std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &inputPath, std::unique_ptr<Ship>& simShip, int portNumber,list<string>& currAlgErrors){
    std::ifstream inFile;
    string line,id,instruction;
    std::pair<string,string> idAndInstruction;
    std::map<string,string> linesFromPortFile;
    int errorsCount = 0,instructionsCount = 0;
    inFile.open(outputPath);
    parseDataFromPortFile(linesFromPortFile, inputPath);
    if(inFile.fail()) {
        std::cerr << FAIL_TO_READ_PATH + outputPath << endl;
        return std::nullopt;
    }
    while(getline(inFile,line)){
        vector<int> coordinates;
        extractCraneInstruction(line, idAndInstruction, coordinates);
        instruction = std::get<0>(idAndInstruction);
        id = std::get<1>(idAndInstruction);
        /*if the below statement pass test, then we can execute instruction or if it's reject then do nothing as we need to reject*/
        if(validateInstruction(instruction, id, coordinates, simShip, linesFromPortFile, portNumber)){
            if(instruction == "R")continue;
            coordinate one = std::tuple<int,int>(coordinates[0],coordinates[1]);
            std::unique_ptr<Container> cont = std::make_unique<Container>(id);
            if(instruction == "L") {
                execute(simShip, instruction.at(0), cont, one, std::forward_as_tuple(-1, -1));
            }
            else if(instruction == "U"){
                execute(simShip, instruction.at(0), cont, one, std::forward_as_tuple(-1, -1));
                cont.reset(nullptr);
            }
            else if(instruction == "M"){
                coordinate two = std::tuple<int,int>(coordinates[3],coordinates[4]);
                execute(simShip, instruction.at(0), cont, one, two);
                cont.reset(nullptr);
            }
            instructionsCount++;
        }
        else{
            string msg = "Error: container id: " + id + ", instruction: " + instruction;
            currAlgErrors.emplace_back(msg);
            errorsCount++;
        }
    }
    inFile.close();
    return {std::pair<int,int>(instructionsCount,errorsCount)};
}

/**
 * This function gets a string to extract the data from and arrange it.
 * @param toParse - the string to parse from
 * @param instruction
 * @param coordinates
 */
void extractCraneInstruction(string &toParse, std::pair<string,string> &instruction, vector<int> &coordinates){
    auto parsedInfo = stringSplit(toParse,delim);

    for(int i = 0; i < (int)parsedInfo.size(); i++){
        if(i == 0)
            std::get<0>(instruction) = parsedInfo.at(0);
        else if(i == 1)
            std::get<1>(instruction) = parsedInfo.at(1);
        else
            coordinates.emplace_back(stoi(parsedInfo.at(i)));
    }
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
bool validateInstruction(string &instruction,string &id, vector<int> &coordinates,std::unique_ptr<Ship>& ship,std::map<string,string>& portContainers,int portNum){
    auto map = ship->getMap();
    bool isValid;
    if(instruction == "L"){
        isValid =  validateLoadInstruction(coordinates, ship);
    }
    else if(instruction == "U"){
        isValid =  validateUnloadInstruction(coordinates, ship);
    }
    else if(instruction == "R"){
        isValid = validateRejectInstruction(portContainers, id, ship, portNum);
    }
    else {
        isValid = validateMoveInstruction(coordinates, *map);
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
bool validateRejectInstruction(std::map<string,string>& portContainers, string& id,std::unique_ptr<Ship>& ship,int portNum){
    string line = portContainers[id];
    auto parsedInfo = stringSplit(line,delim);
    string portName = parsedInfo.at(2) + " " + parsedInfo.at(3);
    VALIDATION reason = VALIDATION::Valid;/*might be used in exercise 2 to be more specific*/
    bool test1 = !validateContainerData(line,reason,id,ship);
    bool test3 = (ship->getRoute().at(portNum)->get_name() == portName);
    bool test2 = !isPortInRoute(portName,ship->getRoute(),portNum);
    bool test4 = ship->getFreeSpace() == 0; /*if != 0 that means this test didn't passed --> reject at this test failed*/
    /*if one of the test fails, that means --> that reject at this instruction is necessary*/
    return (test1 || test2 || test3 || test4);
}

/**
 * This function validates load crane instruction given by the algorithm
 * @param coordinates - given coordinates that algorithm did load to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool validateLoadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship){
    int x = coordinates[0],y = coordinates[1], z = coordinates[2];
    auto map = ship->getMap();
    if((x < 0 || x > ship->getAxis("x")) && (y < 0 || y > ship->getAxis("y"))) return false;
    else if((int)(*map).at(x).at(y).size() != z) return false;
    /*if not in size --> either in lower level which is wrong, or floating in the air*/
    /*else if(check weight balance)
    To exercise 2 need to check also weight*/
    return true;
}

/**
 * This function validate's unload crane instruction. given by the algorithm
 * @param coordinates - given coordinates that algorithm did unload to
 * @param ship - to get ship map
 * @return true iff all tests were failed
 */
bool validateUnloadInstruction(vector<int> &coordinates,std::unique_ptr<Ship>& ship){
    int x = coordinates[0],y = coordinates[1], z = coordinates[2];
    auto map = ship->getMap();
    if((x < 0 || x > ship->getAxis("x")) && (y < 0 || y > ship->getAxis("y"))) return false;
    else if((int)(*map).at(x).at(y).size() != z+1) return false;
    else if((*map).at(x).at(y).at(z).getId() == "block") return false;
    /*if z is not the top floor available container then we can't unload from this position
      if position (x,y,z) at map is block container then we can't move them*/
    /*else if(check weight calculator)
     * To exercise 2 need to check also weight*/
    return true;
}

/**
 * This function checks if algorithm move crane instruction is valid, logically it checks if the unload operation
 * and the load operation of the move are legal
 * @param coordinates - the vector of coordinates to unload and load , x1-z1 are unload coordinates.
 * @param map - ship's map
 * @return true iff it passed all tests
 */
bool validateMoveInstruction(vector<int> &coordinates, vector<vector<vector<Container>>>& map){
    int x1 = coordinates[0],y1 = coordinates[1],z1 = coordinates[2];
    int x2 = coordinates[3],y2 = coordinates[4],z2 = coordinates[5];
    return (((int)(map.at(x1).at(y1).size()) == z1 + 1) && ((int)(map.at(x2).at(y2).size()) == z2));
}

/**
 * This function checks if the given line and is is valid
 * the validation checks are - existing id, bad id, bad port name, port not in route.
 * @param line - the container data line
 * @param reason - assigning the reason if the validation failed
 * @param id - the container id
 * @param ship - current ship
 * @return true iff reason == valid
 */
bool validateContainerData(const std::string& line, VALIDATION& reason, std::string& id, std::unique_ptr<Ship>& ship) {
    int i=-1;
    auto data = stringSplit(line, delim);
    if(data.size() != 4)
        return false;
    std::string port_name;

    for(const std::string& item : data){
        ++i;
        if (i == 0) {
            id = item;
            bool idBool = validateId(item);
            if(!idBool){
                reason = VALIDATION::InvalidID;
                return false;
            }
            else {
                if(idExistOnShip(item, ship)){
                    reason = VALIDATION::ExistID;
                    return false;
                }
            }
        }
        if(i == 1) {
            bool weight = isNumber(item);
            if(!weight){
                return false;
            }
        }
        else if (i == 2) {
            port_name = item;
        }
        else if(i == 3){
            port_name += " " + item;
        }
    }
    bool dest = isValidPortName(port_name);
    if(!dest){
        reason = VALIDATION::InvalidPort;
        return false;
    }
    return true;
}

/**
 * This function validates if given port portName is valid port name according to seaport code
 * @param portName
 * @return true iff it's valid port name
 */
bool isValidPortName(const std::string& portName){
    std::regex reg("\\s*[A-Za-z]{5}\\s*");
    return std::regex_match(portName, reg);
}
/**
 * This function checks if a given string is a valid number aka include's only 0-9 chars
 * @param s
 * @return true iff it's a valid number
 */
bool isNumber(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}


/**
 * util function that split strings by a given delimiter
 * @param s - string to be splitted
 * @param delimiter - delimiters to be splitted by
 * @return vector of words in s
 */
std::vector<std::string> stringSplit(std::string s, const char* delimiter) {
    if(iscntrl(s[s.length() - 1])){
        s = s.substr(0, s.length() - 1);
    }
    size_t start = 0;
    size_t end = s.find_first_of(delimiter);

    std::vector<std::string> output;

    while (end <= std::string::npos)
    {
        output.emplace_back(s.substr(start, end - start));

        if (end == std::string::npos)
            break;

        start = end + 1;
        end = s.find_first_of(delimiter, start);

        while(start == end){
            start = end + 1;
            end = s.find_first_of(delimiter, start);
        }
    }

    return output;
}

/**
 * This function validate's that the id is in the right format according to ISO 6346
 * @param str - the checked id
 * @return true iff it's in the right format
 */
bool validateId(const std::string& str) {
    int i = 0;
    if (str.length() != 11){
        return false;
    }
    for(auto letter : str){
        if(i < 3){ // owner code
            if(!isupper(letter)){
                return false;
            }
        }
        else if(i == 3){ // category identifier
            if (letter != 'U' && letter != 'J' && letter != 'Z'){
                return false;
            }
        }
        else { // serial number & check digit
            if(!isdigit(letter)){
                return false;
            }
        }
        ++i;
    }
    return true;
}

/**
 * This function checks if the given container id already exist in the ship map
 * @param id
 * @param ship
 * @return true iff it already exist
 */
bool idExistOnShip(const std::string& id, std::unique_ptr<Ship>& ship){
    auto map = ship->getContainersByPort();
    for(auto & pPort : map){
        for(auto con_it : pPort.second) {
            if(con_it.getId() == id){
                return true;
            }
        }
    }
    return false;
}


/**
 * This function checks if the given portName exist in the following port's in the route file
 * from the current route location
 * @param portName
 * @param route
 * @param portNum
 * @return true iff it's in the following route
 */
bool isPortInRoute(const std::string& portName, const std::vector<std::shared_ptr<Port>>& route, int portNum) {
    bool found = false;
    for(auto port_it = route.begin() + portNum + 1; port_it != route.end(); ++port_it){
        if(((*port_it)->get_name()) == portName){
            found = true;
            break;
        }
    }
    return found && portName != "NOT_IN_ROUTE";
}


/**
 * This function extracting the travel route from a file and sets it to be the given ship travel's route
 * @param ship
 * @param inFile
 */
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath,list<string> &generalErrors) {
    std::unique_ptr<std::vector<std::shared_ptr<Port>>> vec = std::make_unique<std::vector<std::shared_ptr<Port>>>();
    string line;
    std::ifstream inFile;
    int returnStatement = 0;

    inFile.open(filePath);
    if (inFile.fail()) {
        std::cerr << FAIL_TO_READ_PATH + filePath << endl;
        returnStatement = FAIL_TO_READ_PATH_CODE;
    }
    else {
        while (getline(inFile, line)) {
            if (line.at(0) == '#') continue; //comment symbol
            else if (isValidPortName(line)) {
                if(iscntrl(line[line.length() - 1])){
                    line = line.substr(0, line.length() - 1);
                }
                if(vec->at(vec->size()-1) && vec->at(vec->size()-1)->get_name() == line){
                    generalErrors.emplace_back("Port " + line + " 2 or more consecutive times");
                }
                else if (!portAlreadyExist(*vec, line)) {
                    std::shared_ptr<Port> p1 = std::make_shared<Port>(line);
                    vec->emplace_back(p1);
                }
            }
        }
        ship->setRoute(*vec);
    }

    inFile.close();
    return returnStatement;
}

/**
 * overloaded function without error log for algorithm usage
 */
int extractTravelRoute(std::unique_ptr<Ship>& ship, const std::string& filePath){
    list<string> tempListForAlg;
    return extractTravelRoute(ship, filePath, tempListForAlg);
}


/*************************************************************************
************************ MERGE WITH STOWAGE ALGORITHM ********************
*************************************************************************/


void writeToOutput(std::ofstream& output,
                              const std::string& command, const std::string& id,
                              const std::tuple<int,int,int>& pos, const std::tuple<int,int,int>& movedTo){
    if(command == "R"){
        output << command << ", " <<  id << std::endl;
    } else if (command == "M") {
        output << command << ", " <<  id << ", " << std::get<0>(pos) <<
               ", " << std::get<1>(pos) << ", " << std::get<2>(pos) <<
               ", " << std::get<0>(movedTo) << ", " << std::get<1>(movedTo) <<
               ", " << std::get<2>(movedTo) << std::endl;
    } else {
        output << command << ", " <<  id << ", " << std::get<0>(pos) << ", " << std::get<1>(pos) << ", " << std::get<2>(pos) << std::endl;
    }
}

/**
 * Parses the containers data and connecting it to the "load" list of the port
 * @param input_full_path_and_file_name
 */
bool parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output,
        std::unique_ptr<Ship>& ship, std::shared_ptr<Port>& port) {
    std::string line;
    std::ifstream input;

    input.open(inputFullPathAndFileName);

    if(input.fail()){
        std::cout << "Error Opening file, closing program" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(getline(input,line)){
        if (!line.empty() && line.at(0) == '#') continue; //comment symbol
        std::string id; int weight;
        std::shared_ptr<Port> dest;
        VALIDATION reason = VALIDATION::Valid;
        if(validateContainerData(line, reason, id, ship)) {
            extractContainersData(line, id, weight, dest, ship);
            if(dest == nullptr) {
                std::cout << id << ": "<< CONTAINER_NOT_IN_ROUTE << std::endl;
                writeToOutput(output,"R", id,std::forward_as_tuple(-1,-1,-1) , std::forward_as_tuple(-1,-1,-1));
            }
            else {
                std::unique_ptr<Container> con = std::make_unique<Container>(id, weight,port, dest);
                port->addContainer(*con, 'L');
            }
        }
        else {
            if(reason != VALIDATION::Valid){
                writeToOutput(output, "R", id, std::forward_as_tuple(-1,-1,-1), std::forward_as_tuple(-1,-1,-1));
            }
        }
    }
    input.close();
    return true;
}

/**
 * parses the data from a given line
 * @param line - data line of container
 * @param id - container's ID
 * @param weight - container's weight
 * @param dest - container's destination
 */
void extractContainersData(const std::string& line, std::string &id, int &weight, std::shared_ptr<Port>& dest, std::unique_ptr<Ship>& ship) {
    int i=0;
    auto data = stringSplit(line, delim);
    std::string port_name;
    for(const std::string& item : data){
        switch(i){
            case 0:
                id = item;
                ++i;
                break;
            case 1:
                weight = stoi(item);
                ++i;
                break;
            case 2:
                port_name = item;
                ++i;
                break;
            case 3:
                port_name += " " + item;
        }
    }
    auto dest_temp = ship->getPortByName(port_name);
    if(!dest_temp){
        dest = dest_temp;
    }
}