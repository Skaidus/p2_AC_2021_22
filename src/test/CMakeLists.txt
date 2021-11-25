cmake_minimum_required(VERSION 3.20)
project(p1_AC_2020_21)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(OpenMP REQUIRED)

add_executable(s_test_paos s_test_paos.cpp ../p2/SpaceVector/SpaceVector.cpp ../p2/Simulators/AosSimulator.cpp ../p2/Parsing/Parsing.cpp ../p2/Point/Point.cpp ../p2/Simulators/Simulator.hpp)
add_executable(s_test_aos s_test_aos.cpp ../p1/SpaceVector/SpaceVector.cpp ../p1/Simulators/AosSimulator.cpp ../p1/Parsing/Parsing.cpp ../p1/Point/Point.cpp ../p1/Simulators/Simulator.hpp)
add_executable(s_test_psoa s_test_psoa.cpp ../p2/SpaceVector/SpaceVector.cpp ../p2/Simulators/SoaSimulator.cpp ../p2/Parsing/Parsing.cpp ../p2/Simulators/Simulator.hpp)
add_executable(s_test_soa s_test_soa.cpp ../p1/SpaceVector/SpaceVector.cpp ../p1/Simulators/SoaSimulator.cpp ../p1/Parsing/Parsing.cpp ../p1/Simulators/Simulator.hpp)

target_link_libraries(s_test_paos PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(s_test_psoa PUBLIC OpenMP::OpenMP_CXX)

add_executable(test_paos test_paos.cpp ../p2/SpaceVector/SpaceVector.cpp ../p2/Simulators/AosSimulator.cpp ../p2/Parsing/Parsing.cpp ../p2/Point/Point.cpp ../p2/Simulators/Simulator.hpp)
add_executable(test_aos test_aos.cpp ../p1/SpaceVector/SpaceVector.cpp ../p1/Simulators/AosSimulator.cpp ../p1/Parsing/Parsing.cpp ../p1/Point/Point.cpp ../p1/Simulators/Simulator.hpp)
add_executable(test_psoa test_psoa.cpp ../p2/SpaceVector/SpaceVector.cpp ../p2/Simulators/SoaSimulator.cpp ../p2/Parsing/Parsing.cpp ../p2/Simulators/Simulator.hpp)
add_executable(test_soa test_soa.cpp ../p1/SpaceVector/SpaceVector.cpp ../p1/Simulators/SoaSimulator.cpp ../p1/Parsing/Parsing.cpp ../p1/Simulators/Simulator.hpp)


target_link_libraries(test_paos PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_psoa PUBLIC OpenMP::OpenMP_CXX)