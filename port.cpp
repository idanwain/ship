#include "port.h"
#include "container.h"
#include "common.h"
#include <algorithm>
#include <fstream>

void Port::addContainer(Container& container, Type command) {
    switch(command){
        case Type::LOAD: load.emplace_back(container); break;
        case Type::UNLOAD: unload.emplace_back(container); break;
        case Type::ARRIVED: arrived.emplace_back(container); break;
        case Type::PRIORITY: priority.emplace_back(container); break;
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

std::vector<Container>* Port::getContainerVec(Type type){
    switch(type){
        case Type::LOAD: return &this->load;
        case Type::UNLOAD: return &this->unload;
        case Type::PRIORITY: return &this->priority;
        case Type::ARRIVED: return &this->arrived;
        default: return nullptr;
    }
}
