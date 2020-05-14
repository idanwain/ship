/**
* This header is a module of the functions used by the simulator to validate the algortihms instructions
*
*      *******      Functions      *******
 */
#ifndef SIMULATORVALIDATION_H
#define SIMULATORVALIDATION_H

enum class VALIDATION;
#include "SimulatorObj.h"


std::optional<pair<int,int>> validateAlgorithm(string &outputPath, string &contAtPortPath,int portNumber, list<string>& currAlgErrors,SimulatorObj* simulator,string& portName,int visitNumber);
bool validateInstruction(string &instruction,string &id,vector<int> &coordinates,SimulatorObj* sim,std::map<string,list<string>> &portContainers,int portNum);
bool validateLoadInstruction(vector<int> &coordinates,SimulatorObj* sim,int kg);
bool validateUnloadInstruction(vector<int> &coordinates,SimulatorObj* sim);
bool validateMoveInstruction(vector<int> &coordinates, SimulatorObj* sim);
bool validateRejectInstruction(std::map<string,list<string>>& portContainers, string& id,SimulatorObj* sim,int portNum,int kg);
bool validateContainerData(const string& line, VALIDATION& reason, string& id, std::unique_ptr<Ship>& ship);

bool checkIfBalanceWeightIssue(SimulatorObj* sim, int kg,std::tuple<int,int,int>& coordinates);
int extractKgToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);
string extractPortNameToValidate(map<string,list<string>>& rawData,SimulatorObj* sim,string& id);



#endif
