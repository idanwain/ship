#include "ship.h"

/* given a container, returns a tuple of the container's position at the ship */
std::tuple<int, int, int> Ship::get_coordinate(const Container& container)
{
	return std::tuple<int, int, int>();
}

/* given a position, returns a vector of the column of the position */
std::vector<Container> Ship::get_column_at(std::tuple<int, int, int> position)
{
	return std::vector<Container>();
}

std::vector<Port*> Ship::get_route() {
	return this->route;
}

bool Ship::has_space()
{
	return false;
}

bool Ship::has_weight_prob()
{
	return false;
}

void Ship::add_container(const Container& container, Port* port)
{
}

std::vector<std::vector<std::vector<Container>>>* Ship::get_map() {
    return &shipMap;
}

Port* Ship::getPortByName(const std::string &name){
    for(auto port : this->route){
        if(port->get_name() == name){
            return port;
        }
    }
    /*
     * TODO need to deal if no such port exist in the list--> might be if we load a cargo to drop at port but
     * the certain port doesn't exist in the route because we are not stopping at.
     */
}

void Ship::setRoute(std::vector<Port *> &route) {
    this->route = route;
}

int Ship::getAxis(const std::string &str) {
    if(str == "x") return this->x;
    else if(str == "y") return this->y;
    else if(str == "z") return this->z;
    else return -1;

}

