#include "port.h"
#include "container.h"
#include "ship.h"

int Port::add_container(const Container& container, std::string command) {
	if (command != UNLOAD || command != LOAD) {
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

int Port::remove_container(const Container& container, std::string command) {
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

	std::cout << "Containters to unload from port " << this->name << ":" << std::endl;

	for (auto it = unload.begin(); it != unload.end(); ++it) {
		std::cout << *it;
	};

	std::cout << "Containters to load from port " << this->name << ":" << std::endl;
	for (auto it = load.begin(); it != load.end(); ++it) {
		std::cout << *it;
	};
}

void Port::add_instruction() {
	instructions += 1;
}

std::string const Port::get_name() {
	return name;
}


std::vector<Container> Port::get_containers_to(std::string command){
	std::vector<Container> vec;
	if (command != UNLOAD || command != LOAD) {
		return vec;
	}
	if (command == LOAD) {
		for (auto it = load.begin(); it != load.end(); ++it) {
			vec.emplace_back(*it);
		};
	}
	else {
		for (auto it = unload.begin(); it != unload.end(); ++it) {
			vec.emplace_back(*it);
		};
	}

	return vec;
}

bool Port::operator==(const Port& p)
{
	return this->get_name() == p.name;
}

/*
* given a ship and a container, the port imports the container from the ship
* containers that are stacked above this container on the ship are moved to the port and added to the "load" list of the port
* containers that the destination port == this port are no longer matter and has removed from the "unload" list of the port
* ^^^ might cause some problems to stowage algorithm ^^^
*/

void Port::import_container(Ship* ship, Container& container)
{
	std::tuple<int, int, int> position = ship->get_coordinate(container);
	std::vector<Container> column = ship->get_column_at(position);

	for (auto it = column.end() - 1; *it != container;) {
		if (!(*(it->get_dest()) == *this)) {
			this->add_container(*it, LOAD);
		}
		column.pop_back();

		--it;
		if (*it == container) {
			it = column.erase(it);
		}
		this->remove_container(*it, UNLOAD); // stowage algorithm ambiguity
	}
}

