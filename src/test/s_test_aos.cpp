//
// Created by Usuario on 10/13/2021.
//

#include "../p1/Parsing/Parsing.cpp"
#include "../p1/Simulators/AosSimulator.cpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    // Parse args
    Arguments args{};
    int err;
    // Exit if error
    if ((err = convertInput(argc, argv, args)) < 0) return err;
    // SOA experiment
    auto soa = AosSimulator(args.num_objects, args.random_seed, args.size_enclosure, args.time_step);
    soa.run(args.num_iterations);
    cout << soa << '\n';
    return 0;
}
