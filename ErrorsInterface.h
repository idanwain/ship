/**
 * This header is a container of all Errors, and pre defined print messages of the program
 */
/*----------------------Globals-------------------*/
#define toString(x)             std::to_string(x)
#define CLANG_TIDY              "FOR CLANG TIDY ERRORS, DO NOT RECIEVE THIS VALUE JUST USE AT AS IS"
#define ERROR_NOTRAVELPATH      std::cerr << "Error: No travel path recieved, exiting program..." << endl;

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



/*----------------------Validation Errors-------------------*/
#define ERROR_ContLine(x,y)     "Error: container id: " + x + ", instruction: " + y

/*----------------------Simulator Errors-------------------*/
#define ERROR_RESULTSFILE       std::cerr << "Error: failed to create results file" << std::endl;
#define ERROR_ERRORSFILE        std::cerr << "Error: failed to create errors file" << std::endl;
#define ERROR_NOTDIRECTORY(x,y) std::cerr << "Error: "  << x.path().filename().string()  << y << std::endl;

