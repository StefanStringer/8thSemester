#pragma once
#include <iostream>
#include <cmath>
#include <complex>
#include <type_traits>
#include <algorithm>

// Trait to check if T is a specialization of std::complex
template<typename T>
struct is_complex : std::false_type {};

template<typename U>
struct is_complex<std::complex<U>> : std::true_type {};

template<typename T>
inline constexpr bool is_complex_v = is_complex<T>::value;

// Helper to check if T is an arithmetic type or complex
template<typename T>
struct is_arithmetic_or_complex : std::integral_constant<bool, 
    std::is_arithmetic_v<T> || is_complex_v<T>
> {};

template<typename T>
inline constexpr bool is_arithmetic_or_complex_v = is_arithmetic_or_complex<T>::value;

template<typename T>
class vec {
public:
    T x, y, z;

    // --- Constructors ---
    vec(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    vec() : x{}, y{}, z{} {} // Default initialization
    
    // Copy/Move constructors and assignment are implicitly generated
    vec(const vec&) = default;
    vec(vec&&) = default;
    vec& operator=(const vec&) = default;
    vec& operator=(vec&&) = default;

    // --- Member Operators (Compound) ---
    vec& operator+=(const vec& rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    vec& operator-=(const vec& rhs) {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }
    vec& operator*=(const T& s) {
        x *= s; y *= s; z *= s;
        return *this;
    }
    vec& operator/=(const T& s) {
        x /= s; y /= s; z /= s;
        return *this;
    }

    // --- Utility ---
    void print(const std::string& s = "") const {
        std::cout << s << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // --- Math Operations ---
    // Dot product: returns T (scalar)
    T dot(const vec& b) const {
        return x * b.x + y * b.y + z * b.z;
    }

    // Cross product: returns vec<T>
    vec cross(const vec& b) const {
        return vec(
            y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x
        );
    }

    // Norm: returns T (magnitude)
    T norm() const {
        if constexpr (is_complex_v<T>) {
            using V = typename T::value_type;
            V sum_sq = std::norm(x) + std::norm(y) + std::norm(z);
            return static_cast<T>(std::sqrt(sum_sq)); 
        } else {
            return std::sqrt(x*x + y*y + z*z);
        }
    }

    // --- Friend Stream Output (Must be template) ---
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const vec<U>& v);
};

// --- Non-Member Operators (Templates) ---

// Modern form: takes first arg by value
template<typename T>
vec<T> operator+(vec<T> a, const vec<T>& b) {
    a += b;
    return a;
}

template<typename T>
vec<T> operator-(const vec<T>& a, const vec<T>& b) {
    return vec<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
vec<T> operator-(const vec<T>& v) {
    return vec<T>(-v.x, -v.y, -v.z);
}

template<typename T>
vec<T> operator*(const vec<T>& v, const T& s) {
    return vec<T>(v.x * s, v.y * s, v.z * s);
}

template<typename T>
vec<T> operator*(const T& s, const vec<T>& v) {
    return v * s;
}

template<typename T>
vec<T> operator/(const vec<T>& v, const T& s) {
    return vec<T>(v.x / s, v.y / s, v.z / s);
}

// --- Stream Output Implementation ---
template<typename T>
std::ostream& operator<<(std::ostream& os, const vec<T>& v) {
    os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
    return os;
}

// --- Approximate Equality ---
template<typename T>
bool approx(const vec<T>& a, const vec<T>& b, T acc = T(1e-6), T eps = T(1e-6)) {
    auto close = [&](const T& ai, const T& bi) {
        T diff = std::abs(ai - bi);
        if (diff <= acc) return true;
        
        T max_val = std::max(std::abs(ai), std::abs(bi));
        if (max_val == T{}) return true;
        
        return (diff / max_val) <= eps;
    };

    return close(a.x, b.x) && close(a.y, b.y) && close(a.z, b.z);
}