cmake_minimum_required(VERSION 3.15)
project(ship1)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(ship1
        common.cpp
        container.cpp
        container.h
        port.cpp
        port.h
        ship.cpp
        ship.h
        simulator.cpp
        stowage_algorithm.cpp)
