#include "Common.h"

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
    if(data.size() != 3){
        reason = VALIDATION::InvalidNumParameters;
        id = data.at(0);
        return false;
    }
    std::string port_name;

    for(const std::string& item : data){
        ++i;
        if (i == 0) {
            id = item;
            bool idBool = isValidId(item);
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
            if(!isValidInteger(item) || atoi(item.data()) < 0){
                reason = VALIDATION::InvalidWeight;
                return false;
            }
        }
        else if (i == 2) {
            port_name = item;
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
bool isValidId(const std::string& str) {
    std::regex reg("[A-Z]{3}[UJZ][0-9]{7}");
    //return std::regex_match(str,reg);
    std::map<char,int> numericalValues;
    char curr;
    int value = 0,sumDigits = 0, checkDigit = 0;
    numericalValues.insert({'A',10});
    /*Following ISO requirements*/
    if(std::regex_match(str, reg)){
        /*Init map*/
        for(int i = 0,j = 1; i < 27; i++,j++){
            curr = 'B' + i;
            if((11+j)% 11 == 0)
                j+=1;
            value = 11+j;
            numericalValues.insert({curr,value});
        }
        checkDigit = str.at(str.length() -1) - 48;
        for(int i = 0; i < (int)str.length()-1; i++){
            if(i < 4)
                value = numericalValues[str.at(i)];
            else
                value = str.at(i) - 48;
            sumDigits += (value * (1 << i));

        }
        value = sumDigits / 11;
        value = value * 11;
        if((sumDigits - value) % 10 == checkDigit)
            return true;
    }
    return false;
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
    if(route[portNum]->get_name() == portName){
        return false;
    }
    for(auto port_it = route.begin() + portNum + 1; port_it != route.end(); ++port_it){
        if(((*port_it)->get_name()) == portName){
            found = true;
            break;
        }
    }
    return found && portName != "NOT_IN_ROUTE";
}


void writeToOutput(std::ofstream& output, AbstractAlgorithm::Action command, const std::string& id, const std::tuple<int,int,int> pos, const std::tuple<int,int,int>& movedTo){
    switch(command) {
        case AbstractAlgorithm::Action::REJECT:
            output << static_cast<char>(command) << ", " << id << std::endl;
            break;
        case AbstractAlgorithm::Action::MOVE:
            output << static_cast<char>(command) << ", " << id << ", " << std::get<2>(pos) <<
                   ", " << std::get<0>(pos) << ", " << std::get<1>(pos) <<
                   ", " << std::get<2>(movedTo) << ", " << std::get<0>(movedTo) <<
                   ", " << std::get<1>(movedTo) << std::endl;
            break;
        case  AbstractAlgorithm::Action::LOAD:
        case  AbstractAlgorithm::Action::UNLOAD:
            output << static_cast<char>(command) << ", " << id << ", " << std::get<2>(pos) << ", " << std::get<0>(pos) << ", "
                   << std::get<1>(pos) << std::endl;
            break;
    }
}


/**
 * This function checks if the port file is valid aka <port_symbol>_<num>.<filetype>
 * @param fileName
 * @return true iff it's in the right format
 */
bool isValidPortFileName(const string& fileName){
    std::regex reg("[A-Za-z]{5}_[1-9]+\\.cargo_data");
    return std::regex_match(fileName, reg);
}

bool isValidShipRouteFileName(const string& fileName){
    std::regex reg(".*\\.route");
    return std::regex_match(fileName,reg);
}

bool isValidShipMapFileName(const string& fileName){
    std::regex reg(".*\\.ship_plan");
    return std::regex_match(fileName,reg);
}

/**
 * This function checks if a given string is comment (contains one or more #) or
 * if it's a white spaces string
 * @param line
 * @return
 */
bool isCommentLine(const string& line){
    std::regex commentLine("\\s*[#]+.*");
    std::regex whiteSpaces("\\s*\\t*\\r*\\n*");
return (std::regex_match(line,commentLine) || std::regex_match(line,whiteSpaces));
}

/**
 * This function checks if the travel name folder is in the right format aka "Travel" followed by any valid numbers
 * @param travelName
 * @return true iff it's in the right format
 */
bool isValidTravelName(const string& travelName){
    std::regex reg("Travel.*");
    return std::regex_match(travelName, reg);
}

/**
 * This function gets the array of 19 available errors, and the number that returned from the algorithm run
 * over the current port and transform this number into bit number, if bit at index i is 1 then there's an error
 * and array at index length() - i will get the value true as there's an error.
 * @param arr - the given array
 * @param num - the number returned from the algorithm run
 */
void initArrayOfErrors(std::array<bool,NUM_OF_ERRORS> &arr,int num){
    string binary = std::bitset<NUM_OF_ERRORS>(num).to_string();
    int index = 0, length = (int)binary.length()-1;
    for(index = 0; index <= length; index++){
        arr[index] = binary.at(length-index) - 48;
    }
}

/**
 * This function gets a curr error number which is a sum of 2^i distinct values only
 * @param currError - the current error number which we want to update
 * @param newError - the new error number which we want to compare to, and assign only values which not exist
 * in the curr error number
 */
void updateErrorNum(int* currError,int newError){
    std::array<bool,NUM_OF_ERRORS> currErrorArr {false};
    std::array<bool,NUM_OF_ERRORS> newErrorArr  {false};
    initArrayOfErrors(currErrorArr,*currError);
    initArrayOfErrors(newErrorArr,newError);
    for(int i = 0; i < NUM_OF_ERRORS; i++){
        if(!currErrorArr[i] && newErrorArr[i]){ /*if currError is false then no error yet found, and the other is true then found*/
            (*currError) += (1 << i);
        }
    }
}

bool isValidInteger(const std::string& str){
    return std::regex_match(str, std::regex("[-|+]*[0-9]+"));
}



/**
 * This function creates a container based on instruction
 * @param sim - current simulator
 * @param rawData - map id --> container at port line
 * @param id - the container id we wish to create object
 * @param instruction - Load\Unload --> L\U
 * @param srcPortName - the source of the container
 * @return container.
 */
std::unique_ptr<Container> createContainer(SimulatorObj* sim,map<string,list<string>> &rawData,string& id, string& instruction,string& srcPortName) {
    vector<string> parsedInfo;
    std::unique_ptr<Container> cont;
    if (instruction == "L") {
        auto srcPort = (sim->getShip()->getPortByName(srcPortName));
        /*Case we load container exists in the raw Data*/
        if (rawData.find(id) != rawData.end()) {
            parsedInfo = stringSplit(rawData[id].front(), delim);
            auto dstPort = (sim->getShip()->getPortByName(parsedInfo[2]));
            int kg = atoi(parsedInfo[1].data());
            cont = std::make_unique<Container>(id, kg, srcPort, dstPort);
        }
            /*Case we load container that unloaded before and now loaded*/
        else {
            for (auto &container : *sim->getPort()->getContainerVec(Type::PRIORITY))
                if (container.getId() == id) {
                    cont = std::make_unique<Container>(container.getId(), container.getWeight(),srcPort , container.getDest());
                }

        }
    }
    if(instruction == "U"){
        std::tuple<int,int,int> tup = sim->getShip()->getCoordinate(id);
        if(std::get<0>(tup) == -1 || std::get<1>(tup) == -1 || std::get<2>(tup) == -1)
            return nullptr;
        auto srcPort = (sim->getShip()->getPortByName(srcPortName));
        auto &container = (sim->getShip()->getMap())[std::get<0>(tup)][std::get<1>(tup)][std::get<2>(tup)];
        cont = std::make_unique<Container>(container.getId(),container.getWeight(),container.getDest(),srcPort);
    }

    return cont;
}

/**
 * This function trim spaces from left and from right
 */
void trimSpaces(string& str){
    //right trim
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), str.end());

    //left trim
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}


