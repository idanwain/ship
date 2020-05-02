cmake_minimum_required(VERSION 3.15)
project(ship3)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG")
set(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(ship3
        container.cpp
        container.h
        port.cpp
        port.h
        ship.cpp
        ship.h
        main.cpp
        WeightBalanceCalculator.cpp WeightBalanceCalculator.h
        Parser.cpp Parser.h
        outputHandler.cpp outputHandler.h
        common.cpp common.h
        AbstractAlgorithm.h
        lifo_algorithm.cpp lifo_algorithm.h
        SimulatorObj.cpp SimulatorObj.h)