#include "port.h"
#include "container.h"

void Container::change_status(State status) {
	this->status = status;
};

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

std::ostream& Container::operator<<(std::ostream& os)
{
	os << "id: " << id <<
		", weight: " << weight <<
		", source: " << source->get_name() <<
		", destination: " << destination->get_name() <<
		std::endl;
	return os;
}

bool Container::operator ==(const Container& c) {
	if (this->get_id() == c.id) {
		return true;
	}
	return false;
}
