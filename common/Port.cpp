#include "Port.h"
#include "Container.h"
#include "Common.h"
#include <algorithm>
#include <fstream>

void Port::addContainer(Container container, Type command) {
    std::cout << "load size is " << load.size() << endl;
    std::cout << "container is " << container << std::endl;
    std::cout << "container id is" << container.getId() << " weight is " << container.getWeight() << endl;
    //Container* con = new Container(container);
    std::cout << " here " << endl;
    if(command == Type::LOAD){
        std::cout << "load " << endl;
        load.emplace_back(container);
    }
    if(command == Type::UNLOAD){
        std::cout << "unload " << endl;
        unload.emplace_back(container);
    }
    if(command == Type::PRIORITY){
        std::cout << "priority " << endl;
        priority.emplace_back(container);
    }
    if(command == Type::ARRIVED){
        std::cout << "arrived " << endl;
        arrived.emplace_back(container);

    }
}

void Port::addContainer(std::string &id, int weight, std::shared_ptr<Port> &src,std::shared_ptr<Port> &dest, Type command) {
    std::cout << "load size is " << load.size() << endl;
    std::cout << " here " << endl;
    if(command == Type::LOAD){
        std::cout << "load " << endl;
        load.emplace_back(Container(id,weight,src,dest));
    }
//    if(command == Type::UNLOAD){
//        std::cout << "unload " << endl;
//        unload.emplace_back(container);
//    }
//    if(command == Type::PRIORITY){
//        std::cout << "priority " << endl;
//        priority.emplace_back(container);
//    }
//    if(command == Type::ARRIVED){
//        std::cout << "arrived " << endl;
//        arrived.emplace_back(container);
//
//    }
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
    priority.clear();
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

void Port::removeContainer(std::string &id, Type command) {
    int i = 0;
    bool found = false;
    if(command == Type::LOAD){
        for(;i < (int)load.size(); i++){
            if(load[i].getId() == id){
                found = true;
                break;
            }
        }
        if(found)
            load.erase(load.begin()+i);
    }
    else if(command == Type::PRIORITY){
        for(; (int)priority.size(); i++){
            if(priority[i].getId() == id){
                found = true;
                break;
            }
        }
        if(found)
            priority.erase(priority.begin()+i);
    }

}
