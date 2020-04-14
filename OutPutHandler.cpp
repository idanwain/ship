
#include "OutPutHandler.h"

/**
 * This function saves the results info of a certain travel route in a data structure
 * @param results_map - the data structure to save on the information
 * @param algVec - the algorithms that been tested in the simulation
 */
void saveResultsInfo(std::map<string,std::list<int>> &results_map,std::vector<Algorithm*> &algVec){
    for(Algorithm* alg : algVec){
        int num = alg->getInstructionsCounter();
        string name = alg->getTypeName();
        if(results_map.contains(name)){
            results_map.at(name).emplace_back(num);
        }
        else{
            std::list<int> ls;
            ls.emplace_back(num);
            results_map.emplace(make_pair(name, ls));
        }
    }
}

/**
 * This function saves all the error occured in an algorithm into a list and returns it
 * errors exists in all ports that have containers left in thier dock, and on the ship
 * @param alg - the algorithm to extract the errors from
 * @return - list of strings , each string is an informative error message
 */
list<string> createAlgListOfErrors(Algorithm* alg){
    auto route = alg->getShip()->get_route();
    std::set<string> visitedPorts;
    std::list<string> res;
    string msg;
    /*Iterate on every port in the travel route, on each port check for any container left in it's dock*/
    for(Port* p : route){
        if(visitedPorts.contains(p->get_name())) continue;
        visitedPorts.emplace(p->get_name());
        vector<Container> containers_vec = *(p->getContainerVec("load"));
        for(Container &cont : containers_vec){
            msg = "Error: container id: " + cont.get_id();
            if(cont.get_dest()->get_name() == "NOT_IN_ROUTE")
                msg += " is not in ship's route";
            else
                msg += ", destPort: " + cont.get_dest()->get_name() + ", currPort: " + p->get_name();
            res.emplace_back(msg);
        }
    }
    /*Iterate over the containers that left on the shipMap at the end of the travel*/
    for(const auto &pair: alg->getShip()->getContainersByPort()){
        std::vector<Container> vec = pair.second;
        for(Container &cont : vec){
            msg = "Error: container id: " + cont.get_id() + ", destPort: " + cont.get_dest()->get_name()
                  + " on ship";
            res.emplace_back(msg);
        }
    }
    return res;

}
/**
 * This function stores the Errors information
 */
void saveErrorsInfo(vector<pair<string,list<pair<string,list<string>>>>> &errors_vec,
                    vector<Algorithm *> &algVec,string &travelName){
    list<pair<string,list<string>>> list_errors;
    for(Algorithm* alg : algVec){
        string name = alg->getTypeName();
        list<string> alg_errorsList = createAlgListOfErrors(alg);
        list_errors.emplace_back(make_pair(name,alg_errorsList));
    }
    errors_vec.emplace_back(make_pair(travelName,list_errors));
}
/**
 * This function manges to save the errors and the results information in below data structures
 * @param results_map - stores results information
 * @param errors_vec - stores errors information
 * @param algVec - list of algorithms tested on
 * @param travelName - list of travel names tested on
 */
void saveOutPutInformation(std::map<string,std::list<int>> &results_map,
                           vector<pair<string,list<pair<string,list<string>>>>> &errors_vec,
                           vector<Algorithm *> &algVec,string &travelName){

    saveErrorsInfo(errors_vec,algVec,travelName);
    saveResultsInfo(results_map,algVec);
}

/**
 * This function creates a file that shows the result of the cartesian multiplication of the simulator
 * @param output_map - stores results information
 * @param travels - list of travel names
 * @param path - the file path to create the file in
 */
void createResultsFile(std::map<string,std::list<int>> &output_map,std::vector<string> &travels,string path){
    std::ofstream inFile;
    const string spaces = "      ";
    int sum = 0;
    path.append("\\simulation.results");
    inFile.open(path); //Default mode is writing to a file
    if(inFile.fail()){
        std::cout << "Failed to create results file" << std::endl;
        exit(1);
    }
    inFile << "RESULTS" << spaces+spaces;
    for(string &travel_name : travels)
        inFile << travel_name << spaces;
    inFile << "Sum" << '\n';
    //iterate over the algorithm names and iterate over them, note that p is a pair<string,list<int>>
    for(const auto &p : output_map){
        sum = 0;
        inFile << p.first << spaces;
        for(int num : p.second){
            inFile << num << spaces << spaces;
            sum += num;
        }
        inFile << sum << '\n';
    }
    inFile.close();
}

/**
 * This function creates a file that shows all the errors existed in the simulator run,
 * errors list - container didn't arrived to it's destination, container didn't picked up as the destination of
 * it isn't doesn't exist in any following ports
 * @param errors_vec - stores errors information
 * @param path - the file path to create the file in
 */
void createErrorsFile(vector<pair<string,list<pair<string,list<string>>>>> &errors_vec,string path){
    const string spaces = "     ";//6spaces
    std::ofstream inFile;
    path.append("\\simulation.errors");
    inFile.open(path);
    if(inFile.fail()){
        std::cout << "Failed to create errors file" << std::endl;
        exit(1);
    }
    for(auto &outter_pair : errors_vec){//first:string, second:list<pair...
        inFile << outter_pair.first << " Errors:" << '\n'; //travel name
        for(pair<string,list<string>> &inner_pair : outter_pair.second){
            inFile << spaces << inner_pair.first + ":" << '\n';
            for(string &msg : inner_pair.second){//inner_pair.second is type list<string>
                inFile << spaces+spaces << msg << '\n';
            }
        }
    }
    inFile.close();
}

/**
 * This function creates the outPutDirectories from list of paths.
 * @param paths
 * @param mainDirectory
 */
void createOutputDirectories(std::vector<std::vector<fs::path>> &paths,char* mainDirectory){
    string outputDir(mainDirectory);
    outputDir.append(OP_MAIN_DIRECTORY);
    fs::path dir(outputDir);
    fs::path root_path(mainDirectory);
    if(!fs::exists(dir) && !fs::create_directory(dir,root_path)){//Case we failed to create output directory.
        std::cout << "Failed to create output directory, exists program" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(auto const &list : paths) {
        if (!list.empty()) {
            string currOutputDir = outputDir + "\\" + list[0].parent_path().filename().string() + "Out";
            fs::path dir(currOutputDir);
            fs::path root_path(outputDir);
            if (!fs::exists(dir)) {
                fs::create_directory(currOutputDir, outputDir);
            }
        }
    }
}