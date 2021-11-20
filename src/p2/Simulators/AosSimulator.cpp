
//
// Created by Usuario on 10/10/2021.
//

//imports
#include "Simulator.hpp"
#include "../merge/mergesort.cpp"
#include <omp.h>
#include <cmath>

using namespace std;

struct grid {
    SpaceVector min;
    SpaceVector max;
    grid(double x, double y, double z, double partition): min{x,y,z}, max{x+partition,y+partition,z+partition}{}
    vector <unsigned int> points;
    bool in(const SpaceVector v){
        return (x<v.x<(x+partition))
    }
};

class AosSimulator : public Simulator {
private:
    void checkCollisions() override {
        merge_sort(x_axis.begin(), x_axis.end(), 6);
        vector <unsigned int> uniform_grid[6];

#pragma omp parallel for schedule(static) num_threads(6)
            for (unsigned int i = 0; i < x_axis.size() -1 ; i++) {
                if(x_axis[i].value-x_axis[i+1].value==0){
                    auto j = x_axis[i].index;
                    auto k = x_axis[i+1].index;
                    if (Point::collide(points[j], points[k])) {
                        points[min(j,k)] += points[max(j,k)];
                        trash[omp_get_thread_num()].emplace_back(k);
                    }
                }
            }
        for(auto i = 5; i>=0; i--){
            for(auto j: trash[i]){
                objs--;
                x_axis[j]=x_axis[objs];
            }
        }
        while (x_axis.size() != objs) x_axis.pop_back();
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
    std::vector<Ordinate> x_axis;
    grid space_partition[6];
    double partition;

    AosSimulator(int objs, int seed, double size, double dt) {

        this->objs = objs;
        this->size = size;
        this->dt = dt;
        partition = size/3.0;
        for(auto &g: space_partition){
            for(auto i=1; i<4; i++){
                for(auto j=1; j<4; j++){
                    for(auto k=1; k<4; k++){
                        g.x = i*partition;
                        g.y = j*partition;
                        g.z = k*partition;
                    }
                }
            }
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
            x_axis.emplace_back(i,(unsigned int) x);
        }
        checkCollisions();
    }

    void add_force(unsigned int first, unsigned  int last, unsigned  int threads)
    {
        unsigned int middle;

        if (threads > 1){
            middle = first + (last - first) / 2;
#pragma omp parallel sections
            {
#pragma omp section
                {
                    add_force(first, middle, threads/2);
                }
#pragma omp section
                {
                    add_force(middle, last, threads - threads/2);
                }
            }
        } else {
            middle = last;
        }
        for(auto i = first; i < middle; i++){
            for(auto j = i + 1; j < last; j++){
                points[x_axis[i].index].addForce(points[x_axis[j].index]);
            }
        }
    }

    void run(const int iterations) override {
        for (auto l = 0; l < iterations; l++) {
            for (unsigned int  i = 0; i < x_axis.size(); i++) {
                for (auto j = i + 1; j < x_axis.size(); j++) points[x_axis[i].index].addForce(points[x_axis[j].index]);
            }
            for (unsigned int i = 0 ; i < points.size(); i++) {
                    points[i].move(dt);
                    checkBounds(points[i]);
                    for(auto j=0; j<6;j++){
                        space_partition[j]
                    }
            }
//            add_force(0, x_axis.size(), 6);
//#pragma omp parallel for schedule(static)  num_threads(6)
//                for (unsigned int i = 0 ; i < x_axis.size(); i++)  {
//                    points[x_axis[i].index].move(dt);
//                    checkBounds(points[x_axis[i].index]);
//                    x_axis[i].value = (unsigned int) points[x_axis[i].index].pos.x;
//                }

            checkCollisions();
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