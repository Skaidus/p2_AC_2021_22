
// Imports
#include "Simulator.hpp"
#include "../Point/Point.hpp"
#include <omp.h>

using namespace std;

class SoaSimulator : public Simulator {
private:
    // Point.cpp-like logic by index

    inline void deletePoint(int i) {
        pos[i] = pos[objs];
        vel[i] = vel[objs];
        mass[i] = mass[objs];
        mass_inv[i] = mass_inv[objs];
    }

    inline void update(int i) {
        if (updated[i]) {
            mass_inv[i] = 1.0 / mass[i];
            updated[i] = false;
        }
    }

    inline void addPoint(int i, int j) {
        vel[i] += vel[j];
        mass[i] += mass[j];
        if (!updated[i]) { updated[i] = true; }
    }

    inline void move(int i) {
        vel[i] += (force[i] * (dt * mass_inv[i]));
        force[i] = 0;
        pos[i] += (vel[i] * dt);
    }

    inline void addForce(const int i, const int j) {
        auto force_vec = (pos[j] - pos[i]);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        force[i] += force_vec * ((G * mass[i] * mass[j]) / force_vec_prod);
    }

    inline bool collide(const int i, const int j) {
        auto sumx = pos[i].x - pos[j].x;
        sumx = sumx * sumx;
        if (sumx >= 1) return false;
        auto sumy = pos[i].y - pos[j].y;
        sumy = sumy * sumy;
        if (sumy >= 1) return false;
        auto sumz = pos[i].z - pos[j].z;
        sumz = sumz * sumz;
        if (sumz >= 1) return false;
        return (sumx + sumy + sumz) < 1;
    }

    // Simulator functions

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

    void resolveCollisions() {
        for (int i = objs - 1; i >= 0; i--) {
            if (killed[i]) {
                addPoint(killer[i], i);
                objs--;
                deletePoint(i);
            } else {
                update(i);
            }
        }
    }

public:
    std::vector<SpaceVector> pos;
    std::vector<SpaceVector> vel;
    std::vector<SpaceVector> force;
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
        force = vector<SpaceVector>(objs);
        killed = vector<bool>(objs);
        killer = vector<int>(objs);
        updated = vector<bool>(objs);
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
                        if (i != j) addForce(i, j);
                    }
                }
#pragma omp for schedule(static)
                for (int i = 0; i < objs; i++) {
                    move(i);
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
    for (int i = 0; i < s.objs; i++) os << s.pos[i] << ' ' << s.vel[i] << ' ' << s.mass[i] << '\n';
    return os;
}
