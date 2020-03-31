#include <iostream>
#include <cstring>
#include <string>
#include <list> 
#include <iterator> 
#include <map>
#include "port.h"

class Container; class Port;


#define WIDTH
#define HEIGHT
#define DEPTH


class Ship {
	int*** ship_plan; //temp
	std::map<Port, std::list<Container>> containers;
	std::list<Port> route;


	/*given a route of ports, the C'tor parses the containers of any port to a map*/
	Ship(int width, int height, int depth, std::list<Port> route): route(route) {
		//init ship plan
		//**********
		//init containers by port
		std::list<Port>::iterator it;
		for (it = route.begin(); it != route.end(); ++it) {
			if (containers.find(*it) != containers.end()) {
				std::list<Container> cp = it->get_containers();
				containers.insert(std::pair<Port, std::list<Container>>(*it, cp));
			}
		}
	}


};
