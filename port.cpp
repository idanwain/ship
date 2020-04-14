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

void Port::getContainersToLoad(std::vector<Container>** vec, char list_category){
    if(list_category == 'L'){
        *vec = &load;
    } else if(list_category == 'P'){
        *vec = &priority;
    }
}

bool Port::operator==(const Port& p)
{
    return this->get_name() == p.name;
}

Port::~Port(){
    unload.clear();
    load.clear();
    arrived.clear();
}

/* for debugging purpose only */
void Port::printContainers() {

    std::cout << "Containers to unload from port " << this->name << ":" << std::endl;

    for (auto & it : unload) {
        std::cout << it;
    };

    std::cout << "Containers to load from port " << this->name << ":" << std::endl;
    for (auto & it : load) {
        std::cout << it;
    };
}

std::vector<Container>* Port::getContainerVec(const std::string &type){
    if(type == "load")
        return &this->load;
    else if(type == "unload")
        return &this->unload;
    else if(type == "priority")
        return &this->priority;
    else if(type == "arrived")
        return &this->arrived;

    return nullptr;
}
