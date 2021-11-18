//
// Created by Usuario on 11/14/2021.
//

#include <vector>
#include "../Point/Point.cpp"
#include <algorithm>


using namespace std;

struct Ordinate {
    unsigned int index;
    unsigned int value;
    Ordinate(unsigned int index, unsigned int value): index{index}, value{value} {}
};

struct {
    bool operator()(Ordinate a, Ordinate b) const { return (a.value < b.value);}
} customLess;

template<class Iter>
void merge_sort(Iter first, Iter last, int threads)
{
    if(last - first > 1){
        Iter middle = first + (last - first) / 2;
        if (threads > 1){
#pragma omp parallel sections
            {
#pragma omp section
                {
                    merge_sort(first, middle, threads/2);
                }
#pragma omp section
                {
                    merge_sort(middle, last, threads - threads/2);
                }
            }
            inplace_merge(first, middle, last, customLess);
        } else {
            std::sort(first, last, customLess);
        }

    }
}
