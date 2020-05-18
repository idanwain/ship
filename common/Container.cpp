#include "Container.h"

int Container::getWeight() {
	return weight;
}

std::string Container::getId() {
	return id;
}

std::shared_ptr<Port>& Container::getDest() {
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
    return this->getId() == c.id;
}

bool Container::operator!=(const Container& c) {
	return *this == c;
}

bool Container::operator<(const Container &c) {
    return distanceFromDest < c.distanceFromDest;
}

void Container::setDistance(int distance) {
    distanceFromDest = distance;
}

std::shared_ptr<Port> &Container::getSrc() {
    return this->source;
}
