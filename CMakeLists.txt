cmake_minimum_required(VERSION 3.15)
project(ship1)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG")
set(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(
        ship1
        main.cpp
        Common/Container.cpp                            Common/Container.h
        Common/Port.cpp                                 Common/Port.h
        Common/Ship.cpp                                 Common/Ship.h
        Interface/WeightBalanceCalculator.cpp           Interface/WeightBalanceCalculator.h
        Common/Parser.cpp                               Common/Parser.h
        Common/Common.cpp                               Common/Common.h
                                                        Interface/ErrorsInterface.h
                                                        ../Interface/AbstractAlgorithm.h
        Algorithm/LifoAlgorithm.cpp                     Algorithm/LifoAlgorithm.h
        Algorithm/UnsortedLifoAlgorithm.cpp             Algorithm/UnsortedLifoAlgorithm.h
        Simulator/SimulatorObj.cpp                      Simulator/SimulatorObj.h
        Interface/AlgorithmRegistration.cpp             Interface/AlgorithmRegistration.h
        Simulator/AlgorithmFactoryRegistrar.cpp         Simulator/AlgorithmFactoryRegistrar.h
)