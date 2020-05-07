#ifndef SHIP1_ALGORITHMFACTORYREGISTRAR_H
#define SHIP1_ALGORITHMFACTORYREGISTRAR_H


#include <map>
#include <functional>
#include <memory>
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"

class AlgorithmFactoryRegistrar {
    static AlgorithmFactoryRegistrar registrar;
    std::map<std::string, std::function<std::unique_ptr<AbstractAlgorithm>()>> map;
public:
    void registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory);
    static AlgorithmFactoryRegistrar& getRegistrar();
};


#endif //SHIP1_ALGORITHMFACTORYREGISTRAR_H
