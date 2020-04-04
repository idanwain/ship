#include "lifo_algorithm.h"

void  Lifo_algorithm::get_instructions_for_craine(std::ofstream& outfile)  {
    //port->import_containers(ship);
    std::vector<Container> unload = port->get_containers_to(UNLOAD);
    for (auto & it : unload) {
        if (*(it.get_dest()) == *port && it.is_on_board() == ON_BOARD) {
            port->import_container(ship, it);
        }
    }
    //port->load_containers(ship);
    std::vector<Container> load = port->get_containers_to(LOAD);
    std::vector<Port*> route = ship->get_route();
    std::stack<Container> stack;
    for (auto & route_it : route) {
        for (auto load_it = load.begin(); load_it != load.end(); ++load_it) {
            if (load_it->get_dest() == route_it) {
                stack.push(*load_it);
                load.erase(load_it);
            }
        }
    }
    while (!stack.empty() && ship->has_space() && !(ship->has_weight_prob())) {
        port->load_to_ship(stack, ship);
    }
    while (!stack.empty()) {
        load.emplace_back(stack.top());
        stack.pop();
    }
}