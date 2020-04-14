cmake_minimum_required(VERSION 3.15)
project(shipTest)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG")
set(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

include_directories(.)

add_executable(shipTest
        container.cpp
        container.h
        port.cpp
        port.h
        ship.cpp
        ship.h
        simulator.cpp
        stowage_algorithm.cpp lifo_algorithm.cpp lifo_algorithm.h stowage_algorithm.h Parser.cpp Parser.h Error.cpp Error.h WeightBalanceCalculator.cpp WeightBalanceCalculator.h OutPutHandler.cpp OutPutHandler.h common.cpp common.h)
