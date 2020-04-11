#ifndef SHIP1_ERROR_H
#define SHIP1_ERROR_H

#include "container.h"


class Error {
    Container* pCon = nullptr;
    Port* pPort = nullptr;
public:
    Error(Container* pCon1, Port* pPort1): pCon(pCon1), pPort(pPort1){}
    friend std::ostream& operator<<(std::ostream& os, const Error& c);
    //TODO iterate over ports & containersByPort --> extract data for errors in the end of algorithm run
};


#endif //SHIP1_ERROR_H
