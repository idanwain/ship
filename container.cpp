#include "port.h"
#include "container.h"

int Container::change_status(std::string command, Port* port) {
	if (command != UNLOAD || command != LOAD) {
		return 0;
	}
	if (command == LOAD) {
		this->status.on_board = 1;
		this->status.port = nullptr;
	}
	else {
		this->status.on_board = 0;
		this->status.port = port;
	}
	return 1;
}

int Container::get_weight() {
	return weight;
}

std::string const Container::get_id() {
	return id;
}

Port* const Container::get_source() {
	return source;
}

Port* const Container::get_dest() {
	return destination;
}

int Container::is_on_board() {
	return status.on_board;
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
	if (this->get_id() == c.id) {
		return true;
	}
	return false;
}

bool Container::operator!=(const Container& c)
{
	return *this == c;
}
