//
// Created by Usuario on 11/14/2021.
//

#include "vector"
#include "../Point/Point.cpp"

using namespace std;

static vector<Point> merge(const vector<Point>& left, const vector<Point>& right)
{
    vector<Point> result;
    unsigned left_it = 0, right_it = 0;

    while(left_it < left.size() && right_it < right.size())
    {
        if(left[left_it].pos.x < right[right_it].pos.x || right[right_it].order == -1)
        {
            result.push_back(left[left_it]);
            left_it++;
        }
        else
        {
            result.push_back(right[right_it]);
            right_it++;
        }
    }

    // Push the remaining data from both vectors onto the resultant
    while(left_it < left.size())
    {
        result.push_back(left[left_it]);
        left_it++;
    }

    while(right_it < right.size())
    {
        result.push_back(right[right_it]);
        right_it++;
    }

    return result;
}

static vector<Point> mergesort(vector<Point>& vec, int threads)
{
    // Termination condition: List is completely sorted if it
    // only contains a single element.
    if(vec.size() == 1)
    {
        return vec;
    }

    // Determine the location of the middle element in the vector
    auto middle = vec.begin() + (vec.size() / 2);

    vector<Point> left(vec.begin(), middle);
    vector<Point> right(middle, vec.end());

    // Perform a merge sort on the two smaller vectors

    if (threads > 1)
    {
#pragma omp parallel sections
        {
#pragma omp section
            {
                left = mergesort(left, threads/2);
            }
#pragma omp section
            {
                right = mergesort(right, threads - threads/2);
            }
        }
    }
    else
    {
        left = mergesort(left, 1);
        right = mergesort(right, 1);
    }

    return merge(left, right);
}
