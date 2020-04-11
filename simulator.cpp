
#include <iostream>
#include <string>
#include "ship.h"
#include "Parser.h"
#include "stowage_algorithm.h"
#include "lifo_algorithm.h"
#include "WeightBalanceCalculator.h"

/*
 * TODO function only for debug purposes --> to delete.
 */
void printWhereBlocks(Ship* &ship){
        auto map = ship->get_map();
        for(size_t i = 0; i < (*map).size(); i++){
            for(size_t j = 0; j < (*map)[i].size(); j++){
                for(size_t k = 0; k < (*map)[i][j].size(); k++){
                    cout << "size is " << (*map)[i][j].size() << endl;
                    if((*map)[i][j][k].get_id() == "block"){
                        cout << "at " << i << "," << j << "," << k <<endl;
                    }
                }
            }
        }
}

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
void destroyAlgVec(std::vector<Algorithm*> &algVec){
    for(auto &alg : algVec){
        delete alg;
    }
    algVec.clear();
}

int main(int argc, char** argv) {

    string path = argv[argc - 1];
    std::vector<std::vector<fs::path>> directories;
    std::vector<Algorithm *> algVec;
    std::list<std::list<string>> outputInformation;
    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);

    for (auto &folder : directories) {
        Ship* mainShip = extractArgsForShip(folder);
        if(mainShip == nullptr) continue;
        mainShip->initCalc();
        initAlgorithmList(algVec, mainShip);
        for (auto &alg : algVec) {
            for (size_t j = 2; j < folder.size(); j++) {
                string outputPath = getFullOutPutPath(folder.at(j), path,alg->getTypeName());
                string inputPath = folder[j].string();
                (*alg)(inputPath, outputPath);
            }
        }
        delete mainShip;
        destroyAlgVec(algVec);
    }
 }
