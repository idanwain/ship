cmake_minimum_required(VERSION 3.15)
project(ship3)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG")
set(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(
        ship3
        main.cpp
        Container.cpp               Container.h
        Port.cpp                    Port.h
        Ship.cpp                    Ship.h
        WeightBalanceCalculator.cpp WeightBalanceCalculator.h
        Parser.cpp                  Parser.h
        Common.cpp                  Common.h
                                    AbstractAlgorithm.h
        LifoAlgorithm.cpp           LifoAlgorithm.h
        UnsortedLifoAlgorithm.cpp   UnsortedLifoAlgorithm.h
        SimulatorObj.cpp            SimulatorObj.h
        AlgorithmRegistration.cpp AlgorithmRegistration.h
)