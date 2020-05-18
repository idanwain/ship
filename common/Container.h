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
* getWeight         - returns the weight of the container.
* getId             - return the ID of the container.
* getDest           - returns a pointer to the container's destination port.
* operator<<        - returns an ostream that describes the container.
* bool operator==   - return true if both IDs are equal, false otherwise.
* bool operator!=   - return !(operator==).
* bool operator <   - return true if this->distanceToDest is less then other->distanceToDest, false otherwise.
* void setDistance  - sets distanceFromDest.
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
#include <memory>
#include <utility>
#include "Port.h"

class Port;

class Container {
    std::string id;
	int weight{};
    std::shared_ptr<Port> source;
    std::shared_ptr<Port> destination;
	int distanceFromDest{};

public:
	Container(std::string id, int weight, std::shared_ptr<Port>& _source, std::shared_ptr<Port>& _dest) :
		id(std::move(id)),
		weight(weight),
        source(_source),
        destination(_dest),
        distanceFromDest(INT_MAX){
	    std::cout << " in container contstructor" << std::endl;
	}

//	Container(Container &&contToCopy){
//        std::cout << "in move c'tor of container " << std::endl;
//        this->id = contToCopy.id;
//        this->weight = contToCopy.weight;
//        this->source = contToCopy.source;
//        this->destination = contToCopy.destination;
//        this->distanceFromDest = contToCopy.distanceFromDest;
//        std::cout << "finishing move c'tor of container " << std::endl;
//	}
//    /*Container copy c'tor*/
//    Container(const Container& contToCopy){
//        std::cout << "in copy c'tor of container " << std::endl;
//        this->id = contToCopy.id;
//	    this->weight = contToCopy.weight;
//        this->source = contToCopy.source;
//        this->destination = contToCopy.destination;
//        this->distanceFromDest = contToCopy.distanceFromDest;
//        std::cout << "finishing copy c'tor of container " << std::endl;
//    }
    //Container(){};
	Container(const std::string& id1): id(id1), weight(-1), source(nullptr), destination(nullptr),distanceFromDest(-1){}
    explicit Container(int kg): id("WeightContainer"), weight(kg), source(nullptr), destination(nullptr),distanceFromDest(-1){}

	int getWeight();
	std::string getId();
	std::shared_ptr<Port>& getDest();
	std::shared_ptr<Port>& getSrc();
    friend std::ostream& operator<<(std::ostream& os, const Container& c);
	bool operator==(const Container& c);
	bool operator!=(const Container& c);
    bool operator <(const Container& c);
    void setDistance(int distance);

};
#endif