cmake_minimum_required(VERSION 3.20)
project(p1_AC_2020_21)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(OpenMP REQUIRED)

add_executable(aos src/p1/aos.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/AosSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Point/Point.cpp src/p1/Simulators/Simulator.hpp)
add_executable(soa src/p1/soa.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/SoaSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Simulators/Simulator.hpp)

add_executable(paos src/p2/paos.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/AosSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Point/Point.cpp src/p2/Simulators/Simulator.hpp)
add_executable(psoa src/p2/psoa.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/SoaSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Simulators/Simulator.hpp)
target_link_libraries(paos PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(psoa PUBLIC OpenMP::OpenMP_CXX)