
#include <string>
#include "ship.h"
#include "Parser.h"
#include "AbstractAlgorithm.h"
#include "lifo_algorithm.h"
#include "Unsorted_Lifo_Algorithm.h"
#include "erroneous_algorithm.h"
#include "outputHandler.h"
#include "common.h"

/**
 * This function creates a vector of algorithms that the simulator willing to test on
 * @param algList
 * @param ship
 */
void initAlgorithmList(std::vector<std::unique_ptr<AbstractAlgorithm>>& algList, vector<fs::path> &folder){
    std::string ship_plan_file_path = folder[0].string();
    std::string route_file_path = folder[1].string();

    //TODO make polymorphic algorithm factory & change to smart pointers
    std::unique_ptr<AbstractAlgorithm> lifoAlgorithm = std::make_unique<Lifo_algorithm>();
//    std::unique_ptr<AbstractAlgorithm> unsortedLifoAlgorithm = std::make_unique<Unsorted_Lifo_Algorithm>();
//    std::unique_ptr<AbstractAlgorithm> erroneousAlgorithm = std::make_unique<Erroneous_algorithm>();
    algList.emplace_back(std::move(lifoAlgorithm));
//    algList.emplace_back(unsortedLifoAlgorithm);
//    algList.emplace_back(erroneousAlgorithm);
    //init alg data
    for(auto& alg : algList){
        alg->readShipPlan(ship_plan_file_path);
        alg->readShipRoute(route_file_path);
        //alg->setWeightBalanceCalculator(); //TODO deal with the calc
    }
}

/**
 * This function returns the full path of file to created for algorithm purposes
 * @param path
 * @param root_path
 * @return
 */
string getFullOutPutPath(fs::path &path, string &root_path, const string &algName){
    return root_path + PATH_SEPARATOR +  OP_MAIN_DIRECTORY + PATH_SEPARATOR + path.parent_path().filename().string()
    + "Out" + PATH_SEPARATOR + algName + "- " + path.filename().string();

}

/**
 * This function iterate through the vector and delete each algorithm
 * @param algVec
 */
void destroyAlgVec(std::vector<std::unique_ptr<AbstractAlgorithm>> &algVec){
    for(auto &alg : algVec)
        alg.reset(nullptr);

    algVec.clear();
}


int main(int argc, char** argv) {

    string path = argv[argc - 1];
    vector<vector<fs::path>> directories;
    vector<std::unique_ptr<AbstractAlgorithm>> algVec;
    vector<string> travelNames;
    std::map<string,list<int>> outputResultsInfo;
    vector<pair<string,list<pair<string,list<string>>>>> outputAlgorithmErrors;
    std::map<string,std::map<string,list<string>>> outputSimulatorErrors;

    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);
    /*Cartesian Loop*/
    for (auto &travel_folder : directories) {
        list<string> currTravelGeneralErrors;
        std::unique_ptr<Ship> mainShip = extractArgsForShip(travel_folder,currTravelGeneralErrors);
        if(mainShip == nullptr)
            continue; /* can happen if either route/map files are erroneous*/
        mainShip->initCalc();
        string currTravel = travel_folder.at(0).parent_path().filename().string();
        travelNames.push_back(currTravel);
        initAlgorithmList(algVec, travel_folder);
        std::map<string,list<string>> simCurrTravelErrors;
        for (auto &alg : algVec) {
            int portNumber = -1;
            std::unique_ptr<Ship> simShip = std::make_unique<Ship>(*mainShip);
            list<string> simCurrAlgErrors;
            for (size_t j = 2; j < travel_folder.size(); j++) {
                portNumber++;
                string outputPath = getFullOutPutPath(travel_folder.at(j), path, typeid(alg).name()); //no getName function in AbsAlg class
                string inputPath = travel_folder.at(j).string();
                alg->getInstructionsForCargo(inputPath, outputPath);
                validateAlgorithm(outputPath, inputPath, simShip, portNumber, simCurrAlgErrors);
            }
            simCurrTravelErrors.insert(make_pair(typeid(alg).name(),simCurrAlgErrors));
            simShip.reset(nullptr);
        }
        outputSimulatorErrors.insert(make_pair(currTravel,simCurrTravelErrors));
        //TODO: need to pass a ship param to saveOutputInfo function in purpose of data separation & ownership
        /**
         * Info about containers that didnt end up at their destination (on board / other port)
         * should be derived from mainShip / simShip
         * We need not to rely on algorithm data (members or functions) at all except from the output of it.
         */
        //saveOutputInformation(outputResultsInfo, outputAlgorithmErrors, algVec, currTravel);
        destroyAlgVec(algVec);
        mainShip.reset(nullptr);
    }
    createResultsFile(outputResultsInfo, travelNames, path);
    createErrorsFile(outputAlgorithmErrors,outputSimulatorErrors, path);
    return (EXIT_SUCCESS);
 }
