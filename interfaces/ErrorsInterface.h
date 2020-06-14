/**
 * This header is a container of all Errors, and pre defined print messages of the program
 */
/*----------------------Globals-------------------*/
#define toString(x)             std::to_string(x)
#define CLANG_TIDY              "FOR CLANG TIDY ERRORS, DO NOT RECEIVE THIS VALUE JUST USE AT AS IS"

/*----------------------ERRORS IN PLAN FILE-------------------*/
#define ERROR_XYZ_DIM(x)                    "ship plan: at line number " + toString(x) + " one of the provided ship plan constraints exceeding the dimensions of the ship"
#define ERROR_BAD_LINE(x)                   "ship plan: at line number " + toString(x) + " bad line format"
#define ERROR_DIFF_VALUE(x,y,z)             "ship plan: at line number " + toString(x) + " constraint at (" + toString(y) + "," + toString(z) +") already given with diff value,ignoring travel"
#define ERROR_SAME_VALUE(x,y,z)             "ship plan: at line number " + toString(x) + " constraint at (" + toString(y) + "," + toString(z) +") already given with same value"
#define ERROR_LACK_PLAN                     "ship plan: Lack of plan file, ignoring this travel"
#define ERROR_FATA_LPLAN                    "ship plan: Fatal error occurred, ignoring this travel"
#define ERROR_PLAN_MANY_FILES(x)            "ship plan: There is already plan file, ignoring " + x

/*----------------------ERRORS IN ROUTE FILE-------------------*/
#define ERROR_LACK_ROUTE                    "route: Lack of route file, ignoring this travel"
#define ERROR_FATAL_ROUTE                   "route: Fatal error occurred, ignoring this travel"
#define ERROR_NOT_ENOUGH_PORTS              "route: File contains less then 2 valid ports, ignoring this travel"
#define ERROR_PORT_TWICE(x)                 "route: Port " + x + " occurs 2 or more consecutive times"
#define ERROR_NO_PORT_EXIST_IN_TRAVEL(x)    "route: Port " + x + " has cargo_data file but not in route, ignoring his cargo files"
#define ERROR_ROUTE_MANY_FILES(x)           "route: There is already plan file, ignoring " + x
#define ERROR_TOO_MANY_CARGO_FILES          "route: Number of cargo_data files > route length"


/*----------------------PRINT TO CONSOLE-------------------*/
#define ERROR_READ_PATH(x)                      std::cerr << "Failed to read from this file path " << x << std::endl;
#define NO_RESULT_FILE                          std::cout << "No results found, simulation.results file won't be created" << std::endl;
#define NO_ERROR_FILE                           std::cout << "No errors found, simulation.errors file won't be created" << std::endl;
#define NO_TRAVEL_PATH                          std::cerr << "Error: failed to read travel path, exiting program..." << endl;
#define ERROR_ERRORS_FILE                       std::cerr << "Error: failed to create errors file" << std::endl;
#define ERROR_RESULTS_FILE                      std::cerr << "Error: failed to create results file" << std::endl;


/*----------------------Validation Errors-------------------*/
#define ERROR_CONT_LINE_INSTRUCTION(x,y,z,w)    "Error: algorithm instructed invalid instruction at port " + x + " visit number " + toString(z) + " instruction: " + w + " container id " + y
#define ERROR_PLAN_FATAL                        "Error: algorithm indicated there's a fatal error in plan file --> simulator rejects"
#define ERROR_DUPLICATE_XY                      "Error: algorithm indicated there's a to (x,y) constraints with different Z value --> simulator rejects"
#define ERROR_TRAVEL_FATAL                      "Error: algorithm indicates there's a fatal error in route file --> simulator rejects"
#define ERROR_TRAVEL_SINGLEPORT                 "Error: algorithm indicates there's a 0 or 1 ports in route file --> simulator rejects"
#define ERROR_NO_CARGO_TOLOAD(x,y)              "Error: algorithm indicates there's no cargo to load at port " + x + " visit number " + toString(y) + " --> simulator rejects"
#define ERROR_LINE_NOT_HANDLE(x,y,z)            "Error: algorithm didn't handled this cargo_data line: " + x +" at port " + y + " visit number " + toString(z)
#define ERROR_CONT_NOT_INDEST(x,y,z)            "Error: container " + x + " was on ship , but unloaded at port " + y + " while it's destination " + z
#define ERROR_CONT_LEFT_ONSHIP(x)               "Error: container " + x + " left on ship, after stopping at it's destination port"
#define ERROR_CONT_LEFT_ONPORT(x,y,z)           "Error: container " + x + " left at port " + y + " while ship has space"
#define ERROR_LEFT_PRIORITY_ONPORT              "Error: There is at least 1 prioritized container that left on port but supposed to be loaded"
#define ERROR_CONT_LEFT_LAST_PORT(x)            "Error: container " + x + " left on ship at the end of the travel"
#define ERROR_INSTRUCTIONS_FILE(x)               "Error: simulator couldn't read all together instructions file at : " + x


/*----------------------Simulator Errors-------------------*/
#define ERROR_NOT_DIRECTORY(x,y)                "Error: " + x.path().filename().string() + y
#define ERROR_INVALID_FILE(x)                   "Error: " + x + " is not a valid route/port/ship plan file, ignoring"
#define ERROR_ALG_NOT_REG(x)                    "Error: algorithm: " + x + " didn't register, ignoring this algorithm"
#define ERROR_ALG_PLAN                          "Error: Algorithm crashed in read plan/route or calculator initialize"
#define ERROR_ALG_FAILED                        "Error: Algorithm Failed in get instructions for cargo"



