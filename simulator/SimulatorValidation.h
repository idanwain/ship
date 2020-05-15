/**
* This header is a module of the functions used by the simulator to validate the algortihms instructions
*
*      *******      Functions      *******
 */
#ifndef SIMULATORVALIDATION_H
#define SIMULATORVALIDATION_H

enum class VALIDATION;
#include "SimulatorObj.h"

class SimulatorValidation{
    map<string,list<string>> rawDataFromPortFile;
    map<string,map<string,VALIDATION>> mustRejected;
    map<string,Container> priorityRejected;
    map<string,Container> possiblePriorityReject;
    SimulatorObj* sim = nullptr;
    int loadCapacity = 0;
public:
    explicit SimulatorValidation(SimulatorObj* simToCopy):sim(simToCopy){};
    std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &contAtPortPath,list<string>& currAlgErrors,string& portName,int visitNumber);
    bool validateInstruction(string &instruction,string &id,vector<int> &coordinates);
    bool validateLoadInstruction(vector<int> &coordinates,int kg,string& id);
    bool validateUnloadInstruction(vector<int> &coordinates);
    bool validateMoveInstruction(vector<int> &coordinates);
    bool validateRejectInstruction(string& id,int kg);
    void initPriorityRejected();
    void initLoadedListAndRejected();
    void eraseFromRawData(string& line,string& id);
    int finalChecks(list<string> &currAlgErrors,string& portName, int visitNumber);
    static bool checkIfBalanceWeightIssue(SimulatorObj* sim, int kg,std::tuple<int,int,int>& coordinates);
    static int checkForContainersNotUnloaded(SimulatorObj* sim, list<string> &currAlgErrors);
    static int checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string& portName, int visitNum);
    static int checkIfContainersLeftOnPort(SimulatorObj* sim , list<string> &currAlgErrors);
    int checkPrioritizedHandledProperly(list<string> &currAlgErrors);
    static int extractKgToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);
    static string extractPortNameToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);
    static void validateContainerDataForReject(string& line,VALIDATION &reason,SimulatorObj* sim);
    void execute(std::unique_ptr<Ship>& ship, char command,std::unique_ptr<Container>& container, coordinate origin, coordinate dest, const std::shared_ptr<Port>& port);

};



#endif
