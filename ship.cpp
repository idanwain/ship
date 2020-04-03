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

