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
    std::map<std::string, std::function<std::unique_ptr<AbstractAlgorithm>()>> map;
public:
    void registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory);
    std::map<std::string, std::function<std::unique_ptr<AbstractAlgorithm>()>>& getMap();

    static AlgorithmFactoryRegistrar& getRegistrar();
};


#endif //SHIP1_ALGORITHMFACTORYREGISTRAR_H
