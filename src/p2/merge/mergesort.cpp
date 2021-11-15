//
// Created by Usuario on 11/14/2021.
//

#include <vector>
#include "../Point/Point.cpp"
#include <algorithm>

using namespace std;

struct {
    bool operator()(Point a, Point b) const { return (b.order==-1)||(a.pos.x < b.pos.x);}
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
        } else {
            merge_sort(first, middle, 1);
            merge_sort(middle, last, 1);
        }
        inplace_merge(first, middle, last, customLess);
    }
}
