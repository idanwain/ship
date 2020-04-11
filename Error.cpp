#include "Error.h"

std::ostream &operator<<(std::ostream &os, const Error &e) {
    os << "Container's ID: " << e.pCon->get_id() <<
    ",\t" << "Destination port: " << e.pCon->get_dest() <<
    ",\t" << "Current port: " << e.pPort->get_name() << std::endl;

    return os;
}
