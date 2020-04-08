#include "port.h"
#include "container.h"
#include "Parser.h"

int Container::change_status(const std::string& command, Port* port) {
	if (command != "U" || command != "L") {
		return 0;
	}
	if (command == "L") {
		this->status->on_board = 1;
		this->status->port = nullptr;
	}
	else {
		this->status->on_board = 0;
		this->status->port = port;
	}
	return 1;
}

int Container::get_weight() {
	return weight;
}

std::string Container::get_id() {
	return id;
}

Port* Container::get_source() {
	return source;
}

Port*  Container::get_dest() {
	return destination;
}

bool Container::is_on_board() {
    return this->onBoard == 1;
}

void Container::getOnBoard(){
    this->onBoard = 1;
}

void Container::getOffBoard(){
    this->onBoard = 0;
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
