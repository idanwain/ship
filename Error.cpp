#include "Error.h"

std::ostream &operator<<(std::ostream &os, const Error &e) {
    std::string reason;
    if(e.reason == weight){
        reason = "weight";
    }
    else if (e.reason == space) {
        reason = "space";
    }

    os << "Container's ID: " << e.pCon->get_id() <<
    ",\t" << "Destination port: " << e.pCon->get_dest() <<
    ",\t" << "Current port: " << e.pPort->get_name() <<
    ",\t" << "Error: " << reason << " problem" << std::endl;

    return os;
}
