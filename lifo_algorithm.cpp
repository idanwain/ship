#include "lifo_algorithm.h"
#include "port.h"
#include "Parser.h"

void  Lifo_algorithm::get_instructions_for_crane(std::ofstream& output) {
    //port->import_containers(ship);
    std::vector<Container> unload;
    ship->get_containers_to_unload(port, unload); //containersByPort
    std::vector<Container> priority_to_load;
    for (auto it_con = unload.begin(); it_con != unload.end();) {
            if(*(it_con->get_dest()) == *port){
                //add to port to arrived containers //TODO add arrived vec
                port->add_container(*it_con, "A");
            }
            else {
                //add to priority vec
                priority_to_load.emplace_back(*it_con);
            }
            //write to file
            std::tuple<int,int,int> position = ship->get_coordinate(*it_con);

            std::vector<Container> column = ship->get_column_at(position); //shipMap
            Port::write_instruction_to_file(output, "U", (it_con)->get_id(), position);
            //delete from ship
                //delete from shipMap
                for(auto container = column.end() - 1; !column.empty() && *container != *it_con;) {
                    column.erase(container);
                    container--;
                }
                if(!column.empty()){
                    column.pop_back();
                }
                //delete from containersByPort
                if(!unload.empty()){
                    it_con = unload.erase(it_con);
                }
    }

    //port->load_priority_containers(ship)
    for(auto& con : priority_to_load){
        ship->add_container(con,ship->find_min_floor());
        Port::write_instruction_to_file(output, "L", con.get_id(), ship->get_coordinate(con));
    }

    //port->load_containers(ship);
    //stack method
    std::vector<Container> load;
    port->get_containers_to_load(load);
    std::vector<Port*> route = ship->get_route();
//    std::stack<Container*> stack;
    for (auto & route_it : route) {
        for (auto load_it = load.end() - 1; !load.empty() && load_it != load.begin();) {
            if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
                port->load_to_ship(*load_it, ship);
                Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
                load.erase(load_it);
//                stack.push(&(*load_it));
            }
            --load_it;
            if(load_it == load.begin()){
                if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
                    port->load_to_ship(*load_it, ship);
                    Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
                    load.erase(load_it);
//                stack.push(&(*load_it));
                }
            }
        }
//    }
//    // load to ship in order
//    while (!stack.empty() && ship->has_space() && !(ship->has_weight_prob())) {
//        Container* con = stack.top();
////        port->load_to_ship(*con, ship);
////        Port::write_instruction_to_file(output, "L", (*con).get_id(), ship->get_coordinate(*con));
//        stack.pop();
//    }
//
//    //rejected containers
//    while (!stack.empty()) {
//        load.emplace_back(*(stack.top()));
//        Port::write_instruction_to_file(output, "R", stack.top()->get_id(), ship->get_coordinate(*(stack.top())));
//        stack.pop();
//    }
    }
}

const std::string Lifo_algorithm::getTypeName() const {
    return this->name;
}

