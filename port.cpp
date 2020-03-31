#include "port.h"
#include "container.h"

/* adds a container to port's list mentiond (containers to load / unload) */
int Port::add_container(const Container& container, std::string command) {
	if (command != UNLOAD || command != LOAD) {
		return 0;
	}
	if (command == LOAD) {
		load.emplace_front(container);
	}
	else {
		unload.emplace_front(container);
	}
	return 1;
}

/* removes a container from port's list mentiond (containers to load / unload) */
int Port::remove_container(const Container& container, std::string command) {
	if (command != UNLOAD || command != LOAD) {
		return 0;
	}
	if (command == LOAD) {
		load.remove(container);
	}
	else {
		unload.remove(container);
	}
	return 1;
}

/* print all containers to load & unload assigned to this port */
void Port::print_containers() {

	std::cout << "Containters to unload from port " << this->name << ":" << std::endl;
	for (auto it = unload.begin(); it != unload.end(); ++it) {
		std::cout << &it;
	};

	std::cout << "Containters to load from port " << this->name << ":" << std::endl;
	for (auto it = load.begin(); it != load.end(); ++it) {
		std::cout << &it;
	};
}

void Port::add_instruction() {
	instructions += 1;
}

std::string const Port::get_name() {
	return name;
}

std::list<Container> Port::get_containers() {
	std::list<Container> lst;
	for (auto it = unload.begin(); it != unload.end(); ++it) {
		lst.emplace_back(it);
	};

	std::cout << "Containters to load from port " << this->name << ":" << std::endl;
	for (auto it = load.begin(); it != load.end(); ++it) {
		lst.emplace_back(it);
	};

	return lst;
}
