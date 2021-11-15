//
// Created by Usuario on 10/10/2021.
//

//std
#include <iostream>
#include <cmath>
//imports
#include "Point.hpp"

class Point {
public:
    SpaceVector pos; // Position of the point in the space
    SpaceVector vel; // Velocity of the point in a specific moment
    SpaceVector forcesum; // Sum of the forces of all points
    bool valid{true};
    double mass{}; // Mass of the point
    double mass_inv{}; // Inverse to save up time

    // Constructors
    inline Point(double x, double y, double z, double mass) : pos(x, y, z),  mass{mass}, mass_inv{1.0 / mass}{}

    // Fusion
    inline Point(SpaceVector pos, SpaceVector vel,  double mass) : pos{pos}, vel{vel}, mass{mass}{}

    inline Point(const Point &) = default;

    inline Point &operator=(Point p) {
        vel = p.vel;
        mass = p.mass;
        mass_inv = 1 / mass;
        return *this;
    }


    // Functions
    // Absorbs particle
    inline Point &operator+=(Point p) {
        vel += p.vel;
        mass += p.mass;
        mass_inv = 1 / mass;
        return *this;
    }

    inline void move(double time) {
        vel += (forcesum * (time * mass_inv));
        forcesum = 0;
        pos += (vel * time);
    }

    inline void addForce(Point &p) {
        auto force_vec = (p.pos - pos);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        auto force_ij = force_vec * ((G * mass * p.mass) / force_vec_prod);
        forcesum += force_ij;
        p.forcesum -= force_ij;
    }


    inline static bool collide(const Point p1, const Point p2) {
        return (p1.pos - p2.pos).dotProduct() < 1;
    }

};

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << p.pos << ' ' << p.vel << ' ' << p.mass;
    return os;
}