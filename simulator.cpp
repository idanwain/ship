#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include "ship.h"
#include "Parser.h"
#include "stowage_algorithm.h"
#include "lifo_algorithm.h"

#define lineSeparatorNum 3


/**
 * This function checks if port already exist in the vector list, if exists it return 0.
 * @param vec - the vector of ports
 * @param str - the string of the port to be check
 * @return 0 iff exist already port with same name
 */
int portAlreadyExist(std::vector<Port*> &vec,string &str){
    for(const auto &element : vec ){
        if(element->get_name() == str){
            vec.emplace_back(element);
            return 1;
        }
    }
    return 0;
}

void getDimensions(std::array<int,3> &arr, std::istream &inFile,string str){
    string line;
    char* input;
    if(str == "byFile"){
        while(getline(inFile,line)) {
            if(line.at(0) != '#') break; //comment symbol
        }
        input = strtok(line.data(),delim);
        for(int i = 0; i < 3; i++) {
            arr[i] = atoi(input);
            input = strtok(NULL, delim);
        }
    }
    else {
        input = strtok(str.data(),delim);
        for(int i = 0; i < 3; i++) {
            arr[i] = atoi(input);
            input = strtok(NULL, delim);
        }
    }
}

void getActualPortName(string &str){
    string temp;
   int space = 0;
    for(auto const& c : str){
        if(temp.size() == 6) break;
        else if(space && isspace(c))continue;
        else {
            if(isspace(c)) space++;
            temp+=c;
        }
    }
    str = temp;
}

void getTravelRoute(Ship* &ship, std::istream &inFile) {
    std::vector<Port *> *vec = new std::vector<Port *>();
    string line;
    while (getline(inFile, line)) {
        if (line.at(0) == '#') continue; //comment symbol
        else if (isValidPortName(line)) {
            getActualPortName(line);
            string *s = new string(line);
            if (!portAlreadyExist(*vec, *s)) {
                Port *p1 = new Port(*s);
                vec->emplace_back(p1);
            }
            delete s;
        }
    }
    ship->setRoute(*vec);
}

/**
 * This function checks every time if the current line is '###' as it's the pre defined seperator
 * @param line
 * @return 1 iff line has sequence of '###' in it
 */
    int isTripleHashTag(std::string line) {
        int i = 0, count = 0;
        while (i < lineSeparatorNum) {
            if (line.at(i) == '#')count++;
            i++;
        }
        return (count == 3) ? 1 : 0;
    }


/**
 * This function gets a pointer to a char array and parse the number from it
 * afterward it updates the char array pointer location
 * @param str
 * @return - the number representation of the string.
 */
    int getNumber(char **str) {
        char *input = *str;
        string s;
        while (input[0] >= '0' && input[0] <= '9') {
            s += input[0];
            input++;
        }
        *str = input;//Moves the pointer of the pointer to the actual new position in the char array
        return stoi(s);

    }
/**
 * This function gets a string(a line from the file) parse the line to get 3 ints (x,y,z) such ship.getMap()[x][y][z]
 * will be initialized to be block container.
 * @param str - the string to parse
 * @param ship - the ship to get it's map from.
 */
    void getBlocksByLine(std::string &str,Ship* &ship) {
        auto map = ship->get_map();
        std::ifstream inFile;
        std::array<int,3> dim;
        getDimensions(dim,inFile,str);
        if(dim[0] > ship->getAxis("x") || dim[1] > ship->getAxis("y") || dim[2] > ship->getAxis("z")){
            cout << "One of the provided ship plan constraints exceeding the dimensions of the ship, ignoring..." << endl;
            return;
        }
        else{
            for(int i = 0; i < ship->getAxis("z")-dim[2]; i++){
                (*map)[dim[0]][dim[1]][i] = Block();
                //TODO ship.setfreeSpace(ship.getAxis(z)-dim[2])
            }
        }
    }

