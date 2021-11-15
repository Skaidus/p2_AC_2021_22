
//
// Created by Usuario on 10/10/2021.
//

//imports
#include "Simulator.hpp"
#include "../merge/mergesort.cpp"
#include <omp.h>
#include <cmath>

using namespace std;

class AosSimulator : public Simulator {
private:
    void checkCollisions() override {
        merge_sort(points.begin(), points.end(), 6);
        while (points[points.size() - 1].order == -1) points.pop_back();
#pragma omp parallel num_threads(6)
        {
#pragma omp for schedule(static)
            for (unsigned int i = 0; i < points.size() -1 ; i++) {
                if (Point::collide(points[i], points[i + 1])) {
                    points[i + 1] = points[i] + points[i + 1];
                    points[i].order = -1;
                }
            }
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

    AosSimulator(int objs, int seed, double size, double dt) {

        this->objs = objs;
        this->size = size;
        this->dt = dt;
        mt19937_64 gen(seed);
        uniform_real_distribution<double> ud(0, size);
        normal_distribution<double> nd(1e21, 1e15);
        double x, y, z, m;
        for (int i = 0; i < objs; i++) {
            x = ud(gen);
            y = ud(gen);
            z = ud(gen);
            m = nd(gen);
            points.emplace_back(x, y, z, i, m);
        }
        checkCollisions();
    }

    void run(const int iterations) override {
        for (auto l = 0; l < iterations; l++) {
            unsigned int chunk = points.size()/6;
# pragma omp parallel num_threads(6)
            {
                unsigned int thread_id = omp_get_thread_num();
                unsigned int chunk_l;
                if(thread_id==5){
                    chunk_l = points.size();
                } else {
                    chunk_l = (thread_id+1) * chunk;
                }
                for (unsigned int i = thread_id *chunk; i < chunk_l; i++) {
                    SpaceVector sum{0};
                    for (auto j = i + 1; j < chunk_l; j++) {
                        points[i].addForce(points[j]);
                        auto force_vec = (points[j].pos - points[i].pos);
                        auto force_vec_prod = force_vec.dotProduct();
                        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
                        auto force_ij = force_vec * ((G * points[i].mass * points[j].mass) / force_vec_prod);
                        sum += force_ij;
                        points[j].forcesum -= force_ij;
                    }
//# pragma omp critical
//                    points[i].forcesum += sum;

                }

# pragma omp barrier
                for (unsigned int i = 0 ; i < points.size(); i++)  {
                    points[i].move(dt);
                    checkBounds(points[i]);
                }
            }
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