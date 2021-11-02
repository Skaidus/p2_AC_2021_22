//
// Created by Usuario on 10/13/2021.
//

#include "Parsing/Parsing.cpp"
#include "./Simulators/SoaSimulator.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    // Abriendo archivos
    fstream init, final;
    init.open("final_config.txt", ios::out);
    final.open("init_config.txt", ios::out);
    // Parse args
    Arguments args{};
    int err;
    // Exit if error
    if ((err = convertInput(argc, argv, args)) < 0) return err;
    // SOA experiment
    auto soa = SoaSimulator(args.num_objects, args.random_seed, args.size_enclosure, args.time_step);
    init << soa << '\n';
    soa.run(args.num_iterations);
    final << soa << '\n';
    return 0;
}
