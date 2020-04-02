#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include "ship.h"

using std::cout;
using std::endl;
using std::string;
const char delim[] = {',','\t','\r',' '};

int portAlreadyExist(std::vector<Port*> &vec,string &str){
    for(const auto &element : vec ){
        if(element->get_name() == str){
            vec.emplace_back(element);
            return 1;
        }
    }
    return 0;
}

void getDimensions(std::array<int,3> &arr, string& str){
    char* input = strtok(str.data(),delim);
    for(int i = 0; i < 3; i++){
        arr[i] = atoi(input);
        input = strtok(NULL,delim);
    }
}

void getTravelRoute(std::vector<Port*> &vec, string& str){
    char* input = strtok(str.data(),delim);
    while(input != NULL){
        string* s = new string(input);

        if(!portAlreadyExist(vec,*s)){
            Port* p1 = new Port(*s);//TODO needs to make sure d'tor of Ship destroys them.
            vec.emplace_back(p1);
        }
        input = strtok(NULL,delim);
        delete s;
    }
}


/*
 * extracting the first 2 lines of the file to get parameters to build new ship
 */
void exctractArgsForShip(std::vector<Port*> &vec,std::array<int,3> &arr, std::istream &inFile){
    string line;
    getline(inFile,line);
    getDimensions(arr,line);
    getline(inFile,line);
    getTravelRoute(vec,line);
}

int main(int argc, char** argv) {

    std::string line;
    std::ifstream inFile;
    inFile.open(argv[1]);

    if(inFile.fail()){
        cout << "Error Opening file, closing program" << endl;
        exit(1);
    }

    std::array<int,3> dimensions{};
    std::vector<Port*> travelRoute;
    exctractArgsForShip(travelRoute,dimensions,inFile);
    Ship* mainShip = new Ship(dimensions[0],dimensions[1],dimensions[2],travelRoute);

    //TODO delete --> Debugging Purpose
    for(const auto& element : travelRoute){
        cout << element << endl;
    }
    //get line by line from file
    while(getline(inFile,line)){
            char* input = strtok(line.data(),delim);
            //parse each parameter in line
            while(input != NULL){
                cout << input << endl;
                input = strtok(NULL,delim);
            }
    }

}