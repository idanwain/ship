#include "erroneous_algorithm.h"
#include "port.h"

/**
 * this function manages the load / unload of containers and logs it into a file.
 * @param output - output file to write instructions for crane
 */
void  Erroneous_algorithm::getInstructionsForCrane(std::ofstream &output) {
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
void Erroneous_algorithm::unloadContainers(std::ofstream& output){
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
            if(*(con_iterator->getDest()) == *port){
                //calculator approved unload --> unload, record & move to next container
                if(ship->getCalc()->tryOperation('U', con_iterator->getWeight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                    unloadSingleContainer(output, *con_iterator, 'A', coor);
                    --con_iterator;
                }
                //calculator didnt approved unload --> record & move to next column
                else{
                    Algorithm::writeToOutput(output, "R", con_iterator->getId(), ship->getCoordinate(*con_iterator), std::forward_as_tuple(-1, -1, -1));
                    break; //next column
                }
            }
            //container's destination != this port
            else {
                coordinate new_spot;
                //checks weight, space and efficiency.
                bool found = ship->findColumnToMoveTo(coor, new_spot, *containersToUnload, con_iterator->getWeight());
                //didn't found a proper coordinate to move the container to
                if(!found){
                    //if cant move, maybe can at least unload it, calculator approved unload --> unload, record & move to next container
                    if(ship->getCalc()->tryOperation('U', con_iterator->getWeight(), std::get<0>(coor), std::get<1>(coor)) == APPROVED){
                        unloadSingleContainer(output, *con_iterator, 'P', coor);
                        --con_iterator;
                    }
                    else{
                        //calculator didnt approved unload --> record & move to next column
                        Algorithm::writeToOutput(output, "R", con_iterator->getId(), ship->getCoordinate(*con_iterator), std::forward_as_tuple(-1, -1, -1));
                        break; //next column
                    }
                }
                //found a proper coordinate to move the container to
                else {
                    std::tuple<int,int,int> old_coor = ship->getCoordinate(*con_iterator);
                    std::string id = con_iterator->getId();
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
 * This function rejects every given container.
 * @param output - output file to write instructions for crane
 */
void Erroneous_algorithm::loadContainers(char list_category, std::ofstream& output){
    //get proper container's vector
    std::vector<Container>* load = port->getContainerVec(list_category);
    if(load == nullptr) return;

    // sort by reverse order of ports in route
    Algorithm::initContainersDistance(*load);
    std::sort(load->begin(), load->end());

    //cut vector by free space on ship
    while(ship->getFreeSpace() < (int)load->size()){
        Algorithm::writeToOutput(output, "R", load->back().getId(), std::forward_as_tuple(-1, -1, -1), std::forward_as_tuple(-1, -1, -1));
        load->pop_back();
    }

    /*This is making only rejects instructions*/
    for(auto con = load->end() - 1; !load->empty() && con >= load->begin();--con){
        Algorithm::writeToOutput(output, "R", con->getId(), std::forward_as_tuple(-1, -1, -1), std::forward_as_tuple(-1, -1, -1));
        load->erase(con);
    }
}

const std::string Erroneous_algorithm::getTypeName() const {
    return this->name;
}

void Erroneous_algorithm::unloadSingleContainer(std::ofstream &output,Container& con, char vecType, coordinate coor){
    port->addContainer(con, vecType);
    Algorithm::writeToOutput(output, "U", con.getId(), ship->getCoordinate(con), std::forward_as_tuple(-1, -1, -1));
    ship->removeContainer(coor);
    Algorithm::increaseInstructionCounter();
}

