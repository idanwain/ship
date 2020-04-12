
#include <iostream>
#include <string>
#include "ship.h"
#include "Parser.h"
#include "stowage_algorithm.h"
#include "lifo_algorithm.h"
#include "OutPutHandler.h"

/**
 * This function creates a vector of algorithms that the simulator willing to test on
 * @param algList
 * @param ship
 */
void initAlgorithmList(std::vector<Algorithm*> &algList,Ship* ship){
    Algorithm* alg = new Lifo_algorithm(new Ship(ship));
    algList.emplace_back(alg);
    //TODO need to insert another algo variants/types
}

/**
 * This function returns the full path of file to created for algorithm purposes
 * @param path
 * @param root_path
 * @return
 */
string getFullOutPutPath(fs::path &path, string &root_path, const string &algName){
    return root_path + OP_MAIN_DIRECTORY + "\\" + path.parent_path().filename().string() + "Out\\" +
    algName + "- " + path.filename().string();

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
    std::map<string,list<int>> outputResultsInfo;
    vector<pair<string,list<pair<string,list<string>>>>> outputErrorInfo;
    vector<string> travelNames;

    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);

    for (auto &travel_folder : directories) {
        Ship* mainShip = extractArgsForShip(travel_folder);//TODO check why empty root exists
        if(mainShip == nullptr) continue;
        string currTravel = travel_folder.at(0).parent_path().filename().string();
        travelNames.push_back(currTravel);
        mainShip->initCalc();
        initAlgorithmList(algVec, mainShip);
        for (auto &alg : algVec) {
            for (size_t j = 2; j < travel_folder.size(); j++) {
                string outputPath = getFullOutPutPath(travel_folder.at(j), path, alg->getTypeName());
                string inputPath = travel_folder[j].string();
                (*alg)(inputPath, outputPath);
            }
        }
        saveOutPutInformation(outputResultsInfo,outputErrorInfo, algVec,currTravel);
        delete mainShip;
        destroyAlgVec(algVec);
    }
    createResultsFile(outputResultsInfo, travelNames, path);
    createErrorsFile(outputErrorInfo,path);
 }
