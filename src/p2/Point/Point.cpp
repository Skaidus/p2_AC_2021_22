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
    int order;
    double mass{}; // Mass of the point
    double mass_inv{}; // Inverse to save up time

    // Constructors
    inline Point(double x, double y, double z, int order, double mass) : pos(x, y, z), order {order}, mass{mass}, mass_inv{1.0 / mass}{}

    // Fusion
    inline Point(SpaceVector pos, SpaceVector vel,  int order, double mass) : pos{pos}, vel{vel}, order{order}, mass{mass}{}

    inline Point(const Point &) = default;

    inline Point &operator=(Point p) {
        vel = p.vel;
        mass = p.mass;
        mass_inv = 1 / mass;
        return *this;
    }


    // Functions

    inline Point operator + (const Point p) const {
        if (p.order < order) {
            return {p.pos, p.vel + vel, p.order, p.mass + mass};
        } else {
            return {pos, p.vel + vel, order, p.mass + mass};
        }
    }

    inline void move(double time) {
        vel += (forcesum * (time * mass_inv));
        forcesum = 0;
        pos += (vel * time);
    }

    inline void addForce(const Point &p) {
        auto force_vec = (p.pos - pos);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        auto force_ij = force_vec * ((G * mass * p.mass) / force_vec_prod);
        forcesum += force_ij;
        //p.forcesum -= force_ij;
    }

    inline static bool real_collide(const Point p1, const Point p2) {
        return (p1.pos - p2.pos).dotProduct() < 1;
    }

    inline static bool collide(const Point p1, const Point p2) {
        auto prod_x = p1.pos.x - p2.pos.x;
        prod_x = prod_x * prod_x;
        if(prod_x>=1) return false;
        auto prod_y = p1.pos.y - p2.pos.y;
        prod_y = prod_y * prod_y;
        auto prod_z = p1.pos.z - p2.pos.z;
        prod_z = prod_z * prod_z;
        return prod_x + prod_y + prod_z < 1;
    }

};

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << p.pos << ' ' << p.vel << ' ' << p.mass;
    return os;
}