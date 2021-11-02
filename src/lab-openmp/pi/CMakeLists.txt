cmake_minimum_required(VERSION 3.20)
project(pi)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(OpenMP REQUIRED)

add_executable(piseq piseq.cpp)

add_executable(piseqtime piseqtime.cpp)
add_executable(pispar1 pispar1.cpp)
add_executable(pispar2 pispar2.cpp)
add_executable(pispar3 pispar3.cpp)
add_executable(pispar4 pispar4.cpp)
target_link_libraries(pispar1 PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(pispar2 PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(pispar3 PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(pispar4 PUBLIC OpenMP::OpenMP_CXX)
