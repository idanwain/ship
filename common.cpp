
#include "common.h"

//Ship* pointer, char - U L M?, coordinate, container*
//check with weight balance calculator
//try operation by calculator
//get calc


void validateAlgorithm(string &path,Ship* simulatorShip){
    std::ifstream inFile;
    string line,id,instruction;
    std::pair<string,string> idAndInstruction;
    vector<int> coordinates;
    inFile.open(path);
    if(inFile.fail()) {
        cout << "Failed to read from this file path - " + path << endl;
        return;
    }
    while(getline(inFile,line)){
        parseInstruction(line,idAndInstruction,coordinates);
        instruction = std::get<0>(idAndInstruction);
        id = std::get<1>(idAndInstruction);
        if(validateInstruction(instruction,coordinates,simulatorShip)){
            coordinate one = std::tuple<int,int>(coordinates[0],coordinates[1]);
            if(instruction == "L") {
                Container *cont = new Container(id);
                executeInstruction(simulatorShip, instruction, one, std::forward_as_tuple(-1, -1), cont);
            }
            else if(instruction == "U"){
                executeInstruction(simulatorShip, instruction, one, std::forward_as_tuple(-1, -1), nullptr);
            }
            else{
                coordinate two = std::tuple<int,int>(coordinates[3],coordinates[4]);
                executeInstruction(simulatorShip, instruction, one, two, nullptr);
            }
        }

    }


}
void parseInstruction(string &toParse,std::pair<string,string> &instruction,vector<int> &coordinates){
    vector<string> parsedInfo = Algorithm::string_split(toParse,delim);
    for(int i = 0; i < parsedInfo.size(); i++){
        if(i == 0)
            std::get<0>(instruction) = parsedInfo.at(0);
        else if(i == 1)
            std::get<1>(instruction) = parsedInfo.at(1);
        else
            coordinates.emplace_back(stoi(parsedInfo.at(i)));
    }
}

bool validateInstruction(std::pair<string,string> &instruction,vector<int> &coordinates,Ship* ship){
    string inst = std::get<0>(instruction);
    string id = std::get<1>(instruction);
    int x1 = coordinates[0],y1 = coordinates[1], z1 = coordinates[2];
    auto map = ship->get_map();
    bool error = false;
    if(inst == "L")
        error =  validateLoadInstruction(x1,y1,z1,*map);
    else if(inst == "U")
        error =  validateUnloadInstruction(x1,y1,z1,*map);
    else{
        error = validateMoveInstruction(coordinates,*map);
    }
    return error;
}

bool validateLoadInstruction(int x,int y,int z,vector<vector<vector<Container>>>& map){
    if((map).at(x).at(y).size() != z)
        /*if not in size --> either in lower level which is wrong, or floating in the air*/
        return false;
    /*else if(check weight balance)
    To exercise 2 need to check also weight*/
    return true;
}

bool validateUnloadInstruction(int x,int y,int z,vector<vector<vector<Container>>>& map){
    if((map).at(x).at(y).size() != z)
        return false;
    /*else if(check weight calculator)
     * To exercise 2 need to check also weight*/
    return true;
}

bool validateMoveInstruction(vector<int> coordinates, vector<vector<vector<Container>>>& map){
    int x1 = coordinates[0], y1 = coordinates[1], z1 = coordinates[2];
    int x2 = coordinates[3], y2 = coordinates[4], z2 = coordinates[5];
    return !validateUnloadInstruction(x1, y1, z1, map) && !validateLoadInstruction(x2, y2, z2, map);
}