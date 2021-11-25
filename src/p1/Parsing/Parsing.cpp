//
// Created by Usuario on 10/11/2021.
//

//std
#include <iostream>
#include <string>

// Contains required args to initialize a simulator
struct Arguments {
    int num_objects;
    int num_iterations;
    int random_seed;
    double size_enclosure;
    double time_step;
};

using namespace std;

// Maps invalid arguments to '?'
inline auto arg_string(int index, int argc, char **argv) {
    return index < argc ? argv[index] : "?";
}

// Maps argv to the sample assignment output
inline auto print_arguments(int argc, char **argv) {
    string out;
    const string parameters[5] = {"num_objects", "num_iterations", "random_seed", "size_enclosure", "time_step"};
    for (int i = 1; i < 6; i++) {
        out += '\t' + parameters[i - 1] + ": " + arg_string(i, argc, argv) + '\n';
    }
    return out;
}

inline int convertInput(int argc, char **argv, Arguments &args) {
    if (argc != 6) {
        cerr << argv[0] << " invoked with " << argc - 1 << " parameters.\nArguments:\n" << print_arguments(argc, argv);
        return -1;
    }

    try {
        try {
            args.num_objects = stoi(argv[1]);
        } catch (const exception &e) {
            throw invalid_argument("Number of objects cannot be converted to integer");
        }
        if (args.num_objects <= 0) throw invalid_argument("Number of objects must be greater than zero");

        try {
            args.num_iterations = stoi(argv[2]);
        } catch (const exception &e) {
            throw invalid_argument("Number of iteration cannot be converted to integer");
        }
        if (args.num_iterations <= 0) throw invalid_argument("Number of iterations must be greater than zero");

        try {
            args.random_seed = stoi(argv[3]);
        } catch (const exception &e) {
            throw invalid_argument("Random seed cannot be converted to integer");
        }
        if (args.random_seed <= 0) throw invalid_argument("Random seed must be greater than zero");

        try {
            args.size_enclosure = stod(argv[4]);
        } catch (const exception &e) {
            throw invalid_argument("Size enclosure cannot be converted to double");
        }
        if (args.size_enclosure <= 0) throw invalid_argument("Size enclosure must be greater than zero");

        try {
            args.time_step = stod(argv[5]);
        } catch (const exception &e) {
            throw invalid_argument("Time step cannot be converted to double");
        }
        if (args.time_step <= 0) throw invalid_argument("Time step must be greater than zero");

    } catch (const exception &e) {
        cerr << e.what() << "\nArguments:\n" << print_arguments(argc, argv);
        return -2;
    }
    cout << "Creating simulation:" << '\n' << print_arguments(argc, argv);
    return 0;
}