#include "Parser.h"
#include "stowage_algorithm.h"

/**
 * This function gets the number from the <port_symbol>_<num>.<filetype> deceleration
 * @param fileName
 * @return the number
 */
int getPortNumFile(const string& fileName){
    if(fileName.size() < 6) return 0;
    int dot = fileName.find(".");
    int dash = fileName.find("_") + 1;
    string numPort = fileName.substr(dash, dot - dash);
    return atoi(numPort.data());

}

/**
 * This function checks if the port file is valid aka <port_symbol>_<num>.<filetype>
 * @param fileName
 * @return true iff it's in the right format
 */
bool isValidPortExpressionFile(const string& fileName){
    std::regex reg("[A-Za-z]{5}_[1-9]+.cargo_data");
    return std::regex_match(fileName, reg);
}

/**
 * This function checks if the travel name folder is in the right format aka "Travel" followed by any valid numbers
 * @param travelName
 * @return true iff it's in the right format
 */
bool isValidTravelName(const string& travelName){
    std::regex reg("Travel[1-9]+");
    return std::regex_match(travelName, reg);
}

/**
 * This function "sort" the given list of directories such that -
 * first file in a sub folder list is ship_plan,second file is ship_route,then put the port_num files in order
 * @param unOrdered - the unOrdered paths
 * @return ordered paths
 */
vector<vector<fs::path>> orderListOfDir(list<list<fs::path>> &unOrdered){
    vector<vector<fs::path>> result(unOrdered.size());
    int ind = 0;
    int numFile = 0;
    string file_name;
    for(auto &list : unOrdered){/*Each list is a Travel folder containing cargo_data route, map etc..*/
        result[ind].resize(list.size());
        for(auto &path : list){/*path is a file in a Travel folder*/
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
                    std::cerr << "Error: there's already another port file with this number, ignoring later one" << std::endl;
                }
                else{
                    result[ind][numFile] = path;
                }
            }
            else{
                std::cerr << "Error: " + file_name + " doesn't match any file format" << std::endl;
            }
        }
        result[ind].shrink_to_fit();
        ind++;
    }
    return result;

}
/**
 * This function initlialzie the list of directories that the simulator is aiming to run the simulation on
 * @param path - the path of the main directory that includes all travels sub folders
 * @param vecOfPaths - the vectors of paths to save the information
 */
void initListDirectories(string &path,vector<vector<fs::path>> &vecOfPaths) {
    string msg = " only sub folders allowed in main folder, file won't be included in the program";
    list<list<fs::path>> unOrderedList;
    for (const auto &entry : fs::directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::cerr << "Error: "  << entry.path().filename().string()  << msg << std::endl;
            continue;
        }
        if(!isValidTravelName(entry.path().filename().string())) continue;
        unOrderedList.emplace_back(list<fs::path>());
        for (const auto &deep_entry : fs::directory_iterator(entry.path())) {
            unOrderedList.back().emplace_back((deep_entry.path()));
        }
    }
    vecOfPaths = orderListOfDir(unOrderedList);
    setActualSize(vecOfPaths);
    //validateSequenceDirectories(vecOfPaths);
}

/**
 * This function gets the paths matrix and validate that files exist.
 * @param direct
 */
void validateSequenceDirectories(vector<vector<fs::path>> &direct) {
    string msg;
    int num = 0;
    setActualSize(direct);
    for (size_t i = 0; i < direct.size(); i++) {
        num++;
        for (size_t j = 0; j < direct[i].size(); j++) {
            if (direct[i][j].empty()) {
                msg.clear();
                if (j == 0) msg = "ship_plan";
                else if (j == 1) msg = "ship_route";
                else msg = ("port num " + std::to_string(j-1));
                std::cerr << "Error: Folder Travel " << num << " lack of " << msg
                          << " file, this travel folder isn't going to be tested" << std::endl;
                direct[i].clear();
                break;
            }
        }
    }
}

/**
 * This function sets each sub folder to it's actual size vector
 * @post vec[i][vec[i].size()-1] != empty for every i=0..direct.size()-1
 * @param direct
 */
void setActualSize(vector<vector<fs::path>> &direct){
    for(size_t i = 0; i < direct.size(); i++){
        for(size_t j = direct[i].size()-1; !direct[i].empty() && j > 0; j--){
            if(!direct[i][j].empty()){
                direct[i].resize(j+1);
                break;
            }
        }
    }
}


/**
 * This function checks if port already exist in the vector list, if exists it return 0 but
 * sets a pointer to the same port at the the end of the vector.
 * @param vec - the vector of ports
 * @param str - the string of the port to be check
 * @return 0 iff exist already port with same name
 */
int portAlreadyExist(vector<Port*> &vec,string &str){
    for(const auto &element : vec ){
        if(element->get_name() == str){
            vec.emplace_back(element);
            return 1;
        }
    }
    return 0;
}

/**
 * This function parsing the dimensions of a ship/container location from file/string
 * @param arr - the array to store the dimensions in
 * @param inFile - the file we are working on to parse the data from
 * @param str - byFile means we parse from file, otherwise parse from the str itself
 */
