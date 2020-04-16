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
* -distanceFromDest:
*   distance from its closest destination port; if port is not in route, distanceFromDest == INT_MAX
*
*      *******      Functions      ******
*
* get_weight - returns the weight of the container.
* get_id - return the ID of the container.
* get_dest - returns a pointer to the container's destination port.
* operator<< - returns an ostream that describes the container.
* bool operator== - return true if both IDs are equal, false otherwise.
* bool operator!= - return !(operator==).
* bool operator < - return true if this->distanceToDest is less then other->distanceToDest, false otherwise.
* void setDistance - sets distanceFromDest.
*
*/

#ifndef CONTAINER_HEADER
#define CONTAINER_HEADER

#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator>
#include <climits>
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