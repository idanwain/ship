#include "Parser.h"


int getPortNumFile(const string& file_name){
    if(file_name.size() < 6) return 0;
    int dot = file_name.find(".");
    int dash = file_name.find("_") + 1;
    string numPort = file_name.substr(dash,dot-dash);
    return atoi(numPort.data());

}

bool isValidPortExpressionFile(const string& file_name){
    std::regex reg("\\s*[A-Z]{2}\\s+[A-Z]{3}_[1-9]+.[a-z]*");
    return std::regex_match(file_name,reg);
}

bool isValidPortName(const string& name){
    std::regex reg("\\s*[A-Z]{2}\\s+[A-Z]{3}s*$");
    return std::regex_match(name,reg);
}

std::vector<std::vector<fs::path>> orderListOfDir(std::list<std::list<fs::path>> &unOrdered){
    std::vector<std::vector<fs::path>> result(unOrdered.size());
    int ind = 0;
    int numFile = 0;
    string file_name;
    for(auto &list : unOrdered){
        result[ind].resize(list.size());
        for(auto &path : list){
            file_name = path.filename().string();
            if(file_name == "ship_plan.txt"){
                result[ind][0] = path;
            }
            else if(file_name == "ship_route.txt"){
                result[ind][1] = path;
            }
            else if(isValidPortExpressionFile(file_name)){
                numFile = getPortNumFile(file_name)+1; //as 0,1 places are for plan and route.
                if(!result[ind][numFile].empty()){
                    std::cout << "Error: there's already another port file with this number" << std::endl;
                }
                else{
                    result[ind][numFile] = path;
                }
            }
            else{
                std::cout << "Error:" + file_name + " doesn't match any file format" << std::endl;
            }
        }
        result[ind].shrink_to_fit();
        ind++;
    }
    return result;

}
void initListDirectories(string &path,std::vector<std::vector<fs::path>> &vecOfPaths) {
    string msg = " Only sub folders allowed in main folder, file won't be included in the program..";
    std::list<std::list<fs::path>> unOrderedList;
    for (const auto &entry : fs::directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::cout <<  entry.path().filename()  << msg << std::endl;
            continue;
        }
        unOrderedList.emplace_back(std::list<fs::path>());
        for (const auto &deep_entry : fs::directory_iterator(entry.path())) {
            unOrderedList.back().emplace_back((deep_entry.path()));
        }
    }
    vecOfPaths = orderListOfDir(unOrderedList);
}

/**
 * This function gets the paths matrix and validate that files exist.
 * @param direct
 */
void validateSequenceDirectories(std::vector<std::vector<fs::path>> &direct) {
    string msg;
    for (int i = 0; i < direct.size(); i++) {
        for (int j = 0; j < direct[i].size(); j++) {
            if (direct[i][j].empty()) {
                if (j == 0) msg = "ship_plan";
                else if (j == 1) msg = "ship_route";
                else msg.append("port num " + std::to_string(j+1));
                std::cout << "Folder Travel " << ++i << " lack of " << msg
                          << " file, this travel folder isn't going to be tested" << std::endl;
                direct.erase(direct.begin()+i); //Erase the i'th element from the vector
            }
        }
    }
}


