
//
// Created by Usuario on 10/10/2021.
//

//imports
#include "Simulator.hpp"
#include <omp.h>
#include "../Point/Point.cpp"
#include <cmath>

using namespace std;

class AosSimulator : public Simulator {
private:
    void checkCollisions() override {
        check_local_Collisions(0, points.size(), 6);
        while(!trash.empty()){
            points[trash.back()] = points.back();
            trash.pop_back();
            points.pop_back();
        }
    }

    void check_local_Collisions(unsigned int first, unsigned  int last, unsigned  int threads) {
        unsigned int middle;

        if (threads > 1) {
            middle = first + (last - first) / 2;
#pragma omp parallel sections
            {
#pragma omp section
                {
                    add_force(first, middle, threads / 2);
                }
#pragma omp section
                {
                    add_force(middle, last, threads - threads / 2);
                }
            }
        } else {
            middle = last;
        }
        for (auto i = first; i < middle; i++) {
            if(points[i].valid) {
                SpaceVector vel_sum{};
                double mass_sum = 0;
                bool modified = false;
                for (auto j = i + 1; j < last; j++) {
                    if(points[j].valid) {
                        if (Point::collide(points[i], points[j])) {
                            mass_sum += points[j].mass;
                            vel_sum += points[j].vel;
                            points[j].valid = false;
                            if(not modified) modified = true;
#pragma omp critical
                            trash.emplace_back(j);
                        }
                    }
                }
                if(modified) {
                    points[i].mass += mass_sum;
                    points[i].mass_inv = 1/points[i].mass;
                    points[i].vel += vel_sum;
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
    std::vector<unsigned int> trash{};
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
            points.emplace_back(x, y, z, m);
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
                points[i].addForce(points[j]);
            }
        }
    }

    void run(const int iterations) override {
        for (auto l = 0; l < iterations; l++) {
            add_force(0, points.size(), 6);
# pragma omp parallel for schedule(static) num_threads(6)
            for (unsigned int i = 0 ; i < points.size(); i++)  {
                points[i].move(dt);
                checkBounds(points[i]);
            }
        }
        checkCollisions();
    }

};

inline std::ostream &operator<<(std::ostream &os, const AosSimulator &s) {
    os.precision(3);
    os << fixed;
    os << s.size << ' ' << s.dt << ' ' << s.objs << '\n';
    for (auto &p: s.points) os << p << '\n';
    return os;
}