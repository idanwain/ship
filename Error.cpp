#include "Error.h"

std::ostream &operator<<(std::ostream &os, const Error &e) {
    os << "Container's ID: " << e.pCon->getId() <<
       ",\t" << "Destination port: " << e.pCon->getDest() <<
       ",\t" << "Current port: " << e.pPort->get_name() << std::endl;

    return os;
}
