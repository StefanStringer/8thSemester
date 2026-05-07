#include "vec.h"
#include <iomanip>
#include <cmath>
#include <algorithm> // for std::max

// --- Member Operators ---
vec& vec::operator+=(const vec& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

vec& vec::operator-=(const vec& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

vec& vec::operator*=(double s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

vec& vec::operator/=(double s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

// --- Utility ---
void vec::print(const std::string& s) const {
    std::cout << s << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

// --- Stream Output ---
std::ostream& operator<<(std::ostream& os, const vec& v) {
    os << std::fixed << std::setprecision(6)
       << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
    return os;
}


// MODERN FORM: Takes 'a' by value. 
// If 'a' is a temporary, it is moved in. We modify it and return it.
vec operator+(vec a, const vec& b) {
    a += b;
    return a;
}

vec operator-(const vec& a, const vec& b) {
    return vec(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec operator-(const vec& v) {
    return vec(-v.x, -v.y, -v.z);
}

vec operator*(const vec& v, double s) {
    return vec(v.x * s, v.y * s, v.z * s);
}

vec operator*(double s, const vec& v) {
    return v * s;
}

vec operator/(const vec& v, double s) {
    return vec(v.x / s, v.y / s, v.z / s);
}

// --- Approximate Equality ---
bool approx(const vec& a, const vec& b, double acc, double eps) {
    auto close = [&](double ai, double bi) {
        double diff = std::abs(ai - bi);
        
        if (diff <= acc) return true;
        
        double max_val = std::max(std::abs(ai), std::abs(bi));
        if (max_val == 0.0) return true;
        
        return (diff / max_val) <= eps;
    };

    return close(a.x, b.x) && close(a.y, b.y) && close(a.z, b.z);
}