#include "common.h"

void execute(Ship* ship, char command, Container* container, coordinate origin, coordinate dest){
    switch(command){
        case 'L': ship->addContainer(*container,origin); break;
        case 'U': ship->removeContainer(origin);  break;
        case 'M': ship->moveContainer(origin, dest);  break;
        default: std::cout << "Invalid command, please insert L/U/M commands." << std::endl;
    }
}

bool validateContainerData(const std::string& line, VALIDATION& reason, std::string& id, Ship* ship) {
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
            bool weight = is_number(item);
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


bool isValidPortName(const std::string& name){
    std::regex reg("\\s*[A-Z]{2}\\s+[A-Z]{3}s*$");
    return std::regex_match(name,reg);
}

bool is_number(const std::string& s) {
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


bool validateId(const std::string& str) {
    int i = 0;
    if (str.length() != 11)
        return false;
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

bool idExistOnShip(const std::string& id, Ship* ship){
    auto map = ship->getContainersByPort();
    for(auto & pPort : map){
        for(auto con_it : pPort.second) {
            if(con_it.get_id() == id){
                return true;
            }
        }
    }
    return false;
}


bool isPortInRoute(Port *pPort, const std::vector<Port*>& route, int portNum) {
    bool found = false;
    for(auto port_it = route.begin() + portNum + 1; port_it != route.end(); ++port_it){
        if(*(*port_it) == *pPort){
            found = true;
            break;
        }
    }
    return pPort->get_name() != "NOT_IN_ROUTE" && found;
}

