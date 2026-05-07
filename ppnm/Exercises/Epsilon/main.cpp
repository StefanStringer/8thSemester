#include <iostream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <cstdio>


bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;
    double max_ab = std::max(std::abs(a), std::abs(b));
    if (max_ab == 0.0) return true;
    return diff <= eps * max_ab;
}

int main() {
    std::cout << "Part 1: machine epsilon\n" << std::endl;

    float f = 1.0f;
    while ((float)(1.0f + f) != 1.0f) { f /= 2.0f; } f *= 2.0f;

    double d = 1.0;
    while ((double)(1.0 + d) != 1.0) { d /= 2.0; } d *= 2.0;

    long double l = 1.0L; //throw the L for long double
    while ((long double)(1.0L + l) != 1.0L) { l /= 2.0L; } l *= 2.0L;

    std::printf("      float eps=%g\n", f);
    std::printf("     double eps=%g\n", d);
    std::printf("long double eps=%Lg\n", l);

    std::cout << std::numeric_limits<float>::epsilon() << "\n";
    std::cout << std::numeric_limits<double>::epsilon() << "\n";
    std::cout << std::numeric_limits<long double>::epsilon() << "\n";

    std::cout << "Calculated epsilon double = " << std::pow(2.0, -52.0) << std::endl;
    std::cout << "Calculated epsilon float  = " << std::pow(2.0, -23.0) << std::endl;

    std::cout << "\nPart 2: non-commutativity of addition\n" << std::endl;
    double epsilon = std::pow(2.0, -52.0);
    double tiny = epsilon / 2.0;
    double a = 1.0 + tiny + tiny;
    double b = tiny + tiny + 1.0;

    std::cout << "a==b ? " << (a == b ? "true" : "false") << "\n";
    std::cout << "a>1  ? " << (a > 1.0 ? "true" : "false") << "\n";
    std::cout << "b>1  ? " << (b > 1.0 ? "true" : "false") << "\n";

    std::cout << std::fixed << std::setprecision(17);
    std::cout << "       tiny=" << tiny << "\n";
    std::cout << "1+tiny+tiny=" << a << "\n";
    std::cout << "tiny+tiny+1=" << b << "\n";

    std::cout << "\nPart 3: comparing doubles\n" << std::endl;
    
    double d1 = 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1;
    double d2 = 8 * 0.1;

    std::cout << "d1==d2? " << (d1 == d2 ? "true" : "false") << "\n";
    
    std::cout << std::fixed << std::setprecision(17);
    std::cout << "d1=" << d1 << "\n";
    std::cout << "d2=" << d2 << "\n";

    bool same = approx(d1, d2);
    std::cout << "\nUsing approx(d1, d2) with default tolerances:\n";
    std::cout << (same ? "≈ (approximately equal)" : "≠ (different)") << '\n';

    double tighterAcc = 1e-12;
    double tighterEps = 1e-12;
    bool sameTight = approx(d1, d2, tighterAcc, tighterEps);
    std::cout << "\nWith tighter tolerances (1e-12):\n";
    std::cout << (sameTight ? "≈" : "≠") << '\n';

    return 0;
}