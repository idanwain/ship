#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include "ship.h"
#include "Parser.h"
#include "stowage_algorithm.h"
#include "lifo_algorithm.h"


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


void getTravelRoute(Ship* &ship, std::istream &inFile) {
    std::vector<Port *> *vec = new std::vector<Port *>();
    string line;
    while (getline(inFile, line)) {
        if (line.at(0) == '#') continue; //comment symbol
        else if (isValidPortName(line)) {
            if (!portAlreadyExist(*vec, line)) {
                Port *p1 = new Port(line);
                vec->emplace_back(p1);
            }
        }
    }
    ship->setRoute(*vec);
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
    std::array<int,3> dim{};
    getDimensions(dim,inFile,str);
    if(dim[0] > ship->getAxis("x") || dim[1] > ship->getAxis("y") || dim[2] > ship->getAxis("z")){
        cout << "One of the provided ship plan constraints exceeding the dimensions of the ship, ignoring..." << endl;
        return;
    }
    else{
        for(int i = 0; i < ship->getAxis("z")-dim[2]; i++){
            (*map)[dim[0]][dim[1]].emplace_back(Block());
        }
        ship->update_free_space(-(ship->getAxis("z") - dim[2]));
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

void extractArgsForShip(std::vector<fs::path> &pathArgs, std::tuple<Ship*, bool> &tup) {
    std::ifstream inFile;
    string line, file_path;
    std::array<int, 3> dimensions{};
    std::vector<Port *> travelRoute;
    std::get<1>(tup) = false;
    Ship* ship;
    if(pathArgs.empty()){
        return;
    }
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
        for(int i = 0; i < (*map).size(); i++){
            for(int j = 0; j < (*map)[i].size(); j++){
                for(int k = 0; k < (*map)[i][j].size(); k++){
                    cout << "size is " << (*map)[i][j].size() << endl;
                    if((*map)[i][j][k].get_id() == "block"){
                        cout << "at " << i << "," << j << "," << k <<endl;
                    }
                }
            }
        }
    }

/**
 * This function returns the full path of file to created for algorithm purposes
 * @param path
 * @param root_path
 * @return
 */
 string getFullOutPutPath(fs::path &path, string &root_path, const string &algName){
    return root_path + OP_MAIN_DIRECTORY + "\\" + path.parent_path().filename().string() + "Out\\" +
    algName + "- " + path.filename().string();

}

int main(int argc, char** argv) {

    string path = argv[argc - 1];
    std::vector<std::vector<fs::path>> directories;
    std::vector<Algorithm *> algVec;
    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);

    for (auto &folder : directories) {
        std::tuple<Ship*, bool> tup;
        extractArgsForShip(folder, tup);
        if (!std::get<1>(tup)) continue;
        Ship* mainShip = std::get<0>(tup);
        initAlgorithmList(algVec, mainShip);
        for (auto &alg : algVec) {
            for (int j = 2; j < folder.size(); j++) {
                string outputPath = getFullOutPutPath(folder.at(j), path,alg->getTypeName());
                alg->operator()(folder.at(j).string(), outputPath);
            }
        }
    }
 }
