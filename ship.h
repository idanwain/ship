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
	std::vector<Port*> route;

public:
    /*given a route of ports, the C'tor parses the containers of any port to a map*/
    Ship(int x, int y, int z, std::vector<Port*> route): route(route) {
        typedef std::vector<std::vector<Container>> matrix_container;
        std::vector<matrix_container> collection;
        collection.reserve(z);
        collection.resize(z,matrix_container(x,std::vector<Container>(y)));
        shipMap = collection;

//        std::vector<std::vector<std::vector<Container>>> s(x,std::vector<std::vector<Container>>(y,std::vector<Container>(z)));
//        shipMap = s;
    }

};

#endif // !SHIP_HEADER
