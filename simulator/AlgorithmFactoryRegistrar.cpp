#include "AlgorithmFactoryRegistrar.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

AlgorithmFactoryRegistrar AlgorithmFactoryRegistrar::registrar;

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    std::string algTypeidName = typeid(*algorithmFactory()).name();
    auto format = ".so";
    std::string path = fs::current_path().string() + PATH_SEPARATOR + ".." + PATH_SEPARATOR + "algorithm";
    //TODO Need to access algorithm folder

    for(auto& entry: fs::directory_iterator(path)){
        if (!entry.is_directory()) {
            auto filename = entry.path().filename().string();
            if (auto index = filename.find(format); index != std::string::npos) {
                auto algName = filename.substr(0, index);
                if(algTypeidName.find(algName) != std::string::npos){
                    map.insert(std::make_pair(algName, algorithmFactory));
                    break;
                }
            }
        }
    }

}

AlgorithmFactoryRegistrar &AlgorithmFactoryRegistrar::getRegistrar() {
    return registrar;
}
std::map<std::string, std::function<std::unique_ptr<AbstractAlgorithm>()>>& AlgorithmFactoryRegistrar::getMap(){
    return this->map;
}

