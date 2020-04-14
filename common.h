#ifndef SHIP1_COMMON_H
#define SHIP1_COMMON_H

#include <regex>
#include "ship.h"
enum class VALIDATION {InvalidID, ExistID, InvalidPort, Valid};

void execute(Ship* ship, char command,Container* container, coordinate origin, coordinate dest);
bool validateContainerData(const std::string& line, VALIDATION& reason, std::string& id, Ship* ship);
std::vector<std::string> stringSplit(std::string s, const char* delimiter);
bool is_number(const std::string& s);
bool isValidPortName(const std::string& name);
bool validateId(const std::string& str);
bool idExistOnShip(const std::string& id, Ship* ship);
bool isPortInRoute(Port *pPort, const std::vector<Port*>& route, int portNum);


#endif //SHIP1_COMMON_H
