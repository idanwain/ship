#include "port.h"
#include "container.h"
#include "ship.h"
#include "Parser.h"
#include <algorithm>
#include <fstream>

int Port::add_container(const Container& container, const std::string& command) {
    if (command != "U" && command != "L" && command != "A" && command != "P") {
        return 0;
    }
    if (command == "L") {
        load.emplace_back(container);
    }
    else if(command == "U") {
        unload.emplace_back(container);
    }
    else if(command == "A"){
        arrived.emplace_back(container);
    }
    else{
        priority.emplace_back(container);
    }
    return 1;
}

const std::string&  Port::get_name() {
    return name;
}

void Port::get_containers_to_load(std::vector<Container>** vec, char list_category){
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

void Port::write_instruction_to_file(std::ofstream& output , const std::string& command, const std::string& id, const std::tuple<int,int,int>& pos){
    output << command << ", " <<  id << ", " << std::get<0>(pos) << ", " << std::get<1>(pos) << ", " << std::get<2>(pos) << std::endl;
}

void Port::load_to_ship(Container& container, Ship* ship)
{
    std::tuple<int,int> coordinate = ship->find_min_floor();
    ship->add_container(container, coordinate);
//    container.getOnBoard();
    instructions++;
}

Port::~Port(){
    unload.clear();
    load.clear();
    arrived.clear();
}

/**
* given a ship and a container, the port imports the container from the ship
* containers that are stacked above this container on the ship are moved to the port and added to the "load" list of the port
* containers that the destination port == this port are no longer matter and has removed from the "unload" list of the port
* ^^^ might cause some problems to stowage algorithm ^^^
*/
//
//void Port::import_container(Ship* ship, Container& container, std::ofstream& output, std::vector<Container>& priority_to_load)
//{
//    std::tuple<int, int, int> position = ship->get_coordinate(container);
//    std::vector<Container> column = ship->get_column_at(position);
//
//    for (auto it = column.end() - 1; *it != container;) {
//        auto pos = ship->get_coordinate(*it);
//
//        if (!(*(it->get_dest()) == *this)) {
//            priority_to_load.emplace_back(*it);
//        }
//
//        column.pop_back();
//        write_instruction_to_file(output, "U", it->get_id(), pos);
//        --it;
//
//        if (*it == container) {
//            write_instruction_to_file(output, "U", it->get_id(), pos);
//            it = column.erase(it);
//        }
//
//        this->remove_container(*it, "U"); // stowage algorithm ambiguity
//    }
//}

//
//int Port::remove_container(const Container& container, const std::string& command) {
//    if (command != "U" || command != "L") {
//        return 0;
//    }
//    if (command == "L") {
//        auto it = std::find(load.begin(), load.end(), container);
//        if (it != load.end()) {
//            load.erase(it);
//        }
//    }
//    else {
//        auto it = std::find(unload.begin(), unload.end(), container);
//        if (it != unload.end()) {
//            unload.erase(it);
//        }
//    }
//    return 1;
//}

/* for debugging purpose only */
void Port::print_containers() {

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
