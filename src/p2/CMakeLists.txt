cmake_minimum_required(VERSION 3.20)
project(p1_AC_2020_21)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(OpenMP REQUIRED)

add_executable(paos paos.cpp SpaceVector/SpaceVector.cpp Simulators/AosSimulator.cpp Parsing/Parsing.cpp Point/Point.cpp Simulators/Simulator.hpp)
add_executable(psoa psoa.cpp SpaceVector/SpaceVector.cpp Simulators/SoaSimulator.cpp Parsing/Parsing.cpp Simulators/Simulator.hpp)


target_link_libraries(paos PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(psoa PUBLIC OpenMP::OpenMP_CXX)