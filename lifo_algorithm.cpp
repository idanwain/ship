#include "lifo_algorithm.h"
#include "port.h"


/**
 * this function manages the load / unload of containers and logs it into a file.
 * @param output - output file to write instructions for crane
 */
void  Lifo_algorithm::get_instructions_for_crane(std::ofstream& output) {
    //unload containers from ship to port
    std::vector<Container> priority_to_load;
    unloadContainers(output, priority_to_load);

    //load unrelated unloaded containers back to ship
    for(auto& con : priority_to_load){
        ship->add_container(con,ship->find_min_floor());
        Port::write_instruction_to_file(output, "L", con.get_id(), ship->get_coordinate(con));
    }

    //load containers from port to ship
    loadContainers(output);
}

/**
 * This function unloads all the containers that need to be unloaded to port by these scheme:
 *      - get all containers to unload
 *      - find the columns of those containers inside the ship
 *      - unload containers that are in those columns to port
 *      - stop unload when hits the port's container
 *
 * @param output - output file to write instructions for crane
 * @param priority_to_load - a refernce to a vector that stores all containers
 *                           that are non-relevant to this port and has been unloaded
 */
void Lifo_algorithm::unloadContainers(std::ofstream& output, std::vector<Container>& priority_to_load){
    std::vector<Container> unload;
    ship->get_containers_to_unload(port, unload); //containersByPort

    for (auto it_con = unload.begin(); it_con != unload.end();) {
        if(*(it_con->get_dest()) == *port){
            //add to port to arrived containers
            port->add_container(*it_con, "A");
        }
        else {
            //add to priority vec
            priority_to_load.emplace_back(*it_con);
        }
        std::tuple<int,int,int> position = ship->get_coordinate(*it_con);
        std::vector<Container> column = ship->get_column_at(position); //shipMap

        //write to file
        Port::write_instruction_to_file(output, "U", (it_con)->get_id(), position);

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
}

/**
 * This function loads port's containers to ship by this scheme:
 * for every port in route (in reverse order) load all containers
 * to lowest free spot in the ship.
 * @param output - output file to write instructions for crane
 */
void Lifo_algorithm::loadContainers(std::ofstream& output){
    std::vector<Container> load;
    port->get_containers_to_load(load);
    std::vector<Port*> route = ship->get_route();

    for (auto & route_it : route) {
        for (auto load_it = load.end() - 1; !load.empty() && load_it != load.begin();) {
            if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
                port->load_to_ship(*load_it, ship);
                Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
                load.erase(load_it);
            }
            --load_it;
            if(load_it == load.begin()){
                if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
                    port->load_to_ship(*load_it, ship);
                    Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
                    load.erase(load_it);
                }
            }
        }
    }
}

const std::string Lifo_algorithm::getTypeName() const {
    return this->name;
}

