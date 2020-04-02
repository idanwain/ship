#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator> 
#include <map>
#include "port.h"
#include "container.h"

#ifndef SHIP_HEADER
#define SHIP_HEADER

class Container; class Port;

#define ON_BOARD 1
#define WIDTH
#define HEIGHT
#define DEPTH


class Ship {
	std::vector<std::vector<std::vector<Container>>> shipMap;
	std::vector<Port*> route;

	/*given a route of ports, the C'tor parses the containers of any port to a map*/
	Ship(int x, int y, int z, std::vector<Port*> route): route(route) {
		//init blank ship plan
		//**********
		//init containers by port
	}

public:
	std::tuple<int, int, int> get_coordinate(const Container& container); //need to implement
	std::vector<Container> get_column_at(std::tuple<int, int, int> position); //need to implement
};

#endif // !SHIP_HEADER
