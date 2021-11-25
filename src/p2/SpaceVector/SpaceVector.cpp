//
// Created by Usuario on 10/10/2021.
//

//std
#include <iostream>

class SpaceVector {
public:
    double x, y, z;

    // Constructors
    inline SpaceVector() : x{0}, y{0}, z{0} {}

    inline explicit SpaceVector(double s) : x{s}, y{s}, z{s} {}

    inline SpaceVector(double x, double y, double z) : x{x}, y{y}, z{z} {}

    // Operators
    inline SpaceVector &operator+=(const SpaceVector v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }


    inline SpaceVector &operator-=(const SpaceVector v) {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }

    inline SpaceVector &operator*=(const double s) {
        this->x *= s;
        this->y *= s;
        this->z *= s;
        return *this;
    }


    inline SpaceVector &operator=(const double s) {
        x = s;
        y = s;
        z = s;
        return *this;
    }

    inline SpaceVector(const SpaceVector &) = default;

    inline SpaceVector &operator=(const SpaceVector s) {
        x = s.x;
        y = s.y;
        z = s.z;
        return *this;
    }


    inline SpaceVector operator*(const double s) const {
        return {x * s, y * s, z * s};
    }

    inline SpaceVector operator-(const SpaceVector v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    [[nodiscard]] inline double dotProduct() const {
        return x * x + y * y + z * z;
    }

};

inline std::ostream &operator<<(std::ostream &os, const SpaceVector &v) {
    os << v.x << ' ' << v.y << ' ' << v.z;
    return os;
}