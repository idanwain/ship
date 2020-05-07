//
// Created by idanw on 07/05/2020.
//

#include "AlgorithmFactoryRegistrar.h"

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    auto alg_name = getAlgorithmName();
    map.insert(std::make_pair(alg_name, algorithmFactory));
}

AlgorithmFactoryRegistrar &AlgorithmFactoryRegistrar::getRegistrar() {
    return registrar;
}
