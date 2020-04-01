/*
* This module represents a port
* each port has a:
* -name
* -containters:
*	# to load
*	# to unload
* -Instructions:
*	the number of crain moves executed at this port
*
*
*      *******      Functions      ******
*
* add_container - adds a container to port's list mentiond (containers to load / unload) 
* remove_container - removes a container from port's list mentiond (containers to load / unload)
* print_containers - print all containers to load & unload assigned to this port
* add_instruction - adds +1 to instructions counter for crain move
* * get_name - returns the name of the port
* get_containers - retruns a list of all the containers assigned to a port
*
*/
#ifndef PORT_HEADER
#define PORT_HEADER


#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator> 

class Container;

#define UNLOAD "unload"
#define LOAD "load"

class Port {
	std::string name;
	std::list<Container> load;
	std::list<Container> unload;
	int instructions;
public:

	/*C'tor*/
	Port(const std::string& name) : name(name), instructions(0){}
	
	int add_container(const Container& container, std::string command);
	int remove_container(Container& container, std::string command);
	void print_containers();
	void add_instruction();
	std::string const get_name();
	std::list<Container> get_containers();
};
	
#endif