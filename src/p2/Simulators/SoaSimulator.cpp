//
// Created by Usuario on 10/12/2021.
//

// Imports
#include "Simulator.hpp"
#include "../Point/Point.hpp"

using namespace std;

class SoaSimulator : public Simulator {
private:
    void checkCollisions() override {
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

    inline void checkBounds(int i) {
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


    inline void absorb(int i, int j) {
        vel[i] += vel[j];
        mass[i] += mass[j];
        mass_inv[i] = 1 / mass[i];
        deletePoint(j);
    }

    inline void addForce(int i, int j) {
        auto force_vec = (pos[j] - pos[i]);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        auto force_ij = force_vec * ((G * mass[i] * mass[j]) / force_vec_prod);
        force[i] += force_ij;
        force[j] -= force_ij;
    }

    inline void move(int i) {
        vel[i] += (force[i] * (dt * mass_inv[i]));
        force[i] = 0;
        pos[i] += (vel[i] * dt);
    }

    inline void deletePoint(int i) {
        objs--;
        pos[i] = pos[objs];
        vel[i] = vel[objs];
        force[i] = force[objs];
        mass[i] = mass[objs];
        mass_inv[i] = mass_inv[objs];
    }

public:
    std::vector<SpaceVector> pos;
    std::vector<SpaceVector> vel;
    std::vector<SpaceVector> force;
    std::vector<double> mass;
    std::vector<double> mass_inv;

    SoaSimulator(int objs, int seed, double size, double dt) {
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
            pos.emplace_back(SpaceVector(x, y, z));
            mass.emplace_back(m);
            mass_inv.emplace_back(1 / m);
        }
        vel = vector<SpaceVector>(objs);
        force = vector<SpaceVector>(objs);
        checkCollisions();

    }

    void run(int iterations) override {
        for (auto l = 0; l < iterations; l++) {
            for (unsigned int i = 0; i < objs; i++) {
                for (auto j = i + 1; j < objs; j++) addForce(i, j);
            }
            for (unsigned int i = 0; i < objs; i++) {
                move(i);
                checkBounds(i);
            }
            checkCollisions();
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
