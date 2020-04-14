#include "port.h"
#include "container.h"

int Container::get_weight() {
	return weight;
}

std::string Container::get_id() {
	return id;
}

Port*  Container::get_dest() {
	return destination;
}

std::ostream& operator<<(std::ostream& os, const Container& c)
{
	os << "id: " << c.id <<
		", weight: " << c.weight <<
		", source: " << c.source->get_name() <<
		", destination: " << c.destination->get_name() <<
		std::endl;
	return os;
}

bool Container::operator ==(const Container& c) {
    return this->get_id() == c.id;
}

bool Container::operator!=(const Container& c) {
	return *this == c;
}

Container::~Container(){}

bool Container::operator<(const Container &c) {
    return distanceFromDest < c.distanceFromDest;
}

void Container::setDistance(int distance) {
    distanceFromDest = distance;
}
