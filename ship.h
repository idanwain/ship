#include <iostream>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
#include <map>
#include "port.h"
#include "container.h"
#include <stack>

#define ON_BOARD 1
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
        this->shipMap = collection;

//        std::vector<std::vector<std::vector<Container>>> s(x,std::vector<std::vector<Container>>(y,std::vector<Container>(z)));
//        shipMap = s;
    }
    std::tuple<int, int, int> get_coordinate(const Container& container); //need to implement
    std::vector<Container> get_column_at(std::tuple<int, int, int> position); //need to implement
    std::vector<Port*> get_route();
    std::vector<std::vector<std::vector<Container>>>* get_map();
    bool has_space(); //need to implement
    bool has_weight_prob(); //need to implement
    void add_container(const Container& container, Port* port); //need to impelment
    Port* getPortByName(const std::string &name);

};

#endif // !SHIP_HEADER