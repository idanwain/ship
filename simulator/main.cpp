/**
 * This module is includes the main function which simulates the cartesian loop such that for every travel folder
 * and every algorithm in runs the simulation. given a list of {Alg1,Alg2,Alg3...} and a list of {Travel1,Travel2..}
 * for every i,and j the simulator runs Algi over Travelj, every iteration over Travelj the algorithm gets from the
 * simulator the next port cargo file, which includes containers awaiting at this port then :
 * 1. the algorithm generates crane instructions file including Load/Unload/Move/Reject operations
 * 2. the simulator validates the crane instructions file generated by the algorithm, and gather erroneous operations
 * after finishing Travelj and prior to run Travelx the simulator saves the information it gathered in a data structure.
 * at the end while every Algi iterated over every Travelj the simulator creates 2 output files :
 * 1. Simulation.errors  - including the general errors the simulator found, so the errors it found validating the algs.
 * 2. Simulation.results - representing the total number of L/R/M instructions of every algorithm on every Travel, also
 * the total number of errors a given algorithm made over all the travel folders, sorted by following criteria :
 * 1. first will be shown the algorithm with the lowest errors occurred(generated by the simulator).
 * 2. second will be compared only iff 2 algorithms errors count is same the it will be sorted by number of instructions.
 * Note* - if no output path given -> output files will be at the directory the main program runs from.
 */
#include <string>
#include "../common/Ship.h"
#include "../common/Parser.h"
#include "AlgorithmFactoryRegistrar.h"
#include <dlfcn.h>
#include <memory>

/*------------------------------Global Variables---------------------------*/

string mainTravelPath;
string mainAlgorithmsPath = fs::current_path().string();
string mainOutputPath = fs::current_path().string();

/*------------------------------Shared Objects Deleter ---------------------------*/

struct DlCloser {
    void operator()(void *dlhandle) const noexcept {
        std::cout << "in deleter" << std::endl;
        dlclose(dlhandle);
    }
};

/*-----------------------------Utility Functions-------------------------*/

/**
 * This function creates a vector of algorithms that the simulator willing to test on
 * @param algList
 * @param ship
 */
void initAlgorithmList(vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> &algList, map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map){
    for(auto &entry: map){
        algList.emplace_back(make_pair(entry.first,entry.second()));
    }
}

/**
 * This function iterate through the vector and delete each algorithm
 * @param algVec
 */
void destroyAlgVec(vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> &algVec){
    for(auto& alg : algVec){
        if(alg.second){
            alg.second.release();
            std::cout << "alg is alive" << std::endl;
        } else {
            std::cout << "alg is dead" << std::endl;
        }
    }
    algVec.clear();
}


/**
 * This function gets the paths or sets them to be the current working directory
 * @param argc
 * @param argv
 */
void initPaths(int argc,char** argv){
    string basePath = fs::current_path().string();
    const string travelFlag = "-travel_path";
    const string outputFlag = "-output_path";
    const string algorithmFlag = "-algorithm_path";

    for(int i = 1; i+1 < argc; i++){
        if(argv[i] == travelFlag)
            mainTravelPath = argv[i+1];
        else if(argv[i] == outputFlag)
            mainOutputPath = argv[i+1];
        else if(argv[i] == algorithmFlag)
            mainAlgorithmsPath = argv[i+1];
    }

    if(mainTravelPath.empty()) {
        ERROR_NOTRAVELPATH;
        exit(EXIT_FAILURE);
    }
}

/**
 * This function gets the algorithms.so files from the mainAlgorithms path (if given or from current path)
 * and saves the paths in the given vector.
 * @param algPaths
 */
void getAlgSoFiles(vector<fs::path> &algPaths){
    std::regex reg("_[0-9]+_[a-z]+\\.so");
    for(const auto &entry : fs::directory_iterator(mainAlgorithmsPath)) {
        if (!entry.is_directory()) {
            if (std::regex_match(entry.path().filename().string(), reg)) {
                algPaths.emplace_back(entry);
            }
        }
    }
}

void dynamicLoadSoFiles(vector<fs::path>& algPaths, vector<std::unique_ptr<void, DlCloser>>& SharedObjs,
        map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map){
   for(auto& path : algPaths){
       std::unique_ptr<void, DlCloser> soAlg(dlopen(path.c_str(), RTLD_LAZY));
       if(!soAlg){
           std::cerr << "dlopen failed" << dlerror() << std::endl;
       } else {
           string algName = path.stem().string();
           auto& registrar = AlgorithmFactoryRegistrar::getRegistrar();
           if(registrar.setName(algName)){
               SharedObjs.emplace_back(std::move(soAlg));
               auto& nameVec = registrar.getNameVec();
               int pos = std::distance(nameVec.begin(), std::find(nameVec.begin(), nameVec.end(), algName));
               std::function<std::unique_ptr<AbstractAlgorithm>()>& algorithmFactory = registrar.getFuncVec().at(pos);
               map.insert({algName, algorithmFactory});
           } else {
               P_ALGNOTREGISTER(algName);
           }
       }
   }
}

void destroySharedObjs(vector<std::unique_ptr<void, DlCloser>>& vector) {
    for(auto &ptr : vector)
        ptr.reset(nullptr);

    vector.clear();
}

int main(int argc, char** argv) {

    map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>> map;
    vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> algVec;
    vector<std::unique_ptr<void, DlCloser>> SharedObjs;
    vector<fs::path> algPaths;
    initPaths(argc,argv);
    SimulatorObj simulator(mainTravelPath,mainOutputPath);
    getAlgSoFiles(algPaths);
    std::cout << "before open so" << std::endl;
    dynamicLoadSoFiles(algPaths, SharedObjs, map);


    /*Cartesian Loop*/
    for (auto &travel : simulator.getTravels()) {
        std::cout << "in main loop" << std::endl;
        initAlgorithmList(algVec, map);
        std::unique_ptr<Ship> mainShip = extractArgsForShip(travel,simulator);
        if(mainShip != nullptr){
            for (auto &alg : algVec) {
                std::cout << "start inner loop" << std::endl;
                WeightBalanceCalculator algCalc;
                int errCode1 = alg.second->readShipPlan(travel->getPlanPath().string());
                std::cout << "after readShipPlan" << std::endl;
                int errCode2 = alg.second->readShipRoute(travel->getRoutePath().string());
                std::cout << "after readShipRoute" << std::endl;
                int errCode3 = algCalc.readShipPlan(travel->getPlanPath().string());
                std::cout << "after readShipPlan calc" << std::endl;
                alg.second->setWeightBalanceCalculator(algCalc);
                simulator.updateArrayOfCodes(errCode1 + errCode2 + errCode3,"alg");
                simulator.setShipAndCalculator(mainShip, travel->getPlanPath().string());
                simulator.runCurrentAlgorithm(alg,travel);
                simulator.getShip().reset(nullptr);
                std::cout << "in if statement end inner loop" << std::endl;
            }
        }
        std::cout << "after if statement inner loop" << std::endl;
        simulator.prepareForNewTravel();
        mainShip.reset(nullptr);
        destroyAlgVec(algVec);
        std::cout << "after if statement end inner loop" << std::endl;
    }
    std::cerr << "before file creating" << endl;
    simulator.createResultsFile(mainTravelPath);
    simulator.createErrorsFile(mainTravelPath);
    destroySharedObjs(SharedObjs);
    std::cerr << "IN MAIN LAST ROW" << endl;
    std::cerr << "NOTICE: this core dump happens only at the end of the program" << endl;
    return (EXIT_SUCCESS);
 }

