#ifndef SHIP_HEADER
#define SHIP_HEADER

#include <iostream>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
#include <map>
#include "port.h"
#include "container.h"
#include <stack>

#define ON_BOARD 1
#define WIDTH
#define HEIGHT
#define DEPTH

const char delim[] = {',','\t','\r',' '};
class Container; class Port;

class Ship {
    std::vector<std::vector<std::vector<Container>>> shipMap;
    std::map<Port*, std::vector<Container>> containersByPort;
    std::vector<Port*> route;
    int freeSpace;
    int x;
    int y;
    int z;
public:
    /*given a route of ports, the C'tor parses the containers of any port to a map*/
    Ship(int x, int y, int z) {
        std::vector<std::vector<std::vector<Container>>> map;
        map.resize(x);
        for(int i = 0; i < x; i++){
            map[i].resize(y);
            for(int j = 0; j < y; j++){
                map[i][j].reserve(z);
            }
        }
        this->shipMap = map;
        this->x = x;
        this->y = y;
        this->z = z;
        freeSpace = x*y*z;
    }
    /*Copy c'tor*/
    Ship(const Ship* shipToCopy){
        x = shipToCopy->getAxis("x");
        y = shipToCopy->getAxis("y");
        z = shipToCopy->getAxis("z");
        freeSpace = x*y*z;
        this->shipMap.resize(x);
        for(int i = 0; i < x; i++){
            this->shipMap[i].resize(y);
            for(int j = 0; j < y; j++){
                //Note that this c'tor only for copying the blocks
                this->shipMap[i][j] = shipToCopy->shipMap[i][j];
            }
        }
        bool found = false;
        for(Port* p_out : shipToCopy->route){
            for(Port* p_this : this->route){
                if(p_out->get_name() == p_this->get_name()){
                    this->route.emplace_back(p_this);
                    found = true;
                    break;
                }
            }
            if(!found){//Case we didn't found a port that already exist...need to insert this one.
                this->route.emplace_back(new Port(p_out->get_name()));
            }
            found = false;
        }
    }
    ~Ship();
    std::tuple<int, int, int> get_coordinate(const Container& container);
    std::vector<Container>& get_column_at(std::tuple<int, int, int> position);
    std::vector<Port*> get_route();
    std::vector<std::vector<std::vector<Container>>>* get_map();
    bool has_space(); //TODO check space in ship, maybe need to maintain number of free spots for containers on ship
    bool has_weight_prob(); //TODO change name of function according to instructions and implement weight problems check
    void add_container(Container& container, std::tuple<int,int> coordinate); //TODO add container on board
    Port* getPortByName(const std::string &name);
    void setRoute(std::vector<Port*> &route);
    int getAxis(const std::string& str) const;
    void get_containers_to_unload(Port* port, std::vector<Container>& unload);
    void add_container_to_map(Container &container);
    void initContainersByPort(std::vector<Port *> &vector);
    void update_free_space(int num);
    std::tuple<int, int> find_min_floor();
};

#endif // !SHIP_HEADER