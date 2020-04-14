#include "Unsorted_Lifo_Algorithm.h"

/**
 * this function manages the load / unload of containers and logs it into a file.
 * @param output - output file to write instructions for crane
 */
void  Unsorted_Lifo_Algorithm::getInstructionsForCrane(std::ofstream &output) {
    //unload containers from ship to port
    unloadContainers(output);

    //load containers from port to ship
    loadContainers('P',output);
    loadContainers('L',output);
}


/**
 * This function unloads all the containers that need to be unloaded to port by these scheme:
 *      - get all containers to unload
 *      - find the columns of those containers inside the ship
 *      - unload containers that are in those columns to port
 *      - stop unload when hits the port's container
 *
 * @param output - output file to write instructions for crane
 */
void Unsorted_Lifo_Algorithm::unloadContainers(std::ofstream& output){
    std::vector<Container>* containersToUnload = nullptr;
    ship->getContainersToUnload(port, &containersToUnload);
    std::set<coordinate> coordinates_to_handle;
    ship->getCoordinatesToHandle(coordinates_to_handle, *containersToUnload);

    for(coordinate coor : coordinates_to_handle){
        int lowest_floor = ship->getLowestFloorOfRelevantContainer(port, coor);
        std::vector<Container>* column = nullptr;
        ship->getColumn(coor, &column);

        for(auto con_iterator = column->end() - 1; !column->empty() && con_iterator >= column->begin();){
            if(con_iterator - column->begin() == lowest_floor - 1) break;
            //container's destination == this port
            if(*(con_iterator->get_dest()) == *port){
                //calculator approved unload --> unload, record & move to next container
                if(ship->getCalc()->tryOperation('U', con_iterator->get_weight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                    unloadSingleContainer(output, *con_iterator, 'A', coor);
//                    column->pop_back(); // might destroy container -->check in port if exist
                    --con_iterator;
                }
                    //calculator didnt approved unload --> record & move to next column
                else{
                    Algorithm::writeToOutput(output,"R", con_iterator->get_id(), ship->getCoordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                    break; //next column
                }
            }
                //container's destination != this port
            else {
                coordinate new_spot;
                //checks weight, space and efficiency.
                bool found = ship->findColumnToMoveTo(coor, new_spot, *containersToUnload, con_iterator->get_weight());
                //didn't found a proper coordinate to move the container to
                if(!found){
                    //if cant move, maybe can at least unload it, calculator approved unload --> unload, record & move to next container
                    if(ship->getCalc()->tryOperation('U', con_iterator->get_weight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                        unloadSingleContainer(output, *con_iterator, 'P', coor);
//                        column->pop_back();
                        --con_iterator;
                    }
                    else{
                        //calculator didnt approved unload --> record & move to next column
                        Algorithm::writeToOutput(output,"R", con_iterator->get_id(), ship->getCoordinate(*con_iterator), std::forward_as_tuple(-1,-1,-1));
                        break; //next column
                    }
                }
                    //found a proper coordinate to move the container to
                else {
                    std::tuple<int,int,int> old_coor = ship->getCoordinate(*con_iterator);
                    std::string id = con_iterator->get_id();
                    ship->moveContainer(coor, new_spot);
                    std::tuple<int,int,int> new_coor(std::get<0>(new_spot),std::get<1>(new_spot),ship->getTopFloor(new_spot) - 1);
                    Algorithm::writeToOutput(output,"M", id, old_coor, new_coor);
                    Algorithm::increaseInstructionCounter();
                    --con_iterator;
                }
            }
        }
    }
}

/**
 * This function loads port's containers to ship by this scheme:
 * for every port in route (in reverse order) load all containers
 * to lowest free spot in the ship.
 * @param output - output file to write instructions for crane
 */
void Unsorted_Lifo_Algorithm::loadContainers(char list_category, std::ofstream& output){
    //get proper container's vector
    std::vector<Container>* load = nullptr;
    port->getContainersToLoad(&load, list_category);

    //validate by: data, port is'nt in route, space, weight
    for(auto con = load->end() - 1; !load->empty() && con >= load->begin();){
        bool found = false;
        coordinate coor;
        int weight = con->get_weight();
        ship->findColumnToLoad(coor, found, weight);
        if(validateId(con->get_id()) && isPortInRoute(con->get_dest(), ship->getRoute(), getPortNum()) && found){
            ship->addContainer(*con, coor);
            Algorithm::writeToOutput(output,"L", con->get_id(), ship->getCoordinate(*con), std::forward_as_tuple(-1,-1,-1));
            load->erase(con);
            Algorithm::increaseInstructionCounter();
        } else {
            Algorithm::writeToOutput(output,"R", con->get_id(), std::forward_as_tuple(-1,-1,-1), std::forward_as_tuple(-1,-1,-1));
        }
        --con;
    }
}

const std::string Unsorted_Lifo_Algorithm::getTypeName() const {
    return this->name;
}

void Unsorted_Lifo_Algorithm::unloadSingleContainer(std::ofstream &output,Container& con, char vecType, coordinate coor){
    port->addContainer(con, vecType);
    Algorithm::writeToOutput(output,"U", con.get_id(), ship->getCoordinate(con), std::forward_as_tuple(-1,-1,-1));
//    ship->removeFromContainersByPort(con, port);
    ship->removeContainer(coor);
    Algorithm::increaseInstructionCounter();
}