#include "port.h"
#include "container.h"


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
		load.erase((std::find(load.begin(), load.end(), container)));
	}
	else {
		unload.erase((std::find(unload.begin(), unload.end(), container)));
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

std::vector<Container> Port::get_containers() {
	std::vector<Container> vec;
	for (auto it = unload.begin(); it != unload.end(); ++it) {
		vec.emplace_back(*it);
	};

	for (auto it = load.begin(); it != load.end(); ++it) {
		vec.emplace_back(*it);
	};

	return vec;
}
