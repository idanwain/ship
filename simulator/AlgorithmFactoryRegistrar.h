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
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> vec;
public:
    void registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory);
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>>& getVec();

    static AlgorithmFactoryRegistrar& getRegistrar();
};


#endif //SHIP1_ALGORITHMFACTORYREGISTRAR_H
