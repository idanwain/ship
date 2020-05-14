#include "AlgorithmFactoryRegistrar.h"
#include <filesystem>

namespace fs = std::filesystem;

AlgorithmFactoryRegistrar AlgorithmFactoryRegistrar::registrar;

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmFactoryRegistrar::registrar.funcVec.push_back(algorithmFactory);
}


AlgorithmFactoryRegistrar &AlgorithmFactoryRegistrar::getRegistrar() {
    return registrar;
}

std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &AlgorithmFactoryRegistrar::getFuncVec() {
    return this->funcVec;
}

std::vector<std::string> &AlgorithmFactoryRegistrar::getNameVec() {
    return this->nameVec;
}

bool AlgorithmFactoryRegistrar::setName(const std::string &algName) {
    if(funcVec.size() == nameVec.size()) return false;
    nameVec.emplace_back(algName);
    return true;
}



