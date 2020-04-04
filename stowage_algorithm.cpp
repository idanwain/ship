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

    //parse the input data into
    parse_data_to_port(input_full_path_and_file_name);

    //creating an output file
    std::ofstream output (output_full_path_and_file_name);
    get_instructions_for_craine(output);
    output.close();
    ++Algorithm::portNum;
    return 1;
}

 /**
  * Parses the containers data and connecting it to the "load" list of the port
  * @param input_full_path_and_file_name
  */
void  Algorithm::parse_data_to_port(const std::string& input_full_path_and_file_name){
    std::string line;
    std::ifstream input;
    input.open(input_full_path_and_file_name);
    if(input.fail()){
        std::cout << "Error Opening file, closing program" << std::endl;
        exit(1);
    }
    while(getline(input,line)){
        std::string id; int weight; Port *dest = nullptr;
        extract_containers_data(line, id, weight, dest);
        const Container* con = new Container(id, weight, this->port, dest);
        this->port->add_container(*con, LOAD);
    }
    input.close();
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
void  Algorithm::extract_containers_data(const std::string& line, std::string &id, int &weight, Port* dest) {
    int i=0;
    auto data = string_split(line, delim);
    for(std::string item : data){
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
                dest = ship->getPortByName(item);
                break;
        }
    }
}