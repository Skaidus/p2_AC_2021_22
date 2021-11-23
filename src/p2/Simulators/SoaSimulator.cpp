//
// Created by Usuario on 10/12/2021.
//

// Imports
#include "Simulator.hpp"
#include "../Point/Point.hpp"
#include <omp.h>

using namespace std;

class SoaSimulator : public Simulator {
private:
    void checkCollisions2() override {
        for (unsigned int i = 0; i < objs; i++) {
            for (unsigned int j = i + 1; j < objs;) {
                if (collide(i, j)) {
                    absorb(i, j);
                } else {
                    j++;
                }
            }
        }
    }

    void checkCollisions() override {
#pragma omp for schedule(auto)
        for (int i = 0; i < objs; i++) {
            for (int j = 0; j < i; j++) {
                if (collide(i, j)) {
                    killer[i] = j;
                    killed[i] = true;
                    break;
                }
            }
        }
    }

    void resolveCollisions()  {
        for (int i =  objs-1; i >= 0;i--) {
            if (killed[i]) {
                absorb(killer[i], i);
                objs--;
                points[i] = points[objs];
            } else {
                points[i].update();
            }
        }
        while ((int) points.size() != objs) points.pop_back();
    }

    inline void checkBounds(static int i) {
        if (pos[i].x < 0) {
            pos[i].x = 0;
            vel[i].x *= -1;
        }
        if (pos[i].y < 0) {
            pos[i].y = 0;
            vel[i].y *= -1;
        }
        if (pos[i].z < 0) {
            pos[i].z = 0;
            vel[i].z *= -1;
        }
        if (pos[i].x > size) {
            pos[i].x = size;
            vel[i].x *= -1;
        }
        if (pos[i].y > size) {
            pos[i].y = size;
            vel[i].y *= -1;
        }
        if (pos[i].z > size) {
            pos[i].z = size;
            vel[i].z *= -1;
        }
    }

    inline bool collide(int i, int j) {
        return (pos[i] - pos[j]).dotProduct() < 1;
    }


    inline void absorb(static int i,static int j) {
        vel[i] += vel[j];
        mass[i] += mass[j];
        if(!updated[i]) {updated[i]=true;}
        deletePoint(j);
    }

    inline void update(static int i){
        if(updated[i]){
            mass_inv[i] = 1.0 / mass[i];
            updated[i]=false;
        }
    }

    inline void addForce(int i, int j) {
        auto force_vec = (pos[j] - pos[i]);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        auto force_ij = force_vec * ((G * mass[i] * mass[j]) / force_vec_prod);
        force[i] += force_ij;
    }

    inline void move(const int i, const SpaceVector forcesum) {
        vel[i] += (forcesum * (dt * mass_inv[i]));
        pos[i] += (vel[i] * dt);
    }

    inline void deletePoint(const int i) {
        objs--;
        pos[i] = pos[objs];
        vel[i] = vel[objs];
        force[i] = force[objs];
        mass[i] = mass[objs];
        mass_inv[i] = mass_inv[objs];
        killed[i]=killed[objs];
        killer[i]=killer[objs];
        updated[i]=updated[objs];
    }

public:
    std::vector<SpaceVector> pos;
    std::vector<SpaceVector> vel;
    std::vector<double> mass;
    std::vector<double> mass_inv;
    std::vector<bool> killed;
    std::vector<int> killer;
    std::vector<bool> updated;

    SoaSimulator(int objs, int seed, double size, double dt) {
        this->objs = objs;
        this->size = size;
        this->dt = dt;
        vel = vector<SpaceVector>(objs);
        mt19937_64 gen(seed);
        uniform_real_distribution<double> ud(0, size);
        normal_distribution<double> nd(1e21, 1e15);
        double x, y, z, m;
        for (int i = 0; i < objs; i++) {
            x = ud(gen);
            y = ud(gen);
            z = ud(gen);
            m = nd(gen);
            pos.emplace_back(SpaceVector(x, y, z));
            mass.emplace_back(m);
            mass_inv.emplace_back(1.0 / m);
        }
    }

    void run(const int iterations) override {
        omp_set_num_threads(16);

#pragma omp parallel
        checkCollisions();
        resolveCollisions();
        for (auto l = 0; l < iterations; l++) {
#pragma omp parallel
            {
#pragma omp for schedule(static)
                for (int i = 0; i < objs; i++) {
                    SpaceVector forcesum(0);
                    for (int j = 0; j < objs; j++) {
                        if (i != j) addForce(i,j);
                    }
                    move(i, forcesum);
                    checkBounds(i);
                }
                checkCollisions();
            };
            resolveCollisions();
        }
    }
};

inline std::ostream &operator<<(std::ostream &os, const SoaSimulator &s) {
    os.precision(3);
    os << fixed;
    os << s.size << ' ' << s.dt << ' ' << s.objs << '\n';
    for (unsigned int i = 0; i < s.objs; i++) os << s.pos[i] << ' ' << s.vel[i] << ' ' << s.mass[i] << '\n';
    return os;
}
