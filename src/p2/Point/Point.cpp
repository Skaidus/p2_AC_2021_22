//
// Created by Usuario on 10/10/2021.
//

//std
#include <iostream>
//imports
#include "Point.hpp"
#include <bitset>
class Point {
public:
    SpaceVector pos; // Position of the point in the space
    SpaceVector vel; // Velocity of the point in a specific moment
    SpaceVector forcesum{0};
    double mass{}; // Mass of the point
    double mass_inv{}; // Inverse to save up time
    int killer{0};
    bool killed{false};
    bool updated{false};

    // Constructors
    inline Point(double x, double y, double z,  double mass) : pos(x, y, z), mass{mass}, mass_inv{1.0 / mass}{}

    inline Point(const Point &) = default;

    inline Point &operator=(Point p) {
        pos= p.pos;
        vel = p.vel;
        mass = p.mass;
        mass_inv = p.mass_inv;
        killed=p.killed;
        updated=p.updated;
        killer=p.killer;
        return *this;
    }


    inline void update(){
        if(updated) {
            mass_inv = 1.0/mass;
            updated = false;
        }
    }


    // Functions

    inline Point &operator+=(const Point p) {
        vel += p.vel;
        mass += p.mass;
        if(!updated) {updated=true;}
        return *this;
    }

    inline void move(const double time) {
        vel += (forcesum * (time * mass_inv));
        forcesum=0;
        pos += (vel * time);
    }

    inline void addForce(const Point p) {
        auto force_vec = (p.pos - pos);
        auto force_vec_prod = force_vec.dotProduct();
        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
        forcesum += force_vec * ((G * mass * p.mass) / force_vec_prod);
    }

    inline static bool collide(const Point p1, const Point p2) {
        auto sumx = p1.pos.x-p2.pos.x;
        sumx = sumx*sumx;
        if(sumx>=1) return false;
        auto sumy = p1.pos.y-p2.pos.y;
        sumy = sumy * sumy;
        if(sumy>=1) return false;
        auto sumz = p1.pos.z-p2.pos.z;
        sumz = sumz * sumz;
        if(sumz>=1) return false;
        return (sumx+sumy+sumz) < 1;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << p.pos << ' ' << p.vel << ' ' << p.mass;
    return os;
}