cmake_minimum_required(VERSION 3.20)
project(p1_AC_2020_21)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(OpenMP REQUIRED)

add_executable(s_test_paos src/test/s_test_paos.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/AosSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Point/Point.cpp src/p2/Simulators/Simulator.hpp)
add_executable(s_test_aos src/test/s_test_aos.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/AosSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Point/Point.cpp src/p1/Simulators/Simulator.hpp)
add_executable(s_test_psoa src/test/s_test_psoa.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/SoaSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Simulators/Simulator.hpp)
add_executable(s_test_soa src/test/s_test_soa.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/SoaSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Simulators/Simulator.hpp)

target_link_libraries(s_test_paos PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(s_test_psoa PUBLIC OpenMP::OpenMP_CXX)

#add_executable(test_paos src/test/test_paos.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/AosSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Point/Point.cpp src/p2/Simulators/Simulator.hpp)
#add_executable(test_aos src/test/test_aos.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/AosSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Point/Point.cpp src/p1/Simulators/Simulator.hpp)
#add_executable(test_psoa src/test/test_psoa.cpp src/p2/SpaceVector/SpaceVector.cpp src/p2/Simulators/SoaSimulator.cpp src/p2/Parsing/Parsing.cpp src/p2/Simulators/Simulator.hpp)
#add_executable(test_soa src/test/test_soa.cpp src/p1/SpaceVector/SpaceVector.cpp src/p1/Simulators/SoaSimulator.cpp src/p1/Parsing/Parsing.cpp src/p1/Simulators/Simulator.hpp)
#
#
#target_link_libraries(test_paos PUBLIC OpenMP::OpenMP_CXX)
#target_link_libraries(test_psoa PUBLIC OpenMP::OpenMP_CXX)