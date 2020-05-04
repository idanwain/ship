/**
 * This header is a container of all Errors, and pre defined print messages of the program
 */
/*----------------------Globals-------------------*/
#define toString(x)             std::to_string(x)
#define CLANG_TIDY              "FOR CLANG TIDY ERRORS, DO NOT RECEIVE THIS VALUE JUST USE AT AS IS"

/*----------------------ERRORS IN PLAN FILE-------------------*/
#define ERROR_XYZ_DIM(x)        "ship plan: at line number " + toString(x) + " One of the provided ship plan constraints exceeding the dimensions of the ship"
#define ERROR_BADLINE(x)        "ship plan: at line number " + toString(x) + "bad line format"
#define ERROR_DIFFVALUE(x,y,z)  "ship plan: at line number " + toString(x) + " constraint at (" + toString(y) + "," + toString(z) +") already given with diff value,ignoring travel"
#define ERROR_SAMEVALUE(x,y,z)  "ship plan: at line number " + toString(x) + " constraint at (" + toString(y) + "," + toString(z) +") already given with same value"
#define ERROR_LACKPLAN          "ship plan: Lack of plan file, ignoring this travel"
#define ERROR_FATALPLAN         "ship plan: Fatal error occurred, ignoring this travel"


/*----------------------ERRORS IN ROUTE FILE-------------------*/
#define ERROR_LACKROUTE         "route: Lack of route file, ignoring this travel"
#define ERROR_FATALROUTE        "route: Fatal error occurred, ignoring this travel"
#define ERROR_NOTENOUGHPORTS    "route: File contains less then 2 valid ports, ignoring this travel"
#define ERROR_PORTTWICE(x)      "route: Port " + x + " occurs 2 or more consecutive times"


/*----------------------PRINT TO CONSOLE-------------------*/
#define P_ERROR_READPATH(x)     std::cerr << "Failed to read from this file path " << x << std::endl
#define P_NORESULTFILE          std::cout << "No results found, simulation.results file won't be created" << std::endl;
#define P_NOERRORFILE           std::cout << "No errors found, simulation.errors file won't be created" << std::endl;



/*----------------------Validation Errors-------------------*/
#define ERROR_CONTLINEINSTRUCTION(x,y,z)        "Error: Algorithm instructed not a valid instruction at port "+ x + "visit number " + toString(y) + " instruction: " + z
#define PLAN_FATAL                              "Error: Algorithm indicated there's a fatal error in plan file --> simulator rejects"
#define DUPLICATE_XY                            "Error: Algorithm indicated there's a to (x,y) constraints with different Z value --> simulator rejects"
#define TRAVEL_FATAL                            "Error: Algorithm indicates there's a fatal error in route file --> simulator rejects"
#define TRAVEL_SINGLEPORT                       "Error: Algorithm indicates there's a 0 or 1 ports in route file --> simulator rejects"
#define NO_CARGO_TOLOAD(x,y)                    "Error: Algorithm indicates there's no cargo to load at port " + x + " visit number " + toString(y) + " --> simulator rejects"
#define ERROR_IDNOTHANDLE(x,y,z)                "Error: Algorithm didn't handled this cargo_data line: " + x +" at port " + y + " visit number " + toString(z)

/*----------------------Simulator Errors-------------------*/
#define ERROR_RESULTSFILE       std::cerr << "Error: failed to create results file" << std::endl;
#define ERROR_ERRORSFILE        std::cerr << "Error: failed to create errors file" << std::endl;
#define ERROR_NOTDIRECTORY(x,y) std::cerr << "Error: "  << x.path().filename().string()  << y << std::endl;
#define ERROR_NOTRAVELPATH      std::cerr << "Error: No travel path received, exiting program..." << endl;


