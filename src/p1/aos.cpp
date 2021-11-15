//
// Created by Usuario on 10/13/2021.
//

// omp_schedule.cpp
// compile with: /openmp
#include <windows.h>
#include <stdio.h>
#include <omp.h>


int main( )
{
    int NUM_THREADS = 8;
    const int NUM_LOOPS = 64;
    int SLEEP_EVERY_N = 3;
    int nStatic1[NUM_LOOPS];
    int nDynamic1[NUM_LOOPS];
    int nGuided[NUM_LOOPS];

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
#pragma omp for schedule(static) collide
        for (int i = 0 ; i < NUM_LOOPS ; ++i)
        {
            for(int j = 0)
            if ((i % SLEEP_EVERY_N) == 0)
                Sleep(0);
            nStatic1[i] = omp_get_thread_num( );
        }

#pragma omp for schedule(dynamic)
        for (int i = 0 ; i < NUM_LOOPS ; ++i)
        {
            if ((i % SLEEP_EVERY_N) == 0)
                Sleep(0);
            nDynamic1[i] = omp_get_thread_num( );
        }



#pragma omp for schedule(guided)
        for (int i = 0 ; i < NUM_LOOPS ; ++i)
        {
            if ((i % SLEEP_EVERY_N) == 0)
                Sleep(0);
            nGuided[i] = omp_get_thread_num( );
        }
    }

    printf_s("-----------------------------\n");
    printf_s("| static | dynamic | guided |\n");
    printf_s("-----------------------------\n");

    for (int i=0; i<NUM_LOOPS; ++i)
    {
        printf_s("|    %d   |    %d   |    %d    |\n",
                 nStatic1[i],
                 nDynamic1[i], nGuided[i]);
    }

    printf_s("-----------------------------\n");
}

//#include <chrono>
//#include "Parsing/Parsing.cpp"
//#include "Simulators/AosSimulator.cpp"
//
//using namespace std;
//constexpr int N = 30;
//long long total1, total2 = 0;
//
//int main(int argc, char **argv) {
//    // Parse args
//    Arguments args{};
//    int err;
//    // Exit if error
//    if ((err = convertInput(argc, argv, args)) < 0) return err;
//    // AOS experiment
//    for (auto i = 0; i < N; i++) {
//        auto aos = AosSimulator(args.num_objects, args.random_seed+ 100 * i, args.size_enclosure, args.time_step);
//        auto t1 = std::chrono::steady_clock::now();
//        //cout << aos << '\n';
//        aos.run(args.num_iterations);
//        auto t2 = std::chrono::steady_clock::now();
//        //cout << aos << '\n';
//        total1 += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
//    }
//
//    // Report results
//    std::cout << (double) total1 / (double) (N * args.num_iterations) << " ns/iter\n";
//    std::cout  << (double) total1 / (double) N  << " ns\n";
//
//    return 0;
//}



/*
#include "Parsing/Parsing.cpp"
#include "./Simulators/AosSimulator.cpp"
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
    auto soa = AosSimulator(args.num_objects, args.random_seed, args.size_enclosure, args.time_step);
    init << soa << '\n';
    soa.run(args.num_iterations);
    final << soa << '\n';
    return 0;
}
*/
