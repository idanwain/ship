#ifndef SHIP1_ERROR_H
#define SHIP1_ERROR_H

#include "container.h"

enum Reason {weight, space};

class Error {
    Container* pCon = nullptr;
    Port* pPort = nullptr;
    Reason reason;
public:
    Error(Container* pCon1, Port* pPort1, Reason reason1): pCon(pCon1), pPort(pPort1), reason(reason1) {}
    friend std::ostream& operator<<(std::ostream& os, const Error& c);

};


#endif //SHIP1_ERROR_H
