#include <iostream>
#include <omp.h>

int main() {
    std::cout<<omp_get_num_threads()<<'\n';
#pragma omp parallel
  {
      std::cout<<omp_get_num_threads()<<'\n';
    //int id = omp_get_thread_num();
    //std::cout << "Hello(" << id << ") ";
    //std::cout << "world(" << id << ")\n";
  }
    std::cout<<omp_get_num_threads()<<'\n';
}