
#ifndef PARSER_HEADER
#define PARSER_HEADER
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <filesystem>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
namespace fs = std::filesystem;
#define OP_MAIN_DIRECTORY "\\output"

int getPortNumFile(const string& file_name);
bool isValidPortExpressionFile(const string& file_name);
std::vector<std::vector<fs::path>> orderListOfDir(std::list<std::list<fs::path>> &unOrdered);
void initListDirectories(string &path,std::vector<std::vector<fs::path>> &vecOfPaths);
void validateSequenceDirectories(std::vector<std::vector<fs::path>> &direct);
bool isValidPortName(const string& name);
void setActualSize(std::vector<std::vector<fs::path>> &direct);
void createOutputDirectories(std::vector<std::vector<fs::path>> &paths,char* mainDirectory);

#endif // !PARSER_HEADER