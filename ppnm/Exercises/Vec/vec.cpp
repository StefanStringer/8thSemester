// ------------------------------------------------------------
// vec.cpp
// ------------------------------------------------------------
#include "vec.h"
#include <iomanip>   // for std::setprecision (optional, nice printing)
#include <cmath>     // for std::abs, std::max

// ---------- member operators ----------
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
    // (no explicit zero‑check – you can add one if you like)
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

// ---------- utility ----------
void vec::print(const std::string& s) const {
    // Simple debug printer – you can change the format as you wish
    std::cout << s << '(' << x << ", " << y << ", " << z << ')' << std::endl;
}

// ---------- stream output ----------
std::ostream& operator<<(std::ostream& os, const vec& v) {
    // Using a fixed precision makes the output easier to read
    os << std::fixed << std::setprecision(6)
       << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return os;
}

// ---------- non‑member arithmetic ----------
vec operator-(const vec& v) {
    return vec(-v.x, -v.y, -v.z);
}
vec operator-(const vec& a, const vec& b) {
    return vec(a.x - b.x, a.y - b.y, a.z - b.z);
}
vec operator+(const vec& a, const vec& b) {
    return vec(a.x + b.x, a.y + b.y, a.z + b.z);
}
vec operator*(const vec& v, double s) {
    return vec(v.x * s, v.y * s, v.z * s);
}
vec operator*(double s, const vec& v) {
    return v * s;               // reuse the previous overload
}
vec operator/(const vec& v, double s) {
    return vec(v.x / s, v.y / s, v.z / s);
}

// ---------- approximate equality ----------
bool approx(const vec& a,
            const vec& b,
            double acc /* absolute tolerance */,
            double eps /* relative tolerance */) {
    // Helper lambda that checks a single component
    auto close = [&](double ai, double bi) {
        double diff = std::abs(ai - bi);
        double tol  = std::max(acc, eps * std::max(std::abs(ai), std::abs(bi)));
        return diff <= tol;
    };

    // All three components must satisfy the tolerance test
    return close(a.x, b.x) && close(a.y, b.y) && close(a.z, b.z);
}