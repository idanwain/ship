/*
* This module represents a container.
* each container has a:
* -id:
*	5 unique characters.
* -weight:
*	container's weight in kg.
* -Ports:
*	#source port.
*	#destination port.
* -status:
*	represents the current status of a container.
*
*
*      *******      Functions      ******
*
* change_status - given a state, changes the status of the container
* get_weight - returns the weight of the container
* get_id - return the ID of the container
* get_source - returns a pointer to the container's source port
* get_dest - returns a pointer to the container's destination port
* operator<< - returns an ostream that descripts the container
*
*/

#ifndef CONTAINER_HEADER
#define CONTAINER_HEADER




#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator>
#include "port.h"

enum State { error, success , on_ship , pending };
class Port;

class Container {
	std::string id;
	int weight;
	Port* source;
	Port* destination;
	State status;
	
public:
	Container(const std::string& id, int weight, Port* const source, Port* const dest):
		id(id),
		weight(weight),
		source(source),
		destination(dest),
		status(pending) {}

	void change_status(State status);
	int get_weight();
	std::string const get_id();
	Port* const get_source();
	Port* const get_dest();
	std::ostream& operator<<(std::ostream& os);
	bool operator==(const Container& c);
};

#endif // !1