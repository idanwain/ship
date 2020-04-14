#include "common.h"

void execute(Ship* ship, char command, Container* container, coordinate origin, coordinate dest){
    switch(command){
        case 'L': ship->addSimulationContainer(*container,origin); break;
        case 'U': ship->removeContainer(origin);  break;
        case 'M': ship->moveContainer(origin, dest);  break;
        default: std::cout << "Invalid command, please insert L/U/M commands." << std::endl;
    }
}
