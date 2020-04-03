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


class Port;

class Container {
	struct Position {
		int on_board;
		Port* port;
	};
	std::string id;
	int weight;
	Port* source;
	Port* destination;
	Position status;

	
	
public:
	Container(const std::string& id, int weight, Port* const source, Port* const dest) :
		id(id),
		weight(weight),
		source(source),
		destination(dest)
		{
		status.on_board = 0;
		status.port = source;
		}
		Container(){};

	int change_status(std::string command, Port* port);
	int get_weight();
	std::string const get_id();
	Port* const get_source();
	Port* const get_dest();
	int is_on_board();
	friend std::ostream& operator<<(std::ostream& os, const Container& c);
	bool operator==(const Container& c);
	bool operator!=(const Container& c);
};

#endif // !1