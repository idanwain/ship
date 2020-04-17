#ifndef SHIP_HEADER
#define SHIP_HEADER

class WeightBalanceCalculator; class Container; class Port;

#include <iostream>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
#include <map>
#include "port.h"
#include "container.h"
#include <stack>
#include <set>
#include <algorithm>
#include "WeightBalanceCalculator.h"

typedef std::tuple<int,int> coordinate;
const char delim[] = {',','\t','\r',' ', '\n'};

class Ship {
    std::vector<std::vector<std::vector<Container>>> shipMap;
    std::map<Port*, std::vector<Container>> containersByPort;
    std::vector<Port*> route;
    WeightBalanceCalculator* calculator = nullptr;
    int freeSpace;
    int x, y, z;
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
        calculator = shipToCopy->calculator;
        this->shipMap.resize(x);
        for(int i = 0; i < x; i++){
            this->shipMap[i].resize(y);
            for(int j = 0; j < y; j++){
                //Note that this c'tor only for copying the blocks
                this->shipMap[i][j] = shipToCopy->shipMap[i][j];
                this->shipMap[i][j].reserve(z);
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
    std::tuple<int, int, int> getCoordinate(const Container& container);
    std::vector<Port*> getRoute();
    std::vector<std::vector<std::vector<Container>>>* getMap();
    Port* getPortByName(const std::string &name);
    std::map<Port*,std::vector<Container>>& getContainersByPort();
    void initContainersByPort(std::vector<Port *> &vector);
    void setRoute(std::vector<Port*> &route);
    void initCalc();
    int getAxis(const std::string& str) const;
    void getCoordinatesToHandle(std::set<coordinate> &coordinates_to_handle, std::vector<Container>& containers_to_unload);
    int getLowestFloorOfRelevantContainer(Port *pPort, coordinate coor);
    int getTopFloor(coordinate coor);
    void getColumn(coordinate coor, std::vector<Container>** column);
    int getFreeSpace();
    void getContainersToUnload(Port* port, std::vector<Container>** unload);
    bool findColumnToMoveTo(coordinate old_coor, coordinate& new_coor, std::vector<Container>& containersToUnload, int weight);
    void findColumnToLoad(coordinate &coor, bool &found, int kg);
    void addContainer(Container& container, std::tuple<int,int> coordinate);
    void removeContainer(coordinate coor);
    void moveContainer(coordinate origin, coordinate dest);
    void updateFreeSpace(int num);
    WeightBalanceCalculator* getCalc();
};

#endif