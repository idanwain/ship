cmake_minimum_required(VERSION 3.15)
project(ship1)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG -fPIC")
set(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs -shared -ldl -export-dynamic")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(
        ship1
        simulator/main.cpp
        common/Container.cpp                    common/Container.h
        common/Port.cpp                         common/Port.h
        common/Ship.cpp                         common/Ship.h
        common/WeightBalanceCalculator.cpp  interfaces/WeightBalanceCalculator.h
        common/Parser.cpp                       common/Parser.h
        common/Common.cpp                       common/Common.h
        interfaces/ErrorsInterface.h
                                                interfaces/AbstractAlgorithm.h
        algorithm/_313263204_a.cpp              algorithm/_313263204_a.h
        algorithm/_313263204_b.cpp              algorithm/_313263204_b.h
        simulator/SimulatorObj.cpp              simulator/SimulatorObj.h
        simulator/AlgorithmRegistration.cpp     interfaces/AlgorithmRegistration.h
        simulator/AlgorithmFactoryRegistrar.cpp simulator/AlgorithmFactoryRegistrar.h
        simulator/SimulatorValidation.cpp       simulator/SimulatorValidation.h
        simulator/Travel.cpp                    simulator/Travel.h
)