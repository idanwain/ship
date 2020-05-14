#ifndef SHIP1_ALGORITHMFACTORYREGISTRAR_H
#define SHIP1_ALGORITHMFACTORYREGISTRAR_H


#include <map>
#include <functional>
#include <memory>
#include "../interfaces/AbstractAlgorithm.h"
#include "../interfaces/AlgorithmRegistration.h"
#include "SimulatorObj.h"

class AlgorithmFactoryRegistrar {
    static AlgorithmFactoryRegistrar registrar;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> funcVec;
    //TODO make name vector synchronised ny indexes while dynamically loaded
    std::vector<std::string> nameVec;
public:
    void registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory);
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>>& getFuncVec();
    std::vector<std::string>& getNameVec();
    bool setName(const std::string& algName);

    static AlgorithmFactoryRegistrar& getRegistrar();
};


#endif //SHIP1_ALGORITHMFACTORYREGISTRAR_H
