#include "vec.h"
#include <iostream>
#include <complex>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);

    // 1. Double Precision (Standard)
    std::cout << "--- Double Precision ---\n";
    vec<double> v1(1.0, 2.0, 3.0);
    vec<double> v2(4.0, 5.0, 6.0);
    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";
    std::cout << "v1 + v2 = " << v1 + v2 << "\n";
    std::cout << "v1 . v2 = " << v1.dot(v2) << "\n";
    std::cout << "v1 x v2 = " << v1.cross(v2) << "\n";
    std::cout << "|v1| = " << v1.norm() << "\n";

    // 2. Float Precision
    std::cout << "\n--- Float Precision ---\n";
    vec<float> vf1(1.0f, 2.0f, 3.0f);
    vec<float> vf2(4.0f, 5.0f, 6.0f);
    std::cout << "vf1 = " << vf1 << "\n";
    std::cout << "vf1.norm() = " << vf1.norm() << "\n";

    // 3. Complex Numbers (Extra)
    std::cout << "\n--- Complex Numbers ---\n";
    using C = std::complex<double>;
    vec<C> vc1(C(1.0, 2.0), C(3.0, 4.0), C(5.0, 6.0));
    vec<C> vc2(C(0.0, 1.0), C(1.0, 0.0), C(1.0, 1.0));
    
    std::cout << "vc1 = " << vc1 << "\n";
    std::cout << "vc2 = " << vc2 << "\n";
    std::cout << "vc1 + vc2 = " << vc1 + vc2 << "\n";
    std::cout << "vc1 . vc2 = " << vc1.dot(vc2) << "\n";
    std::cout << "|vc1| = " << vc1.norm() << "\n";

    // 4. Approximation Test
    std::cout << "\n--- Approximation Test ---\n";
    vec<double> va(1.0000001, 2.0000001, 3.0000001);
    vec<double> vb(1.0, 2.0, 3.0);
    std::cout << "va approx vb? " << (approx(va, vb) ? "Yes" : "No") << "\n";

    return 0;
}