/**
 * This function parse line by line from the file, and initialized the block containers in the shipmap
 * @param ship - the ship object to get it's shipMap and update it
 * @param inFile - the file pointer
 */
    void extractArgsForBlocks(Ship* &ship, std::istream &inFile) {
        string line;
        while (getline(inFile, line)){
            if(line.at(0) == '#') continue;
            getBlocksByLine(line,ship);
        }
    }


    void getContainerInformation(std::tuple<string, int, string, string> &tup, string &line) {
        int count = 0;
        string *s = new string();
        for (char i : line) {
            if (i != ',') {
                s->push_back(i);
            } else {
                switch (count) {
                    case 0:
                        std::get<0>(tup) = *s;
                        break;
                    case 1:
                        std::get<1>(tup) = stoi(*s);
                        break;
                    case 2:
                        std::get<2>(tup) = *s;
                        break;
                    case 3:
                        std::get<3>(tup) = *s;
                }
                count++;
                s->clear();
            }
        }
    }
    void extractLoadContainers(Ship &ship, std::istream &inFile) {
        string line;
        Port *dst;
        Port *src;
        while (getline(inFile, line) && !isTripleHashTag(line)) {
            std::tuple<string, int, string, string> tup;
            getContainerInformation(tup, line);
            dst = ship.getPortByName(std::get<2>(tup));
            src = ship.getPortByName((std::get<3>(tup)));
            Container *c = new Container(std::get<0>(tup), std::get<1>(tup), src, dst);
            /* TODO need to discuss the correctness of this , we might need change the data structures, it's better to pass the pointer*/
            dst->add_container(*c, "unload"); //This is not good as this are 2 different containers
            src->add_container(*c, "load");
        }
    }


    void extractArgsForShip(std::vector<fs::path> &pathArgs, std::tuple<Ship *, bool> &tup) {
        std::ifstream inFile;
        string line, file_path;
        std::array<int, 3> dimensions{};
        std::vector<Port *> travelRoute;
        std::get<1>(tup) = false;
        Ship* ship;

        for (int i = 0; i < 2; i++) {
            file_path = pathArgs[i].string();
            inFile.open(file_path);
            if (inFile.fail()) {
                cout << "Failed to read from this file path - " + file_path << endl;
                //TODO need to delete the ship if error occurred.
                return;
            } else if (i == 0) {
                getDimensions(dimensions, inFile,"byFile");
                ship = new Ship(dimensions[0], dimensions[1], dimensions[2]);
                extractArgsForBlocks(ship, inFile);
            } else {
                getTravelRoute(ship,inFile);
            }
            inFile.close();
        }
        std::get<0>(tup) = ship;
        std::get<1>(tup) = true;
    }

void initAlgorithmList(std::vector<Algorithm*> &algList,Ship* &ship){
    Algorithm* alg = new Lifo_algorithm(ship);
    algList.emplace_back(alg);
    //TODO need to insert another algo variants/types
}

/*
 * TODO function only for debug purposes --> to delete.
 */
void printWhereBlocks(Ship* &ship){
        auto map = ship->get_map();
        for(int i = 0; i < ship->getAxis("x"); i++){
            for(int j = 0; j < ship->getAxis("y"); j++){
                for(int k = 0; k < ship->getAxis("z"); k++){
                    if((*map)[i][j][k].get_id() == "block"){
                        cout << "at " << i << "," << j << "," << k <<endl;
                    }
                }
            }
        }
    }

int main(int argc, char** argv) {

    string path = argv[argc-1];
    std::vector<std::vector<fs::path>> directories;
    std::vector<Algorithm*> algVec;
    std::tuple<Ship*,bool> tup;
    Ship* mainShip;


    initListDirectories(path,directories);
    validateSequenceDirectories(directories);

    //Cartesian simulator --> Travels X Algorithms
    for(int i = 0; i < directories.size(); i++) {
        extractArgsForShip(directories[i], tup);
        if (!std::get<1>(tup)) continue; //an error occurred while parsing the ship args --> try next folder
            mainShip = std::get<0>(tup);
            initAlgorithmList(algVec, mainShip);
            //printWhereBlocks(mainShip);

    //        for(each algorithm in algVec){
    //            for(each file in directories[i]){
    //                algorithm(input - full path + name);
    //            }
    //        }
    }








}