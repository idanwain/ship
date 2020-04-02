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
#include <iterator> 
#include <vector>
#include<stack>


#define UNLOAD "unload"
#define LOAD "load"

class Container;
class Ship;

class Port {
	std::string name;
	std::vector<Container> load;
	std::vector<Container> unload;
	int instructions;
public:

	/*C'tor*/
	Port(const std::string& name) : name(name), instructions(0){}
	
	int add_container(const Container& container, std::string command);
	int remove_container(const Container& container, std::string command);
	void print_containers();
	void add_instruction();
	std::string const get_name();
	std::vector<Container> get_containers_to(std::string command);
	bool operator==(const Port& p);
	void import_container(Ship* ship, Container& container);
	void load_to_ship(std::stack<Container>& stack, Ship* ship); //need to implement

};
#endif