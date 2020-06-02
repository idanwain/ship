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
  Note - if no output path given -> output files will be at the directory the main program runs from.
 */
#include <string>
#include "../common/Ship.h"
#include "../common/Parser.h"
#include "AlgorithmFactoryRegistrar.h"
#include "ThreadPoolExecuter.h"
#include "TaskProducer.h"
#include <dlfcn.h>
#include <memory>
#include <Util.h>

/*------------------------------Global Variables---------------------------*/

string mainTravelPath;
string mainAlgorithmsPath;
string mainOutputPath;

/*-----------------------------Utility Functions-------------------------*/

/**
 * This function creates a vector of algorithms that the simulator willing to test on
 * @param algList
 * @param ship
 */
vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> initAlgorithmList(map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>>& map){
    vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> algList;
    for(auto &entry: map){
        algList.emplace_back(make_pair(entry.first,entry.second()));
    }
    return algList;
}

//vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> initAlgorithmList(){
//    vector<pair<string,std::unique_ptr<AbstractAlgorithm>>> algList;
//    algList.emplace_back(make_pair("_313263204_a",std::make_unique<_313263204_a>()));
//    algList.emplace_back(make_pair("_313263204_b",std::make_unique<_313263204_b>()));
//    return algList;
//}

/**
 * This function gets the paths or sets them to be the current working directory
 * @param argc
 * @param argv
 */
void initPaths(int argc,char** argv){
    string basePath = fs::current_path().string();
    const string travelFlag = "-travel_path";
    const string outputFlag = "-output";
    const string algorithmFlag = "-algorithm_path";

    for(int i = 1; i+1 < argc; i++){
        if(argv[i] == travelFlag)
            mainTravelPath = argv[i+1];
        else if(argv[i] == outputFlag)
            mainOutputPath = argv[i+1];
        else if(argv[i] == algorithmFlag)
            mainAlgorithmsPath = argv[i+1];
    }
    if(mainOutputPath.empty() || !fs::exists(mainOutputPath))
        mainOutputPath = basePath;
    if(mainAlgorithmsPath.empty() || !fs::exists(mainAlgorithmsPath))
        mainAlgorithmsPath = basePath;

    if(mainTravelPath.empty()) {
        NO_TRAVEL_PATH;
        exit(EXIT_FAILURE);
    }
}

/**
 * This function gets the algorithms.so files from the mainAlgorithms path (if given or from current path)
 * and saves the paths in the given vector.
 */
void getAlgSoFiles(vector<fs::path> &algPaths){
    std::regex reg(".*\\.so");
    for(const auto &entry : fs::directory_iterator(mainAlgorithmsPath)) {
        if (!entry.is_directory()) {
            if (std::regex_match(entry.path().filename().string(), reg)) {
                algPaths.emplace_back(entry);
            }
        }
    }
}

int main(int argc, char** argv) {
    map<string ,std::function<std::unique_ptr<AbstractAlgorithm>()>> map;
    vector<std::unique_ptr<Travel>> TravelsVec;
    vector<fs::path> algPaths;
    tasksContainer tasks;
    list<string> generalErrors;
    initPaths(argc,argv);
    initListOfTravels(mainTravelPath,generalErrors,TravelsVec);
    getAlgSoFiles(algPaths);
    auto& registrar = AlgorithmFactoryRegistrar::getRegistrar();
    registrar.dynamicLoadSoFiles(algPaths, map);
    initTasksContainer(tasks,map,TravelsVec);

    std::vector<std::pair<std::unique_ptr<Travel>, vector<pair<string,std::unique_ptr<AbstractAlgorithm>>>>> travelForAlgs; //TODO: init this vec


    /************* MULTITHREADED TRAVEL X ALGORITHMS *************/
    //this block of code would be putted after the initialization of "Travel X algVec" vector
    //it will replace the for loop under this block
    ThreadPoolExecuter executer {TaskProducer{travelForAlgs}, NumThreads{5} }; //TODO: get number of threads from agrv
    executer.start();
    executer.wait_till_finish();


    //producer --> travel3 x algVec , //thread --> travel4 x algVec //thread --> travel4 x algVec //thread --> travel5 x algVec
    /*Cartesian Loop*/
    for (auto &travel : TravelsVec) { //thread --> travel3 x algVec , //thread --> travel4 x algVec
                                                    //thread --> travel4 x algVec //thread --> travel5 x algVec
        SimulatorObj simulator(mainOutputPath);
        std::unique_ptr<Ship> mainShip = extractArgsForShip(travel, simulator);
        if(mainShip != nullptr){
            for (auto &alg : algVec) {
                int errCode1 = 0, errCode2 = 0;
                WeightBalanceCalculator algCalc;
                try {
                    errCode1 = alg.second->readShipPlan(travel->getPlanPath().string());
                    errCode2 = alg.second->readShipRoute(travel->getRoutePath().string());
                    errCode1 |= algCalc.readShipPlan(travel->getPlanPath().string());
                }
                catch(...) {
                    travel->setAlgCrashError(alg.first);
                    continue;
                }
                alg.second->setWeightBalanceCalculator(algCalc);
                simulator.updateErrorCodes(errCode1 + errCode2, "alg");
                simulator.setShipAndCalculator(mainShip, travel->getPlanPath().string());
                simulator.runAlgorithm(alg, travel);
            }
        }
        else
            travel->setErroneousTravel();
        simulator.prepareNextIteration();
    }
    //Join here
    createResultsFile(mainOutputPath,TravelsVec);
    createErrorsFile(mainOutputPath,TravelsVec,generalErrors);
    return (EXIT_SUCCESS);
}