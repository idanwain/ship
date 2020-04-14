#include "ship.h"

/**
 * X - x coordinate ,  Y - y coordinate. ,  Z - floor number
 * given a container that is certainly on the ship, returns a tuple of the container's position at the ship
 */
std::tuple<int, int, int> Ship::getCoordinate(const Container& container) {
    int x = -1; int y = -1; int z = -1;
    for(auto & it_x : shipMap){
        ++x;
        for(auto & it_y : it_x){
            ++y;
            for(auto & it_z : it_y){
                ++z;
                if(it_z == container){
                    return std::tuple<int,int,int>(x,y,z);
                }
            }
            z = -1;
        }
        y = -1;
    }
	return std::tuple<int, int, int>();
}

std::vector<Port*> Ship::getRoute() {
	return this->route;
}

void Ship::addContainer(Container& container, std::tuple<int,int> coordinate) {
    this->addContainerToMap(container);
    shipMap[std::get<0>(coordinate)][std::get<1>(coordinate)].emplace_back(container);
    freeSpace--;
}

std::vector<std::vector<std::vector<Container>>>* Ship::getMap() {
    return &shipMap;
}

Port* Ship::getPortByName(const std::string &name) {
    Port* pPort = nullptr;
    for(auto port : this->route) {
        if(port->get_name() == name) {
            pPort = port;
        }
    }
    if(pPort == nullptr){
        pPort = new Port("NOT_IN_ROUTE");
    }
    return pPort;
}

void Ship::setRoute(std::vector<Port *> &route) {
    this->route = route;
    //init map of containers by port according to routes
    this->initContainersByPort(route);
}

int Ship::getAxis(const std::string &str) const {
    if(str == "x") return this->x;
    else if(str == "y") return this->y;
    else if(str == "z") return this->z;
    else return -1;

}

void Ship::getContainersToUnload(Port *pPort, std::vector<Container>** unload) {
    *unload =  &containersByPort[pPort];
}

void Ship::addContainerToMap(Container &container) {
    if(this->containersByPort.find(container.get_dest()) == containersByPort.end()){
        this->containersByPort.insert({container.get_dest(), std::vector<Container>{container}});
    } else {
        this->containersByPort[container.get_dest()].emplace_back(container);
    }
}

void Ship::initContainersByPort(std::vector<Port *> &vector) {
    for(auto pPort : vector){
        std::vector<Container> con;
        this->containersByPort.insert({pPort, con});
    }
}

void Ship::updateFreeSpace(int num){
    this->freeSpace += num;
}

Ship::~Ship() {
    for(std::vector<std::vector<Container>> &mat : this->shipMap) {
        for (std::vector<Container> &vec : mat) {
            vec.clear();
        }
        mat.clear();
    }

    for(std::map<Port *, std::vector<Container >>::iterator iter = containersByPort.begin();
         iter != containersByPort.end(); ++iter) {
        Port *port = iter->first;
        delete port;
    }
    std::cout << "in ship's d'tor" << std::endl;
}

void Ship::getCoordinatesToHandle(std::set<coordinate> &coordinates_to_handle, std::vector<Container>& containers_to_unload) {
    for(Container& con : containers_to_unload){
        coordinates_to_handle.emplace(coordinate(std::get<0>(this->getCoordinate(con)),std::get<1>(this->getCoordinate(con))));
    }
}

int Ship::getLowestFloorOfRelevantContainer(Port *pPort, coordinate coor){
    int lowest = 0;
    for(Container& con : this->shipMap[std::get<0>(coor)][std::get<1>(coor)]){
        if(*(con.get_dest()) == (*pPort)){
            break;
        }
        ++lowest;
    }
    return lowest;
}

void Ship::getColumn(coordinate coor, std::vector<Container>** column) {
    *column = &(shipMap[std::get<0>(coor)][std::get<1>(coor)]);
}

void Ship::removeFromContainersByPort(Container &container, Port *pPort) {
    containersByPort[pPort].erase(std::find(containersByPort[pPort].begin(), containersByPort[pPort].end(), container));
}

WeightBalanceCalculator* Ship::getCalc() {
    return calculator;
}

bool Ship::findColumnToMoveTo(coordinate old_coor, coordinate& new_coor, std::vector<Container>& containersToUnload, int weight) {
    std::set<coordinate> coordinates_to_check;
    this->getCoordinatesToHandle(coordinates_to_check, containersToUnload);
    int x = 0; int y = 0;
    for(auto coor_x : shipMap){
        for(auto coor_y : coor_x){
            int size = this->shipMap[x][y].size();
            int capacity = this->shipMap[x][y].capacity();
            if(!std::count(coordinates_to_check.begin(), coordinates_to_check.end(), std::make_tuple(x,y)) &&
                size < capacity &&
                this->getCalc()->tryOperation('U', weight, std::get<0>(old_coor), std::get<1>(old_coor)) == APPROVED &&
                this->getCalc()->tryOperation('L', weight, x, y) == APPROVED){
                    new_coor = std::make_tuple(x,y);
                    return true;
            }
            ++y;
        }
        ++x;
        y=0;
    }
}

void Ship::moveContainer(coordinate origin, coordinate dest) {
    shipMap[std::get<0>(dest)][std::get<1>(dest)].emplace_back(shipMap[std::get<0>(origin)][std::get<1>(origin)].back());
    shipMap[std::get<0>(origin)][std::get<1>(origin)].pop_back();
}

void Ship::initCalc() {
    calculator = new WeightBalanceCalculator(this);
}

int Ship::getTopFloor(coordinate coor) {
    return shipMap[std::get<0>(coor)][std::get<1>(coor)].size();
}

void Ship::findColumnToLoad(coordinate &coor, bool &found, int kg) {
    int x = 0; int y = 0;
    for(auto& coor_x : shipMap){
        for(auto& coor_y : coor_x){
            int size = shipMap[x][y].size(); int capacity = shipMap[x][y].capacity();
            if(size < capacity &&
                    this->getCalc()->tryOperation('L', kg, x, y) == APPROVED && !found){
                coor = std::make_tuple(x,y);
                found = true;
                break;
            }
            ++y;
        }
        if(found){
            break;
        }
        ++x;
        y = 0;
    }
}

void Ship::capacitySizeCheck(){
    int x = 0; int y = 0;
    for(auto coor_x : shipMap){
        for(auto coor_y : coor_x){
            std::cout << "(" << x << "," << y << "): Size: " << shipMap[x][y].size() << ", Capacity: " << shipMap[x][y].capacity() << std::endl;
            ++y;
        }
        ++x;
        y = 0;
    }
}

std::map<Port*,std::vector<Container>>& Ship::getContainersByPort() {
    return this->containersByPort;
}

void Ship::removeContainer(coordinate coor) {
    shipMap[std::get<0>(coor)][std::get<1>(coor)].pop_back();
}

void Ship::addSimulationContainer(Container &container, coordinate coor) {
    shipMap[std::get<0>(coor)][std::get<1>(coor)].emplace_back(container);
}


//std::tuple<int,int> Ship::find_min_floor(){
//    std::tuple<int, int> min_floor_coor;
//    size_t min = INT_MAX;
//    int x=-1; int y=-1;
//    for(auto& it_x : this->shipMap){
//        ++x;
//        for(auto& it_y : it_x){
//            ++y;
//            if(it_y.empty()){} //UNUSED it_y
//            if(shipMap[x][y].size() < min){
//                min = shipMap[x][y].size();
//                min_floor_coor = std::make_tuple(x,y);
//            }
//        }
//        y = -1;
//    }
//    return min_floor_coor;
//}
