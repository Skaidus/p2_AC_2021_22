//
// Created by Usuario on 10/13/2021.
//

#include <chrono>
#include "../p2/Parsing/Parsing.cpp"
#include "../p2/Simulators/AosSimulator.cpp"

using namespace std;
constexpr int N = 30;

int main() {
//    vector<int> num_objects = {4000};
//    vector<int> num_iterations = {250};
    vector<int> num_objects = {4000, 8000};
    vector<int> num_iterations = {250, 500};
    int random_seed = 1;
    //vector<int> num_threads = {8};
    vector<int> num_threads = {1, 2, 4, 8, 16};
    double time_step = 0.2;
    for (auto t: num_threads) {
        for (auto iter: num_iterations) {
            for (auto obj: num_objects) {
                std::vector<double> v;
                for (auto i = 0; i < N; i++) {
                    auto aos = AosSimulator(obj, random_seed + 100 * i, obj * 10, time_step);
                    auto t1 = std::chrono::steady_clock::now();
                    //cout << aos << '\n';
                    aos.run(iter, t);
                    auto t2 = std::chrono::steady_clock::now();
                    //cout << aos << '\n';
                    v.emplace_back(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
                }
                double sum = std::accumulate(std::begin(v), std::end(v), 0.0);
                double m1 = sum / N;
                double m2 = sum / (N * iter);
//                double accum1 = 0.0;
//                double accum2 = 0.0;
//                std::for_each (std::begin(v), std::end(v), [&](const double d) {
//                    accum1 += (d - m1) * (d - m1);
//                });
//                std::for_each (std::begin(v), std::end(v), [&](const double d) {
//                    accum2 += (d/iter - m1) * (d/iter - m1);
//                });
//                double stdev1 = sqrt(accum1 / (N-1));
//                double stdev2 = sqrt(accum2 / (N-1));
//                auto conf1 = 1.960 * (stdev1/ sqrt(N));
//                auto conf2 = 1.960 * (stdev2/ sqrt(N));
                // Report results
                std::cout << "num_objs: " << obj << ", num_iter: " << iter << ", num_threads: " << t << '\n';
                std::cout << m2 << "  ns/iter\n";
                std::cout << m1 << "  ns\n";
            }
        }
    }
}