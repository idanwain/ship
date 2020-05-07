#include "LifoAlgorithm.h"
#include "../Common/Port.h"

REGISTER_ALGORITHM (LifoAlgorithm)

/**
 * This function unloads all the containers that need to be unloaded to port by these scheme:
 *      - get all containers to unload
 *      - find the columns of those containers inside the ship
 *      - unload containers that are in those columns to port
 *      - stop unload when hits the port's container
 * @param output - output file to write instructions for crane
 */
void LifoAlgorithm::unloadContainers(std::ofstream& output){
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

void LifoAlgorithm::handleColumn(coordinate coor, std::vector<Container>* column, int lowest_floor,
        std::vector<Container>* containersToUnload, std::ofstream& output){
    int X = std::get<0>(coor); int Y =  std::get<1>(coor);
    for(auto con_iterator = column->end() - 1; !column->empty() && con_iterator >= column->begin();){
        if(con_iterator - column->begin() == lowest_floor - 1) break;
        if(*(con_iterator->getDest()) == *pPort){
            if(calc.tryOperation('U', con_iterator->getWeight(), X, Y) == APPROVED){
                unloadSingleContainer(output, *con_iterator, Type::ARRIVED, coor);
                --con_iterator;
            }
            else{
                writeToOutput(output, Action::REJECT, con_iterator->getId(), pShip->getCoordinate(*con_iterator));
                break;
            }
        }
        else {
            coordinate new_spot;
            bool found = pShip->findColumnToMoveTo(coor, new_spot, *containersToUnload, con_iterator->getWeight(), calc);
            if(!found){
                if(calc.tryOperation('U', con_iterator->getWeight(), X, Y) == APPROVED){
                    unloadSingleContainer(output, *con_iterator, Type::PRIORITY, coor);
                    --con_iterator;
                }
                else {
                    writeToOutput(output, Action::REJECT, con_iterator->getId(), pShip->getCoordinate(*con_iterator));
                    break;
                }
            }
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
void LifoAlgorithm::loadContainers(Type list_category, std::ofstream& output){
    std::vector<Container>* load = pPort->getContainerVec(list_category);
    if(load == nullptr) return;
    initContainersDistance(*load);
    std::sort(load->begin(), load->end());
    /*for exercise 3*/
//    while(pShip->getFreeSpace() < (int)load->size()){
//        writeToOutput(output, "R", load->back().getId());
//        load->pop_back();
//    }
    for(auto con = load->end() - 1; !load->empty() && con >= load->begin();--con){
        bool found = false;
        coordinate coor;
        int weight = con->getWeight();
        pShip->findColumnToLoad(coor, found, weight, calc);

        bool validID = isValidId(con->getId());
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

int LifoAlgorithm::getPortNum() {
    return portNum;
}

void LifoAlgorithm::unloadSingleContainer(std::ofstream &output,Container& con, Type vecType, coordinate coor){
    pPort->addContainer(con, vecType);
    writeToOutput(output, Action::UNLOAD, con.getId(), pShip->getCoordinate(con));
    pShip->removeContainer(coor);
}

void LifoAlgorithm::initContainersDistance(std::vector<Container> &vector) {
    auto route = this->pShip->getRoute();
    for(auto port = route.rbegin(); port != route.rend(); ++port){
        int distance = std::distance(port, route.rend() - portNum);
        if(distance <= 0){
            break;
        }
        for(auto& con : vector){
            if((*(*port)) == *(con.getDest())){
                con.setDistance(distance);
            }
        }
    }
}

int LifoAlgorithm::readShipPlan(const std::string& full_path_and_file_name){
    return extractShipPlan(full_path_and_file_name, this->pShip) ||
        extractArgsForBlocks(this->pShip, full_path_and_file_name);
}

int LifoAlgorithm::readShipRoute(const std::string& full_path_and_file_name) {
    return extractTravelRoute(pShip, full_path_and_file_name);
}

int LifoAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator& calculator) {
    calc = calculator;
    return 0;
}

int LifoAlgorithm::getInstructionsForCargo(const std::string& input_full_path_and_file_name,
                                            const std::string& output_full_path_and_file_name) {
    if(portNum > pShip->getRoute().size()) portNum = 0;
    this->pPort = pShip->getRoute().at(portNum);
    std::ofstream output(output_full_path_and_file_name);
    if(!parseDataToPort(input_full_path_and_file_name, output, pShip, pPort)){
        std::cout << "CONTAINER_FILE_ERROR" << std::endl;
        output.close();
        return -1;
    };
    unloadContainers(output);
    loadContainers(Type::PRIORITY,output);
    loadContainers(Type::LOAD,output);
    output.close();
    ++portNum;
    return 0;
}