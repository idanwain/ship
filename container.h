/**
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
    std::string id;
	int weight;
	Port* source = nullptr;
	Port* destination = nullptr;
	int distanceFromDest;

public:
	Container(const std::string& id, int weight, Port* const source, Port* const dest) :
		id(id),
		weight(weight),
		source(source),
		destination(dest),
        distanceFromDest(INT_MAX)
		{}
    /*Container copy c'tor*/
    Container(const Container* contToCopy){
	    this->id = contToCopy->id;
	    this->weight = contToCopy->weight;
	    this->source = contToCopy->source;
	    this->destination = contToCopy->destination;
	    this->distanceFromDest = contToCopy->distanceFromDest;
	    std::cout << "in container c'tor" + this->id << std::endl;
	}

	Container(const std::string& id1): id(id1), weight(-1), source(nullptr), destination(nullptr){}
    ~Container();
	int get_weight();
	std::string get_id();
	Port* get_dest();
    friend std::ostream& operator<<(std::ostream& os, const Container& c);
	bool operator==(const Container& c);
	bool operator!=(const Container& c);
    bool operator <(const Container& c);
    void setDistance(int distance);

};

class Block : public Container{
public:
    explicit Block(const std::string &id1 = "block") : Container(id1){}
};

#endif // !1