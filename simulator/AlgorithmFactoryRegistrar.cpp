#include "AlgorithmFactoryRegistrar.h"
#include <filesystem>

namespace fs = std::filesystem;

AlgorithmFactoryRegistrar AlgorithmFactoryRegistrar::registrar;

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmFactoryRegistrar::registrar.vec.push_back(algorithmFactory);
}


AlgorithmFactoryRegistrar &AlgorithmFactoryRegistrar::getRegistrar() {
    return registrar;
}

std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &AlgorithmFactoryRegistrar::getVec() {
    return this->vec;
}



