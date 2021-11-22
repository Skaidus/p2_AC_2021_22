
//
// Created by Usuario on 10/10/2021.
//

//imports
#include "Simulator.hpp"
#include "../merge/mergesort.cpp"
#include <omp.h>
#include <cmath>

//#define num_threads 6

using namespace std;

template <typename T>
std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B)
{
    std::vector<T> AB;
    AB.reserve(A.size() + B.size());                // preallocate memory
    AB.insert(AB.end(), A.begin(), A.end());        // add A;
    AB.insert(AB.end(), B.begin(), B.end());        // add B;
    return AB;
}


class AosSimulator : public Simulator {
private:

    void checkCollisions() override {
        for (unsigned int i = 0; i < objs; i++) {
            bool changed = false;
            for (unsigned int  j = i + 1; j < objs;) {
                if (Point::collide(points[i], points[j])) {
                    points[i] += points[j];
                    objs--;
                    points[j] = points[objs];
                    if(!changed) changed = true;
                } else {
                    j++;
                }
            }
            if(changed) points[i].update();
            while (points.size() != objs) points.pop_back();

        }
    }

    inline void checkBounds(Point &p) {
        if (p.pos.x < 0) {
            p.pos.x = 0;
            p.vel.x *= -1;
        }
        if (p.pos.y < 0) {
            p.pos.y = 0;
            p.vel.y *= -1;
        }
        if (p.pos.z < 0) {
            p.pos.z = 0;
            p.vel.z *= -1;
        }
        if (p.pos.x > size) {
            p.pos.x = size;
            p.vel.x *= -1;
        }
        if (p.pos.y > size) {
            p.pos.y = size;
            p.vel.y *= -1;
        }
        if (p.pos.z > size) {
            p.pos.z = size;
            p.vel.z *= -1;
        }
    }

public:
    std::vector<Point> points;
    std::vector<unsigned int> trash;

    AosSimulator(int objs, int seed, double size, double dt) {

        this->objs = objs;

        this->size = size;
        this->dt = dt;
        trash = std::vector<unsigned int>(objs);
        for(auto i=0; i<objs;objs++){
            trash[i]=i;
        }
        mt19937_64 gen(seed);
        uniform_real_distribution<double> ud(0, size);
        normal_distribution<double> nd(1e21, 1e15);
        double x, y, z, m;

        for (unsigned int i = 0; i < this->objs; i++) {
            x = ud(gen);
            y = ud(gen);
            z = ud(gen);
            m = nd(gen);
            points.emplace_back(x, y, z, m);
        }
        //checkCollisions();
    }

    bool comp(int i, int j){
        return points[i].pos.x<points[j].pos.x;
    }



    void run(const int iterations) override {
        omp_set_num_threads(16);
            for (auto l = 0; l < iterations; l++) {
                int min_slice = (int) objs/16;

#pragma omp parallel
                {
                    auto id = omp_get_thread_num();
                    int start = min_slice*id;
                    int end;
                    if(id==15){
                        end = (int) objs;
                    } else {
                        end = start+min_slice;
                    }
                for (int  i = start; i < end; i++) {
                    SpaceVector forcesum;
                    for (int j = 0; j < (int) objs; j++) {
                        if(i!=j){
                            auto force_vec = (points[j].pos - points[i].pos);
                            auto force_vec_prod = force_vec.dotProduct();
                            force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
                            auto force_ij = force_vec * ((G * points[i].mass * points[j].mass) / force_vec_prod);
                            forcesum += force_ij;
                        }
                    }
                    points[i].move(dt, forcesum);
                    checkBounds(points[i]);
                }
                sort(trash.begin()+start, trash.begin()+end,
                     [this](const unsigned int i, const unsigned int j) -> bool
                     {
                         return points[i].pos.x < points[j].pos.x;
                     });
            }
            for(int i = 0; i<16; i++){
                if(i!=15){
                    inplace_merge(trash.begin()+min_slice*i, trash.begin()+min_slice*(i+1), trash.begin()+min_slice*(i+2),
                         [this](const unsigned int i, const unsigned int j) -> bool
                         {
                             return points[i].pos.x < points[j].pos.x;
                         });
                }
            }



            }
    }
};
inline std::ostream &operator<<(std::ostream &os, const AosSimulator &s) {
    os.precision(3);
    os << fixed;
    os << s.size << ' ' << s.dt << ' ' << s.objs << '\n';
    for (auto &p: s.points) os << p << '\n';
    return os;
}