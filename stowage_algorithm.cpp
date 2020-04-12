#include "stowage_algorithm.h"

/**
 *
 */
 /**
  * Given a file path to a list of containers, this function writes into a file
  * the set of instructions needed to be executed to LOAD / UNLOAD the port's containers.
  * @param input_full_path_and_file_name
  * @param output_full_path_and_file_name
  * @return 1
  */
bool Algorithm::operator()(const std::string& input_full_path_and_file_name,
                const std::string& output_full_path_and_file_name) {
    //init algorithm's port
    this->port = ship->get_route().at(portNum);
     std::ofstream output(output_full_path_and_file_name);
    //parse the input data into
    if(!parse_data_to_port(input_full_path_and_file_name, output)){
        std::cout << CONTAINER_FILE_ERROR << std::endl;
        return false;
    };

    //write to output
    get_instructions_for_crane(output);
    output.close();
    ++Algorithm::portNum;
    return true;
}

void Algorithm::write_to_output(std::ofstream& output,
        const std::string& command, const std::string& id,
        const std::tuple<int,int,int>& pos, const std::tuple<int,int,int>& moved_to){
    if(command == "R"){
        output << command << ", " <<  id << std::endl;
    } else if (command == "M") {
        output << command << ", " <<  id << ", " << std::get<0>(pos) <<
                ", " << std::get<1>(pos) << ", " << std::get<2>(pos) <<
                ", [" << std::get<0>(moved_to) << ", " << std::get<1>(moved_to) <<
                ", " << std::get<2>(moved_to) << "]" << std::endl;
    } else {
        output << command << ", " <<  id << ", " << std::get<0>(pos) << ", " << std::get<1>(pos) << ", " << std::get<2>(pos) << std::endl;
    }
}

 /**
  * Parses the containers data and connecting it to the "load" list of the port
  * @param input_full_path_and_file_name
  */
bool  Algorithm::parse_data_to_port(const std::string& input_full_path_and_file_name, std::ofstream &output) {
    std::string line;
    std::ifstream input;

    input.open(input_full_path_and_file_name);

    if(input.fail()){
        std::cout << "Error Opening file, closing program" << std::endl;
        exit(1);
    }

    while(getline(input,line)){
        std::string id; int weight; Port *dest = nullptr;
        if(!validate_container_data(line, output)){
            std::cout << id << ": "<< CONTAINER_NOT_VALID << std::endl;
        }
        else {
            extract_containers_data(line, id, weight, &dest);
            if(dest == nullptr) {
                std::cout << id << ": "<< CONTAINER_NOT_IN_ROUTE << std::endl;
                Algorithm::write_to_output(output,"R", id,std::forward_as_tuple(-1,-1,-1) , std::forward_as_tuple(-1,-1,-1));
            }
            else {
                Container* con = new Container(id, weight, this->port, dest);
                this->port->add_container(*con, "L");
                //con->getOffBoard();
            }
        }
    }

    input.close();
    return true;
}

/**
 * util function that split strings by a given delimiter
 * @param s - string to be splitted
 * @param delimiter - delimiters to be splitted by
 * @return vector of words in s
 */
std::vector<std::string>  Algorithm::string_split(std::string s, const char* delimiter) {
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
    }

    return output;
}

/**
 * parses the data from a given line
 * @param line - data line of container
 * @param id - container's ID
 * @param weight - container's weight
 * @param dest - container's destination
 */
void  Algorithm::extract_containers_data(const std::string& line, std::string &id, int &weight, Port** dest) {
    int i=0;
    auto data = string_split(line, delim);
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
    if(dest_temp != nullptr){
        *dest = dest_temp;
    }
}

bool Algorithm::validate_id(const std::string& str) {
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

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
                     s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool Algorithm::validate_container_data(const std::string& line, std::ofstream &output) {
    int i=-1;
    auto data = string_split(line, delim);

    if(data.size() != 4)
        return false;

    std::string port_name;

    for(const std::string& item : data){
        ++i;
        if (i == 0) {
            bool id = validate_id(item);
            if(!id){
                Algorithm::write_to_output(output, "R", item, std::forward_as_tuple(-1,-1,-1), std::forward_as_tuple(-1,-1,-1));
                return false;
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
    return dest;
}

void Algorithm::increase_instruction_counter(int instructionsAdded) {
    this->instructions += instructionsAdded;
}

int Algorithm::getPortNum() {
    return portNum;
}

bool Algorithm::isPortInRoute(Port *pPort) {
    bool found = false;
    auto route = ship->get_route();
    for(auto port_it = route.begin() + portNum + 1; port_it != route.end(); ++port_it){
        if(*(*port_it) == *pPort){
            found = true;
            break;
        }
    }
    return pPort->get_name() != "NOT_IN_ROUTE" && found;
}


int Algorithm::getInstructionsCounter() const {
    return this->instructions;
}

void Algorithm::initContainersDistance(std::vector<Container> &vector) {
    auto route = this->ship->get_route();
    for(auto pPort = route.rbegin(); pPort != route.rend(); ++pPort){
        int distance = std::distance(pPort, route.rend() - portNum);
        if(distance <= 0){
            break;
        }
        for(auto& con : vector){
            if((*(*pPort)) == *(con.get_dest())){
                con.setDistance(distance);
            }
        }
    }
}
