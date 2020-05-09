#include "AlgorithmFactoryRegistrar.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

AlgorithmFactoryRegistrar AlgorithmFactoryRegistrar::registrar;

void AlgorithmFactoryRegistrar::registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    std::string algTypeidName = typeid(*algorithmFactory()).name();
    auto format = ".cpp";
    for(auto& entry: fs::directory_iterator([]() {
        char buffer[MAX_PATH];
        GetModuleFileName( nullptr, buffer, MAX_PATH );
        std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
        return std::string( buffer ).substr( 0, pos);
    }() + "\\..\\algorithm")){
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
