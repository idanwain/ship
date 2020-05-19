#include "../interfaces/AlgorithmRegistration.h"
#include "AlgorithmFactoryRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm){
    AlgorithmFactoryRegistrar::getRegistrar().registerAlgorithmFactory(algorithm);
}
