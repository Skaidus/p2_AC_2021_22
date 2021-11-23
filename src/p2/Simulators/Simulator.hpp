//
// Created by Usuario on 10/12/2021.
//

#ifndef P1_AC_2020_21_SIMULATOR_HPP
#define P1_AC_2020_21_SIMULATOR_HPP
#include <vector>
#include <random>
class Simulator {
protected:
    virtual void checkCollisions() = 0;

public:
    int objs{};
    double dt{}, size{};

    virtual void run(int iterations) = 0;
};

#endif //P1_AC_2020_21_SIMULATOR_HPP
