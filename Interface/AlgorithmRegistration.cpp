#include <iostream>
#include "AlgorithmRegistration.h"
#include "../Simulator/AlgorithmFactoryRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm){
    AlgorithmFactoryRegistrar::getRegistrar().registerAlgorithmFactory(algorithm);
}
