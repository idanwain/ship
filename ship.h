#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator> 
#include <map>
#include "port.h"

#ifndef SHIP_HEADER
#define SHIP_HEADER



class Container; class Port;


#define WIDTH
#define HEIGHT
#define DEPTH


class Ship {
	std::vector<std::vector<std::vector<Container>>> shipMap;
	std::vector<Port> route;

	/*given a route of ports, the C'tor parses the containers of any port to a map*/
	Ship(int x, int y, int z, std::vector<Port> route): route(route) {
		//init blank ship plan
		//**********
		//init containers by port
	}

public:

};

#endif // !SHIP_HEADER
