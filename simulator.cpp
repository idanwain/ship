
#include <string>
#include "ship.h"
#include "Parser.h"
#include "stowage_algorithm.h"
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
void initAlgorithmList(std::vector<Algorithm*> &algList,Ship* ship){
    Algorithm* lifoAlgorithm = new Lifo_algorithm(new Ship(ship));
    Algorithm* unsortedLifoAlgorithm = new Unsorted_Lifo_Algorithm(new Ship(ship));
    Algorithm* erroneousAlgorithm = new Erroneous_algorithm(new Ship(ship));
    algList.emplace_back(lifoAlgorithm);
    algList.emplace_back(unsortedLifoAlgorithm);
    algList.emplace_back(erroneousAlgorithm);
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
void destroyAlgVec(std::vector<Algorithm*> &algVec){
    for(auto &alg : algVec)
        delete alg;

    algVec.clear();
}


int main(int argc, char** argv) {

    string path = argv[argc - 1];
    vector<vector<fs::path>> directories;
    vector<Algorithm *> algVec;
    vector<string> travelNames;
    std::map<string,list<int>> outputResultsInfo;
    vector<pair<string,list<pair<string,list<string>>>>> outputAlgorithmErrors;
    std::map<string,std::map<string,list<string>>> outputSimulatorErrors;

    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);
    /*Cartesian Loop*/
    for (auto &travel_folder : directories) {
        Ship* mainShip = extractArgsForShip(travel_folder);
        if(mainShip == nullptr) continue; /* can happen if either route/map files are erroneous*/
        string currTravel = travel_folder.at(0).parent_path().filename().string();
        travelNames.push_back(currTravel);
        initAlgorithmList(algVec, mainShip);
        std::map<string,list<string>> simCurrTravelErrors;
        for (auto &alg : algVec) {
            int portNumber = -1;
            Ship* simShip = new Ship(mainShip);
            list<string> simCurrAlgErrors;
            for (size_t j = 2; j < travel_folder.size(); j++) {
                portNumber++;
                string outputPath = getFullOutPutPath(travel_folder.at(j), path, alg->getTypeName());
                string inputPath = travel_folder.at(j).string();
                (*alg)(inputPath, outputPath);
                validateAlgorithm(outputPath, inputPath, simShip, portNumber, simCurrAlgErrors);
            }
            simCurrTravelErrors.insert(make_pair(alg->getTypeName(),simCurrAlgErrors));
            delete simShip;
        }
        outputSimulatorErrors.insert(make_pair(currTravel,simCurrTravelErrors));
        saveOutputInformation(outputResultsInfo, outputAlgorithmErrors, algVec, currTravel);
        destroyAlgVec(algVec);
        delete mainShip;
    }
    createResultsFile(outputResultsInfo, travelNames, path);
    createErrorsFile(outputAlgorithmErrors,outputSimulatorErrors, path);
    return (EXIT_SUCCESS);
 }
