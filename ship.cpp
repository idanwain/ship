#include "ship.h"


/**
 * X - x coordinate ,  Y - y coordinate. ,  Z - floor number
 * given a container that is certainly on the ship, returns a tuple of the container's position at the ship
 */
std::tuple<int, int, int> Ship::get_coordinate(const Container& container) {
    int x = -1; int y = -1; int z = -1;
    for(auto & it_x : shipMap){
        ++x;
        for(auto & it_y : it_x){
            ++y;
            for(auto & it_z : it_y){
                ++z;
                if(it_z == container){
                    //might be a problem with local temp returned object
                    return std::tuple<int,int,int>(x,y,z);
                }
            }
            z = -1;
        }
        y = -1;
    }
	return std::tuple<int, int, int>();
}

/**
 * given a position, returns a vector of the column of the position
 */
std::vector<Container>& Ship::get_column_at(std::tuple<int, int, int> position) {
	return shipMap[std::get<0>(position)][std::get<1>(position)];
}

std::vector<Port*> Ship::get_route() {
	return this->route;
}

bool Ship::has_space() {
    return freeSpace > 0;
}

bool Ship::has_weight_prob() {
	return false;
}

bool is_full_column(const std::vector<Container>& col) {
    return col.size() < col.capacity();
}

void Ship::add_container(Container& container, std::tuple<int,int> coordinate) {
    this->add_container_to_map(container);
    shipMap[std::get<0>(coordinate)][std::get<1>(coordinate)].emplace_back(container);
    freeSpace--;
}

std::vector<std::vector<std::vector<Container>>>* Ship::get_map() {
    return &shipMap;
}

Port* Ship::getPortByName(const std::string &name, bool& is_in_route) {
    for(auto port : this->route) {
        if(port->get_name() == name) {
            return port;
        }
    }
    is_in_route = false;
    /*
     * TODO need to deal if no such port exist in the list--> might be if we load a cargo to drop at port but
     * the certain port doesn't exist in the route because we are not stopping at.
     */
    Port* dummy = new Port("DUMMY");
    return dummy;
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

void Ship::get_containers_to_unload(Port *pPort, std::vector<Container>& unload) {
    unload =  this->containersByPort[pPort];
}

void Ship::add_container_to_map(Container &container) {
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

std::tuple<int,int> Ship::find_min_floor(){
    std::tuple<int, int> min_floor_coor;
    int min = INT_MAX;
    int x=-1; int y=-1;
    for(auto& it_x : this->shipMap){
        ++x;
        for(auto& it_y : it_x){
            ++y;
            if(it_y.empty()){} //UNUSED it_y
            if(shipMap[x][y].size() < min){
                min = shipMap[x][y].size();
                min_floor_coor = std::make_tuple(x,y);
            }
        }
        y = -1;
    }
    return min_floor_coor;
}


//*
// * returns the highest free floor at (x,y) coordinate
// */
//int Ship::get_top_floor(int x, int y) {
//    return shipMap[x][y].size() ;
//}

//
//std::tuple<int, int> Ship::find_first_free_spot() {
//    int x=-1; int y=-1;
//    for(auto& it_x : shipMap){
//        ++x;
//        for(auto& it_y : it_x){
//            if(!it_y.empty()){} // UNUSED
//            ++y;
//            if(shipMap[x][y].size() >= shipMap[x][y].capacity()){
//                return std::tuple<int,int>(x,y);
//            }
//        }
//    }
//    return std::tuple<int,int>(x,y); //dummy return
//}
void Ship::update_free_space(int num){
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