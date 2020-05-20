/**
* This header is a module of the functions used by the simulator to validate the algortihms instructions
*
*      *******      Functions      *******
* validateAlgorithm                 - main function to validate algorithm crane instructions
* validateInstruction               - managing the instructions validation
* validateLoadInstruction           - validates the load instruction
* validateUnloadInstruction         - validates the unload instruction
* validateMoveInstruction           - validates the move instruction
* validateRejectInstruction         - validates reject instruction
* validateContainerDataForReject    - validate container data for reject and assign to reject map
* initPriorityRejected              - init the priority reject and possible priorirty reject maps
* initLoadedListAndRejected         - init the future container load list and must rejected map
* eraseFromRawData                  - erasing from raw data given id
* finalChecks                       - managing the final checks
* checkIfBalanceWeightIssue         - checks if there's weight balance issue
* checkForContainersNotUnloaded     - checks if there's a container that didn't unloaded
* checkContainersDidntHandle        - checks if there are containers at cargo_data that alg didn't handle at all
* checkIfContainersLeftOnPort       - checks if there were container left on port
* checkIfContainerLeftOnShipFinalPort   - checks if there were containers left on ship final port
* checkPrioritizedHandledProperly       - checks if algorithm prioritized correctly
* extractKgToValidate                   - extracts the kg to validate
* extractPortNameToValidate             - extracts the port name to validate
* execute                               - executes the L/M/U instructions on simulator ship map
* softCheckId                           - soft checks id's that possibly didn't handled by algorithm
* isIdAwaitAtPort                       - checks if the given id await at port
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
    int checkPrioritizedHandledProperly(list<string> &currAlgErrors);
    static bool checkIfBalanceWeightIssue(SimulatorObj* sim, int kg,std::tuple<int,int,int>& coordinates);
    static int checkForContainersNotUnloaded(SimulatorObj* sim, list<string> &currAlgErrors);
    static int checkContainersDidntHandle(map<string, list<string>> &idAndRawLine,list<string> &currAlgErrors,string& portName, int visitNum);
    static int checkIfContainersLeftOnPort(SimulatorObj* sim , list<string> &currAlgErrors);
    static int checkIfContainerLeftOnShipFinalPort(SimulatorObj* sim,list<string> &currAlgErrors);
    static int extractKgToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);
    static string extractPortNameToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);
    static void validateContainerDataForReject(string& line,VALIDATION &reason,SimulatorObj* sim);
    static bool softCheckId(string id);
    void execute(char command,std::unique_ptr<Container>& container, coordinate origin, coordinate dest);
    bool isIdAwaitAtPort(string &id);
};



#endif
