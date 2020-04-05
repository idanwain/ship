#include "port.h"
#include "container.h"
#include "ship.h"
#include <algorithm>
#include <fstream>

int Port::add_container(const Container& container, const std::string& command) {
    if (command != UNLOAD && command != LOAD) {
        return 0;
    }
    if (command == LOAD) {
        load.emplace_back(container);
    }
    else {
        unload.emplace_back(container);
    }
    return 1;
}

int Port::remove_container(const Container& container, const std::string& command) {
    if (command != UNLOAD || command != LOAD) {
        return 0;
    }
    if (command == LOAD) {
        auto it = std::find(load.begin(), load.end(), container);
        if (it != load.end()) {
            load.erase(it);
        }
    }
    else {
        auto it = std::find(unload.begin(), unload.end(), container);
        if (it != unload.end()) {
            unload.erase(it);
        }
    }
    return 1;
}

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

void Port::add_instruction() {
    instructions += 1;
}

const std::string&  Port::get_name() {
    return name;
}


std::vector<Container> Port::get_containers_to(const std::string& command){
    std::vector<Container> vec;
    if (command != UNLOAD || command != LOAD) {
        return vec;
    }
    if (command == LOAD) {
        for (auto & it : load) {
            vec.emplace_back(it);
        };
    }
    else {
        for (auto & it : unload) {
            vec.emplace_back(it);
        };
    }

    return vec;
}

bool Port::operator==(const Port& p)
{
    return this->get_name() == p.name;
}

void Port::write_instruction_to_file(std::ofstream& output , const std::string& command, const std::string& id, const std::tuple<int,int,int>& pos){
    output << command << ", " <<  id << ", " << std::get<0>(pos) << ", " << std::get<1>(pos) << ", " << std::get<2>(pos) << std::endl;
}

/**
* given a ship and a container, the port imports the container from the ship
* containers that are stacked above this container on the ship are moved to the port and added to the "load" list of the port
* containers that the destination port == this port are no longer matter and has removed from the "unload" list of the port
* ^^^ might cause some problems to stowage algorithm ^^^
*/

void Port::import_container(Ship* ship, Container& container, std::ofstream& output, std::vector<Container>& priority_to_load)
{
    std::tuple<int, int, int> position = ship->get_coordinate(container);
    std::vector<Container> column = ship->get_column_at(position);

    for (auto it = column.end() - 1; *it != container;) {
        auto pos = ship->get_coordinate(*it);

        if (!(*(it->get_dest()) == *this)) {
            priority_to_load.emplace_back(*it);
        }

        column.pop_back();
        write_instruction_to_file(output, UNLOAD, it->get_id(), pos);
        --it;

        if (*it == container) {
            write_instruction_to_file(output, UNLOAD, it->get_id(), pos);
            it = column.erase(it);
        }

        this->remove_container(*it, UNLOAD); // stowage algorithm ambiguity
    }
}

void Port::load_to_ship(std::stack<Container>& stack, Ship* ship)
{
    std::tuple<int,int> coordinate = ship->find_first_free_spot();
    ship->add_container(stack.top(), coordinate);
    stack.pop();
}