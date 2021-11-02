#include <chrono>
#include <iomanip>
#include <iostream>
#include <omp.h>

int main() {
    using namespace std::chrono;
    using clk = high_resolution_clock;

    auto start = clk::now();

    constexpr long nsteps = 10'000'000;
    double deltax = 1.0 / nsteps;

    int num_threads = 0;
#pragma omp parallel
    num_threads = omp_get_num_threads();
    double sum = 0.0;
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        for (long i = thread_id; i < nsteps; i += num_threads) {
            double x = (static_cast<double>(i) + 0.5) * deltax;
#pragma omp atomic
            sum += 1.0 / (1.0 + x * x);
        }
    };
    double pi_value = 4 * sum * deltax;

    auto stop = clk::now();
    auto diff = duration_cast<microseconds>(stop - start);

    std::cout << "pi_value = " << std::setprecision(10) << pi_value << "\n";
    std::cout << "Time = " << diff.count() << "ms\n";
}
