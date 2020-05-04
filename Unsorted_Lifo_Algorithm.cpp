#include "Unsorted_Lifo_Algorithm.h"
#include "Port.h"

/**
 * This function unloads all the containers that need to be unloaded to port by these scheme:
 *      - get all containers to unload
 *      - find the columns of those containers inside the ship
 *      - unload containers that are in those columns to port
 *      - stop unload when hits the port's container
 * @param output - output file to write instructions for crane
 */
void Unsorted_Lifo_algorithm::unloadContainers(std::ofstream& output){
    std::vector<Container>* containersToUnload = nullptr;
    pShip->getContainersToUnload(pPort, &containersToUnload);
    std::set<coordinate> coordinates_to_handle;
    pShip->getCoordinatesToHandle(coordinates_to_handle, *containersToUnload);

    for(coordinate coor : coordinates_to_handle){
        int lowest_floor = pShip->getLowestFloorOfRelevantContainer(pPort, coor);
        std::vector<Container>* column = nullptr;
        pShip->getColumn(coor, &column);
        handleColumn(coor, column, lowest_floor, containersToUnload, output);
    }
}

void Unsorted_Lifo_algorithm::handleColumn(coordinate coor, std::vector<Container>* column, int lowest_floor,
                                  std::vector<Container>* containersToUnload, std::ofstream& output){
    int X = std::get<0>(coor); int Y =  std::get<1>(coor);
    for(auto con_iterator = column->end() - 1; !column->empty() && con_iterator >= column->begin();){
        if(con_iterator - column->begin() == lowest_floor - 1) break;
        //container's destination == this port
        if(*(con_iterator->getDest()) == *pPort){
            //calculator approved unload --> unload, record & move to next container
            if(calc.tryOperation('U', con_iterator->getWeight(), X, Y) == APPROVED){
                unloadSingleContainer(output, *con_iterator, Type::ARRIVED, coor);
                --con_iterator;
            }
                //calculator didnt approved unload --> record & move to next column
            else{
                writeToOutput(output, Action::REJECT, con_iterator->getId(), pShip->getCoordinate(*con_iterator));
                break; //next column
            }
        }
            //container's destination != this port
        else {
            coordinate new_spot;
            //checks weight, space and efficiency.
            bool found = pShip->findColumnToMoveTo(coor, new_spot, *containersToUnload, con_iterator->getWeight(), calc);
            //didn't found a proper coordinate to move the container to
            if(!found){
                //if cant move, maybe can at least unload it, calculator approved unload --> unload, record & move to next container
                if(calc.tryOperation('U', con_iterator->getWeight(), X, Y) == APPROVED){
                    unloadSingleContainer(output, *con_iterator, Type::PRIORITY, coor);
                    --con_iterator;
                }
                    //calculator didnt approved unload --> record & move to next column
                else {
                    writeToOutput(output, Action::REJECT, con_iterator->getId(), pShip->getCoordinate(*con_iterator));
                    break; //next column
                }
            }
                //found a proper coordinate to move the container to
            else {
                std::tuple<int,int,int> old_coor = pShip->getCoordinate(*con_iterator);
                std::string id = con_iterator->getId();
                pShip->moveContainer(coor, new_spot);
                std::tuple<int,int,int> new_coor(std::get<0>(new_spot),std::get<1>(new_spot),pShip->getTopFloor(new_spot) - 1);
                writeToOutput(output,Action::MOVE, id, old_coor, new_coor);
                --con_iterator;
            }
        }
    }
}

/**
 * This function loads port's containers to ship by this scheme:
 *      -sort containers by distance from destination.
 *      -cut containers from load list according to free space in ship.
 *      -load containers in reverse order: far destination == lower spot on ship.
 * @param output - output file to write instructions for crane
 */
void Unsorted_Lifo_algorithm::loadContainers(Type list_category, std::ofstream& output){
    //get proper container's vector
    std::vector<Container>* load = pPort->getContainerVec(list_category);
    if(load == nullptr) return;
    //validate by: data, port is'nt in route, space, weight
    for(auto con = load->end() - 1; !load->empty() && con >= load->begin();--con){
        bool found = false;
        coordinate coor;
        int weight = con->getWeight();
        pShip->findColumnToLoad(coor, found, weight, calc);

        bool validID = validateId(con->getId());
        auto route = pShip->getRoute();
        auto destName = con->getDest()->get_name();
        auto currPortNum = getPortNum();
        bool isInRoute = isPortInRoute(destName, route, currPortNum);
        bool uniqueIdOnShip = !pShip->isOnShip(*con);

        if(uniqueIdOnShip && validID && isInRoute && found){
            pShip->addContainer(*con, coor);
            writeToOutput(output, Action::LOAD, con->getId(), pShip->getCoordinate(*con));
        } else {
            writeToOutput(output, Action::REJECT, con->getId());
        }
        load->erase(con);
    }
}

int Unsorted_Lifo_algorithm::getPortNum() {
    return portNum;
}

void Unsorted_Lifo_algorithm::unloadSingleContainer(std::ofstream &output,Container& con, Type vecType, coordinate coor){
    pPort->addContainer(con, vecType);
    writeToOutput(output, Action::UNLOAD, con.getId(), pShip->getCoordinate(con));
    pShip->removeContainer(coor);
}

int Unsorted_Lifo_algorithm::readShipPlan(const std::string& full_path_and_file_name){
    return extractShipPlan(full_path_and_file_name, this->pShip) ||
           extractArgsForBlocks(this->pShip, full_path_and_file_name);
}

int Unsorted_Lifo_algorithm::readShipRoute(const std::string& full_path_and_file_name) {
    return extractTravelRoute(pShip, full_path_and_file_name);
}

int Unsorted_Lifo_algorithm::setWeightBalanceCalculator(WeightBalanceCalculator& calculator) {
    calc = calculator;
    return 0;
}

int Unsorted_Lifo_algorithm::getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                            const std::string& output_full_path_and_file_name) {
    this->pPort = pShip->getRoute().at(portNum);
    std::ofstream output(output_full_path_and_file_name);
    //parse the input data into
    if(!parseDataToPort(input_full_path_and_file_name, output, pShip, pPort)){
        std::cout << "CONTAINER_FILE_ERROR" << std::endl;
        return false;
    };

    //unload containers from ship to port
    unloadContainers(output);
    //load containers from port to ship
    loadContainers(Type::PRIORITY,output);
    loadContainers(Type::LOAD,output);

    output.close();
    ++portNum;
    return 0;
}
