#ifndef SHIP1_ALGORITHMFACTORYREGISTRAR_H
#define SHIP1_ALGORITHMFACTORYREGISTRAR_H


#include <map>
#include <functional>
#include <memory>
#include "../interfaces/AbstractAlgorithm.h"
#include "../interfaces/AlgorithmRegistration.h"
#include "SimulatorObj.h"
#include <dlfcn.h>


/*------------------------------Shared Objects Deleter ---------------------------*/

struct DlCloser {
    void operator()(void *dlhandle) const noexcept {
        dlclose(dlhandle);
    }
};

class AlgorithmFactoryRegistrar {
    static AlgorithmFactoryRegistrar registrar;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> funcVec;
    //TODO make name vector synchronised ny indexes while dynamically loaded
    std::vector<std::string> nameVec;
    vector<std::unique_ptr<void, DlCloser>> SharedObjs;
public:
    ~AlgorithmFactoryRegistrar();
    void registerAlgorithmFactory(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory);
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>>& getFuncVec();
    std::vector<std::string>& getNameVec();
    bool setName(const std::string& algName);
    static AlgorithmFactoryRegistrar& getRegistrar();
    void dynamicLoadSoFiles(vector<fs::path>& algPaths, map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map);
};


#
