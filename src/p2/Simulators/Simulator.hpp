
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

    virtual void run(int iterations, int numthreads) = 0;
};

#endif //P1_AC_2020_21_SIMULATOR_HPP