void getDimensions(std::array<int,3> &arr, std::istream &inFile,string str){
    //TODO need to check if the input is a number, might be symbols, idan made a function need to ask
    string line;
    char* input;
    if(str == "byFile"){
        while(getline(inFile,line)) {
            if(!line.empty() && line.at(0) != '#') break; //comment symbol
        }
        input = strtok(line.data(),delim);
        for(int i = 0; i < 3; i++) {
            arr[i] = atoi(input);
            input = strtok(NULL, delim);
        }
    }
    else {
        input = strtok(str.data(),delim);
        for(int i = 0; i < 3; i++) {
            arr[i] = atoi(input);
            input = strtok(NULL, delim);
        }
    }
}


/**
 * This function gets a string(a line from the file) parse the line to get 3 ints (x,y,z) such ship.getMap()[x][y][z]
 * will be initialized to be block container.
 * @param str - the string to parse
 * @param ship - the ship to get it's map from.
 * @param lineNumber - current line number reading from file optional --> algorithm might
 * @return returns empty string iff no error happened
 */
string setBlocksByLine(string &str, Ship* &ship,int lineNumber) {
    auto map = ship->getMap();
    std::ifstream inFile;
    std::array<int,3> dim{};
    getDimensions(dim,inFile,str);
    if(dim[0] > ship->getAxis("x") || dim[1] > ship->getAxis("y") || dim[2] > ship->getAxis("z")){
        return "Error: at line number " + std::to_string(lineNumber) + " One of the provided ship plan constraints exceeding the dimensions of the ship,ignoring";
    }
    else if(!(*map)[dim[0]][dim[1]].empty()){
        return "Error: at line number " + std::to_string(lineNumber) + " constraint at (" +std::to_string(dim[0]) + ","+ std::to_string(dim[1]) +") already given,ignoring...";
    }
    else{
        for(int i = 0; i < ship->getAxis("z")-dim[2]; i++){
            (*map)[dim[0]][dim[1]].emplace_back(Block());
            ship->updateFreeSpace(-1);
        }
    }
    return "";
}

/**
 * This function parse line by line from the file, and initialized the block containers in the shipmap
 * @param ship - the ship object to get it's shipMap and update it
 * @param inFile - the file pointer
 */
void extractArgsForBlocks(Ship* &ship, std::istream &inFile,list<string> &generalErrors){
    string line;
    int lineNumber = 2;
    while (getline(inFile, line)){
        if(!line.empty() && line.at(0) == '#') {
            lineNumber++;
            continue;
        }
        string error = setBlocksByLine(line, ship,lineNumber);
        if(!error.empty())
            generalErrors.emplace_back(error);
        lineNumber++;
    }
}
/**
 * This function parse the ship map and the ship route files and init a new ship object with
 * the information it parsed.
 * @param folder - the folder that contains ship_route, ship_plan files
 * @return the constructed ship iff folder is not empty.
 */
Ship* extractArgsForShip(vector<fs::path> &folder,list<string> &generalErrors) {
    std::ifstream inFile;
    string line, file_path;
    std::array<int, 3> dimensions{};
    vector<Port *> travelRoute;
    Ship* ship = nullptr;
    if(folder.empty()) return nullptr;
    for (int i = 0; i < 2; i++) {
        file_path = folder[i].string();
        inFile.open(file_path);
        if (inFile.fail()) {
            std::cerr << FAIL_TO_READ_PATH + file_path << endl;
            return nullptr;
        } else if (i == 0) {
            getDimensions(dimensions, inFile,"byFile");
            if(dimensions[0] < 0 || dimensions[1] < 0 || dimensions[2] < 0) {
                generalErrors.emplace_back("Error: one of the ship map dimensions is negative");
                return nullptr;
            }
            ship = new Ship(dimensions[0]+1, dimensions[1]+1, dimensions[2]+1);
            extractArgsForBlocks(ship, inFile,generalErrors);
        } else {
            extractTravelRoute(ship, inFile,generalErrors);
        }
        inFile.close();
    }
    ship->initCalc();
    return ship;
}

/**
 * This function parses the data from a port file, it saves it by container id and the data line of this id in a map
 * @param map - the given map to save instruction by port id
 * @param inputPath - the input path of the port file
 */
void parseDataFromPortFile(std::map<string,string>& map, string& inputPath){
    std::ifstream inFile;
    string line;
    inFile.open(inputPath);
    if(inFile.fail()){
        std::cerr << FAIL_TO_READ_PATH + inputPath << endl;
        return;
    }
    while(getline(inFile,line)){
        if(!line.empty() && line.at(0) == '#')continue;
        vector<string> parsedInfo = stringSplit(line,delim);
        if(parsedInfo.size() != 4)continue; /*case not enough information or too much*/
        string contID = parsedInfo.at(0);
        map.insert(make_pair(contID,line));
    }
    inFile.close();
}

//This function saved to usage in exercise 2
/*string* getPortNameFromFile(string filePath){
    string* portName = new string();
    int i = 0,j;
    for(i = filePath.size()-1; i > 0; i--){
        if(filePath.at(i) == '_'){
            j = i;
        }
        if(filePath.at(i) == '\\')
            break;
    }
    portName->append(filePath.substr(filePath.size() - i+1,j-i));
    return portName;
}*/
