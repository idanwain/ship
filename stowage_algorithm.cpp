#include "stowage_algorithm.h"

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
    this->port = ship->getRoute().at(portNum);
    std::ofstream output(output_full_path_and_file_name);
     //parse the input data into
    if(!parseDataToPort(input_full_path_and_file_name, output)){
        std::cout << CONTAINER_FILE_ERROR << std::endl;
        return false;
    };
    //write to output
    getInstructionsForCrane(output);
    output.close();
    ++Algorithm::portNum;
    return true;
}

void Algorithm::writeToOutput(std::ofstream& output,
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
bool  Algorithm::parseDataToPort(const std::string& inputFullPathAndFileName, std::ofstream &output) {
    std::string line;
    std::ifstream input;

    input.open(inputFullPathAndFileName);

    if(input.fail()){
        std::cout << "Error Opening file, closing program" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(getline(input,line)){
        if (line.at(0) == '#') continue; //comment symbol
        std::string id; int weight; Port *dest = nullptr;
        VALIDATION reason = VALIDATION::Valid;
        if(validateContainerData(line, reason, id, this->ship)) {
            extractContainersData(line, id, weight, &dest);
            if(dest == nullptr) {
                std::cout << id << ": "<< CONTAINER_NOT_IN_ROUTE << std::endl;
                Algorithm::writeToOutput(output,"R", id,std::forward_as_tuple(-1,-1,-1) , std::forward_as_tuple(-1,-1,-1));
            }
            else {
                Container* con = new Container(id, weight, this->port, dest);
                this->port->addContainer(*con, 'L');
            }
        }
        else {
            if(reason != VALIDATION::Valid){
                Algorithm::writeToOutput(output, "R", id, std::forward_as_tuple(-1,-1,-1), std::forward_as_tuple(-1,-1,-1));
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
void  Algorithm::extractContainersData(const std::string& line, std::string &id, int &weight, Port** dest) {
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
    if(dest_temp != nullptr){
        *dest = dest_temp;
    }
}

void Algorithm::increaseInstructionCounter(int instructionsAdded) {
    this->instructions += instructionsAdded;
}

int Algorithm::getInstructionsCounter() const {
    return this->instructions;
}

void Algorithm::initContainersDistance(std::vector<Container> &vector) {
    auto route = this->ship->getRoute();
    for(auto pPort = route.rbegin(); pPort != route.rend(); ++pPort){
        int distance = std::distance(pPort, route.rend() - portNum);
        if(distance <= 0){
            break;
        }
        for(auto& con : vector){
            if((*(*pPort)) == *(con.getDest())){
                con.setDistance(distance);
            }
        }
    }
}

Ship* Algorithm::getShip() const {
    return this->ship;
}

int Algorithm::getPortNum() {
    return portNum;
}
