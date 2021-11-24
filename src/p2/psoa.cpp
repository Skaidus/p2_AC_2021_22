//
// Created by Usuario on 10/13/2021.
//


#include <chrono>
#include "Parsing/Parsing.cpp"
#include "Simulators/SoaSimulator.cpp"

using namespace std;
constexpr int N = 30;

int main(int argc, char **argv) {
    // Parse args
    Arguments args{};
    int err;
    // Exit if error
    if ((err = convertInput(argc, argv, args)) < 0) return err;
    // AOS experiment
    std::vector<double> v;
    for (auto i = 0; i < N; i++) {
        auto aos = SoaSimulator(args.num_objects, args.random_seed+ 100 * i, args.size_enclosure, args.time_step);
        auto t1 = std::chrono::steady_clock::now();
        //cout << aos << '\n';
        aos.run(args.num_iterations, 8);
        auto t2 = std::chrono::steady_clock::now();
        //cout << aos << '\n';
        v.emplace_back(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
    }
    double sum = std::accumulate(std::begin(v), std::end(v), 0.0);
    double m1 =  sum / N;
    double m2 =  sum / (N * args.num_iterations);
    double accum1 = 0.0;
    double accum2 = 0.0;
    std::for_each (std::begin(v), std::end(v), [&](const double d) {
        accum1 += (d - m1) * (d - m1);
    });
    std::for_each (std::begin(v), std::end(v), [&](const double d) {
        accum2 += (d/args.num_iterations - m1) * (d/args.num_iterations - m1);
    });
    double stdev1 = sqrt(accum1 / (N-1));
    double stdev2 = sqrt(accum2 / (N-1));
    auto conf1 = 1.960 * (stdev1/ sqrt(N));
    auto conf2 = 1.960 * (stdev2/ sqrt(N));
    // Report results
    std::cout  << m2  << " ± " << conf2 <<"ns/iter\n";
    std::cout  << m1  << " ± " << conf1 <<"ns\n";
    return 0;
}


/*
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
*/