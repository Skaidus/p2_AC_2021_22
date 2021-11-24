
//
// Created by Usuario on 10/10/2021.
//

//imports
#include "Simulator.hpp"
#include "../Point/Point.cpp"
#include <omp.h>


using namespace std;

class AosSimulator : public Simulator {
private:

    void checkCollisions() override {
#pragma omp for schedule(static, 64)
        for (int i = 0; i < objs; i++) {
            for (int j = 0; j < i; j++) {
                if (Point::collide(points[i], points[j])) {
                    points[i].killer = j;
                    points[i].killed = true;
                    break;
                }
            }
        }
    }

    void resolveCollisions()  {
        for (int i =  objs-1; i >= 0;i--) {
            if (points[i].killed) {
                points[points[i].killer] += points[i];
                objs--;
                points[i] = points[objs];
            } else {
                points[i].update();
            }
        }
        while (((int) points.size()) != objs) points.pop_back();
    }

    void checkCollisions2()  {
        for (int i = 0; i < objs; i++) {
            for (int  j = i + 1; j < objs;) {
                if (Point::collide(points[i], points[j])) {
                    points[i] += points[j];
                    objs--;
                    points[j] = points[objs];
                } else {
                    j++;
                }
            }
            while ((int)points.size() != objs) points.pop_back();

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

        for (int i = 0; i < this->objs; i++) {
            x = ud(gen);
            y = ud(gen);
            z = ud(gen);
            m = nd(gen);
            points.emplace_back(x, y, z, m);
        }
    }

    void run(const int iterations, const int numthreads) override {
        omp_set_num_threads(numthreads);

#pragma omp parallel default(none)
        checkCollisions();
        resolveCollisions();
        for (auto l = 0; l < iterations; l++) {
#pragma omp parallel default(none)
        {
#pragma omp for schedule(static)
                for (int i = 0; i < objs; i++) {
                    for (int j = 0; j < objs; j++) {
                        if (i != j) points[i].addForce(points[j]);
                    }
                }
#pragma omp for schedule(static)
                for (int i = 0; i < objs; i++) {
                    points[i].move(dt);
                    checkBounds(points[i]);
                }
                checkCollisions();

        };
        resolveCollisions();
        //checkCollisions2();
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