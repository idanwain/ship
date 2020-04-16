#include "port.h"
#include "container.h"
#include <algorithm>
#include <fstream>

void Port::addContainer(Container& container, char command) {
    switch(command){
        case 'L': load.emplace_back(container); break;
        case 'U': unload.emplace_back(container); break;
        case 'A': arrived.emplace_back(container); break;
        case 'P': priority.emplace_back(container); break;
        default: std::cout << "Invalid command, please enter L/U/A/P." << std::endl;
    }
}

const std::string&  Port::get_name() {
    return name;
}

bool Port::operator==(const Port& p){
    return this->get_name() == p.name;
}

Port::~Port(){
    unload.clear();
    load.clear();
    arrived.clear();
}

std::vector<Container>* Port::getContainerVec(char type){
    if(type == 'L')
        return &this->load;
    else if(type == 'U')
        return &this->unload;
    else if(type == 'P')
        return &this->priority;
    else if(type == 'A')
        return &this->arrived;

    return nullptr;
}
