#include "lifo_algorithm.h"
#include "port.h"

/**
 * this function manages the load / unload of containers and logs it into a file.
 * @param output - output file to write instructions for crane
 */
void  Lifo_algorithm::get_instructions_for_crane(std::ofstream& output) {
    //unload containers from ship to port
//    std::vector<Container> priority_to_load;
//    unloadContainers(output, priority_to_load);
//
//    //load unrelated unloaded containers back to ship
//    for(auto& con : priority_to_load){
//        ship->add_container(con,ship->find_min_floor());
//        Port::write_instruction_to_file(output, "L", con.get_id(), ship->get_coordinate(con));
//    }
    unload_containers(output);

    //load containers from port to ship
    load_containers('P',output);
    load_containers('L',output);
}

void Lifo_algorithm::unload_containers(std::ofstream& output){
    std::vector<Container>* containersToUnload = nullptr;
    ship->get_containers_to_unload(port, &containersToUnload);
    std::set<coordinate> coordinates_to_handle;
    ship->get_coordinates_to_handle(coordinates_to_handle, *containersToUnload);

    for(coordinate coor : coordinates_to_handle){
        int lowest_floor = ship->get_lowest_floor_of_relevant_container(port, coor);
        std::vector<Container>* column = nullptr;
        ship->get_column(coor, &column);
        for(auto con_iterator = column->end() - 1; !column->empty() && con_iterator >= column->begin();){
            if(con_iterator - column->begin() == lowest_floor - 1){
                break;
            }
            if(*(con_iterator->get_dest()) == *port){
                if(ship->getCalc()->tryOperation('U', con_iterator->get_weight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                    port->add_container(*con_iterator, "A");
                    Algorithm::write_to_output(output,"U", con_iterator->get_id(), ship->get_coordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                    ship->remove_from_containers_by_port(*con_iterator, port);
                    column->pop_back(); // might destroy container -->check in port if exist
                    Algorithm::increase_instruction_counter();
                    --con_iterator;
                }
                else{
                    //calculator didnt approved unload --> do something
                    Algorithm::write_to_output(output,"R", con_iterator->get_id(), ship->get_coordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                    break; //next column
                }
            }
            else {
                bool found = false;
                coordinate new_spot;
                ship->find_column_to_move_to(coor, new_spot, found, *containersToUnload, con_iterator->get_weight()); //checks weight, space and efficiency.
                if(!found){
                    if(ship->getCalc()->tryOperation('U', con_iterator->get_weight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                        port->add_container(*con_iterator, "P");
                        Algorithm::write_to_output(output,"U", con_iterator->get_id(), ship->get_coordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                        ship->remove_from_containers_by_port(*con_iterator, port);
                        column->pop_back(); // might destroy container -->check in port if exist
                        Algorithm::increase_instruction_counter();
                        --con_iterator;
                    }
                    else{
                        //calculator didnt approved unload --> do something
                        Algorithm::write_to_output(output,"R", con_iterator->get_id(), ship->get_coordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                        break; //next column
                    }
                }
                else {
                    std::tuple<int,int,int> old_coor = ship->get_coordinate(*con_iterator);
                    std::string id = con_iterator->get_id();
                    ship->move_container(coor, new_spot);
                    std::tuple<int,int,int> new_coor(std::get<0>(new_spot),std::get<1>(new_spot),ship->getTopFloor(new_spot));
                    Algorithm::write_to_output(output,"M", id, old_coor, new_coor);
                    Algorithm::increase_instruction_counter();
                    --con_iterator;
                }
            }
        }
    }
}

void Lifo_algorithm::load_containers(char list_category, std::ofstream& output){
    //get proper container's vector
    std::vector<Container>* load = nullptr;
    port->get_containers_to_load(&load, list_category);
    // sort by reverse order of ports in route
    Algorithm::initContainersDistance(*load);
    std::sort(load->begin(), load->end());
    //validate by:
    //data, port is'nt in route, space, weight
    for(auto con = load->end() - 1; !load->empty() && con >= load->begin();){
        bool found = false;
        coordinate coor;
        int weight = con->get_weight();
        ship->find_column_to_load(coor, found, weight);
        if(validate_id(con->get_id()) && isPortInRoute(con->get_dest()) && found){
            ship->add_container(*con, coor);
            Algorithm::write_to_output(output,"L", con->get_id(), ship->get_coordinate(*con), std::forward_as_tuple(-1,-1,-1));
            //load->pop_back();
            load->erase(con);
            Algorithm::increase_instruction_counter();
        } else {
            Algorithm::write_to_output(output,"R", con->get_id(), std::forward_as_tuple(-1,-1,-1), std::forward_as_tuple(-1,-1,-1));
        }
        --con;
    }
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
//void Lifo_algorithm::unloadContainers(std::ofstream& output, std::vector<Container>& priority_to_load){
////    std::vector<Container> unload;
////    ship->get_containers_to_unload(port, unload); //containersByPort
////
////    for (auto it_con = unload.begin(); it_con != unload.end();) {
////        if(*(it_con->get_dest()) == *port){
////            //add to port to arrived containers
////            port->add_container(*it_con, "A");
////        }
////        else {
////            //add to priority vec
////            priority_to_load.emplace_back(*it_con);
////        }
////        std::tuple<int,int,int> position = ship->get_coordinate(*it_con);
////        std::vector<Container> column = ship->get_column_at(position); //shipMap
////
////        //write to file
////        Port::write_instruction_to_file(output, "U", (it_con)->get_id(), position);
////
////        //delete from shipMap
////        for(auto container = column.end() - 1; !column.empty() && *container != *it_con;) {
////            column.erase(container);
////            container--;
////        }
////        if(!column.empty()){
////            column.pop_back();
////        }
////        //delete from containersByPort
////        if(!unload.empty()){
////            it_con = unload.erase(it_con);
////        }
////    }
////}

/**
 * This function loads port's containers to ship by this scheme:
 * for every port in route (in reverse order) load all containers
 * to lowest free spot in the ship.
 * @param output - output file to write instructions for crane
 */
//void Lifo_algorithm::loadContainers(std::ofstream& output){
//    std::vector<Container> load;
//    port->get_containers_to_load(load);
//    std::vector<Port*> route = ship->get_route();
//
//    for (auto & route_it : route) {
//        for (auto load_it = load.end() - 1; !load.empty() && load_it != load.begin();) {
//            if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
//                port->load_to_ship(*load_it, ship);
//                Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
//                load.erase(load_it);
//            }
//            --load_it;
//            if(load_it == load.begin()){
//                if (load_it->get_dest() == route_it && ship->has_space() && !(ship->has_weight_prob())) {
//                    port->load_to_ship(*load_it, ship);
//                    Port::write_instruction_to_file(output, "L", (*load_it).get_id(), ship->get_coordinate(*load_it));
//                    load.erase(load_it);
//                }
//            }
//        }
//    }
//}

const std::string Lifo_algorithm::getTypeName() const {
    return this->name;
}



