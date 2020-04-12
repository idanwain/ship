
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

void saveOutPutInformation(std::map<string,std::list<int>> &output_map,std::vector<Algorithm *> algVec){
    for(Algorithm* alg : algVec){
        int num = alg->getInstructionsCounter();
        string name = alg->getTypeName();
        if(output_map.contains(name)){
            output_map.at(name).emplace_back(num);
        }
        else{
            std::list<int> ls;
            ls.emplace_back(num);
            output_map.insert(make_pair(name,ls));
        }

    }
}

void createOutPutFile(std::map<string,std::list<int>> &output_map,std::vector<string> &travels,string path){
    std::ofstream inFile;
    const string spaces = "      ";
    int sum = 0;
    path.append("\\simulation.csv");
    inFile.open(path); //Default mode is writing to a file
    if(inFile.fail()){
        std::cout << "Failed to create results file" << std::endl;
        exit(1);
    }
    inFile << "RESULTS" << ",";
    for(string &travel_name : travels)
        inFile << travel_name << ",";
    inFile << "Sum" << '\n';
    //iterate over the algorithm names and iterate over them, note that p is a pair<string,list<int>>
    for(const auto &p : output_map){
        sum = 0;
        inFile << p.first << ",";
        for(int num : p.second){
            inFile << num << ",";
            sum += num;
        }
        inFile << sum << '\n';
    }
    inFile.close();
}

int main(int argc, char** argv) {

    string path = argv[argc - 1];
    std::vector<std::vector<fs::path>> directories;
    std::vector<Algorithm *> algVec;
    std::map<string,std::list<int>> outputInformation;
    std::vector<string> travelsNum;

    initListDirectories(path, directories);
    createOutputDirectories(directories, argv[1]);

    for (auto &travel_folder : directories) {
        Ship* mainShip = extractArgsForShip(travel_folder);
        if(mainShip == nullptr) continue;
        travelsNum.push_back(travel_folder.at(0).parent_path().filename().string());
        mainShip->initCalc();
        initAlgorithmList(algVec, mainShip);
        for (auto &alg : algVec) {
            for (size_t j = 2; j < travel_folder.size(); j++) {
                string outputPath = getFullOutPutPath(travel_folder.at(j), path, alg->getTypeName());
                string inputPath = travel_folder[j].string();
                (*alg)(inputPath, outputPath);
            }
        }
        saveOutPutInformation(outputInformation,algVec);
        delete mainShip;
        destroyAlgVec(algVec);
    }
    createOutPutFile(outputInformation,travelsNum,path);
 }
