#include "ship.h"
#include "container.h"

class Algorithm {
	Ship* ship;
	Port* port;
public:
	Algorithm(Ship* ship, Port* port): ship(ship), port(port) {}
	
	bool operator()()const {
		//port->import_containers(ship);
		std::vector<Container> unload = port->get_containers_to(UNLOAD);
		for (auto it = unload.begin(); it != unload.end(); ++it) {
			if (*(it->get_dest()) == *port && it->is_on_board() == ON_BOARD) {
				port->import_container(ship, *it);
			}
		}
		//port->load_containers(ship);
	}
};