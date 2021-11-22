//
// Created by Usuario on 10/10/2021.
//

//std
#include <iostream>
#include <cmath>
//imports
#include "Point.hpp"
#include <bitset>
class Point {
public:
    SpaceVector pos; // Position of the point in the space
    SpaceVector vel; // Velocity of the point in a specific moment
    std::bitset<2> status; // [0]: modified, [1]: deleted
    double mass{}; // Mass of the point
    double mass_inv{}; // Inverse to save up time

    // Constructors
    inline Point(double x, double y, double z,  double mass) : pos(x, y, z), mass{mass}, mass_inv{1.0 / mass}{}

    // Fusion
    inline Point(SpaceVector pos, SpaceVector vel, double mass) : pos{pos}, vel{vel}, mass{mass}{}

    inline Point(const Point &) = default;

    inline Point &operator=(Point p) {
        vel = p.vel;
        mass = p.mass;
        mass_inv = p.mass_inv;

        return *this;
    }

    inline void remove(){
        if(!status[1]){
            status[1]=true;
        }
    }

    inline void modify(){
        if(!status[0]){
            status[0]=true;
        }
    }

    inline void update(){
        if(status[0])
            mass_inv = 1/mass;
    }


    // Functions

    inline Point &operator+=(const Point p) {
        vel += p.vel;
        mass += p.mass;
        return *this;
    }

    inline void move(const double time, const SpaceVector forcesum) {
        vel += (forcesum * (time * mass_inv));
        pos += (vel * time);
    }

//    inline void addForce(Point &p) {
//        auto force_vec = (p.pos - pos);
//        auto force_vec_prod = force_vec.dotProduct();
//        force_vec_prod = force_vec_prod * sqrt(force_vec_prod);
//        auto force_ij = force_vec * ((G * mass * p.mass) / force_vec_prod);
//    }

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