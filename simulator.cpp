#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include "ship.h"

#define lineSeparatorNum 3

using std::cout;
using std::endl;
using std::string;
const char delim[] = {',','\t','\r',' '};

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

/**
 * This function checks every time if the current line is '###' as it's the pre defined seperator
 * @param line
 * @return 1 iff line has sequence of '###' in it
 */
int isTripleHashTag(std::string line){
    int i = 0,count = 0;
    while(i < lineSeparatorNum){
        if(line.at(i) == '#')count++;
        i++;
    }
    return (count == 3) ? 1 : 0;
}

/*
 * extracting the first 2 lines of the file to get parameters to build new ship
 */
void extractArgsForShip(std::vector<Port*> &vec, std::array<int,3> &arr, std::istream &inFile){
    string line;
    int count = 0;
    while(count < 2){
        getline(inFile,line);
        if(!isTripleHashTag(line)){
            (count == 0) ? getDimensions(arr,line) : getTravelRoute(vec,line);
            count++;
        }
    }
    //nextLine is ###
    getline(inFile,line);
}

/**
 * This function gets a pointer to a char array and parse the number from it
 * afterward it updates the char array pointer location
 * @param str
 * @return - the number representation of the string.
 */
int getNumber(char** str){
    char* input = *str;
    string s;
    while(input[0] >= '0' && input[0] <= '9'){
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
void getBlocksByLine(std::string &str, Ship &ship){
    auto map = ship.get_map();
    char* input = str.data();
    int x=0,y=0,z=0,counter = 0;
    while(input[0]){
        if(input[0] == ',' || input[0] == '\"'){}
        else if(counter == 0){
            x = getNumber(&input);
            counter++;
        }
        else if(counter == 1){
            y = getNumber(&input);
            counter++;
        }
        else if(counter == 2){
            z = getNumber(&input);
            counter = 0;
            (*map)[x][y][z] = Block();
        }
        input++;
    }

}

/**
 * This function parse line by line from the file, and initialized the block containers in the shipmap
 * @param ship - the ship object to get it's shipMap and update it
 * @param inFile - the file pointer
 */
void extractArgsForBlocks(Ship& ship, std::istream &inFile){
    string line;
    while(getline(inFile,line) && !isTripleHashTag(line)){
            getBlocksByLine(line,ship);
    }
}


void getContainerInformation(std::tuple<string,int,string,string>& tup,string &line){
    int count = 0;
    string* s = new string();
    for(char i : line){
        if(i != ','){
            s->push_back(i);
        }
        else{
            switch(count){
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
void extractLoadContainers(Ship& ship, std::istream &inFile){
    string line;
    Port* dst;
    Port* src;
    while(getline(inFile,line) && !isTripleHashTag(line)){
        std::tuple<string,int,string,string> tup;
        getContainerInformation(tup,line);
        dst = ship.getPortByName(std::get<2>(tup));
        src = ship.getPortByName((std::get<3>(tup)));
        Container* c = new Container(std::get<0>(tup),std::get<1>(tup),src,dst);
        /* TODO need to discuss the correctness of this , we might need change the data structures, it's better to pass the pointer*/
        dst->add_container(*c,"unload"); //This is not good as this are 2 different containers
        src->add_container(*c,"load");
    }
}


int main(int argc, char** argv) {

    std::string line;
    std::ifstream inFile;
    inFile.open(argv[1]);

    if(inFile.fail()){
        cout << "Error Opening file, closing program" << endl;
        exit(1);
    }

    /*This block of code builds the route and the ship map including getting the storage constraints*/
    std::array<int,3> dimensions{};
    std::vector<Port*> travelRoute;
    extractArgsForShip(travelRoute, dimensions, inFile);
    Ship* mainShip = new Ship(dimensions[0],dimensions[1],dimensions[2],travelRoute);
    extractArgsForBlocks(*mainShip, inFile);

    while(!inFile.eof()){
        getline(inFile,line);
        if(!isTripleHashTag(line)){
            extractLoadContainers(*mainShip,inFile);
        }
    }
    //TODO delete --> Debugging Purpose
    for(const auto& element : travelRoute){
        cout << element << endl;
    }

}