#include "AlgorithmFactoryRegistrar.h"
#include <filesystem>

namespace fs = std::filesystem;

/**
 * static instance initialization
 */
AlgorithmFactoryRegistrar AlgorithmFactoryRegistrar::registrar;

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmFactoryRegistrar::registrar.funcVec.push_back(algorithmFactory);
}

/**
 * @return - instance of the static instance of AlgorithmFactoryRegistrar
 */
AlgorithmFactoryRegistrar &AlgorithmFactoryRegistrar::getRegistrar() {
    return registrar;
}

std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &AlgorithmFactoryRegistrar::getFuncVec() {
    return this->funcVec;
}

/**
 * push back algName to nameVec iff an algorithm registered
 * @param algName - name of the algorithm
 * @return - true if algorithm registered, false otherwise.
 */
bool AlgorithmFactoryRegistrar::setName(const std::string &algName) {
    if(funcVec.size() == nameVec.size()) return false;
    nameVec.emplace_back(algName);
    return true;
}

AlgorithmFactoryRegistrar::~AlgorithmFactoryRegistrar() {
    funcVec.clear();
    nameVec.clear();
    SharedObjs.clear();
}

/**
 * loads dynamically the .so files of the algorithms
 * @param algPaths - path to algorithms directory
 * @param map - data structures that holds pairs of (name, algorithm functors).
 */
void AlgorithmFactoryRegistrar::dynamicLoadSoFiles(vector<fs::path>& algPaths,
                                                   map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map){
    for(auto& path : algPaths){
        std::unique_ptr<void, DlCloser> soAlg(dlopen(path.c_str(), RTLD_LAZY));
        if(!soAlg){
            std::cerr << "dlopen failed" << dlerror() << std::endl;
        } else {
            string algName = path.stem().string();
            if(this->setName(algName)){
                this->SharedObjs.emplace_back(std::move(soAlg));
                int pos = std::distance(nameVec.begin(), std::find(nameVec.begin(), nameVec.end(), algName));
                std::function<std::unique_ptr<AbstractAlgorithm>()>& algorithmFactory = this->getFuncVec().at(pos);
                map.insert({algName, algorithmFactory});
            } else {
                P_ALGNOTREGISTER(algName);
            }
        }
    }
}





