// main.cc ------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include "sfuns.h"

int main()
{
    // ----- simple math constants -----
    std::cout << std::setprecision(15);
    std::cout << "sqrt(2) = " << std::sqrt(2.0) << '\n';
    std::cout << "21/5   = " << 21.0 / 5.0 << '\n';
    std::cout << "e^π    = " << std::exp(std::numbers::pi) << '\n';
    std::cout << "π·e    = " << std::numbers::pi * std::exp(1.0) << "\n\n";

    // ----- Gamma and lnGamma for 1 .. 10 -----
    std::cout << "Gamma(x)  (Stirling approx)   |   lnGamma(x)\n";
    std::cout << "-----------------------------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        double g  = sfuns::fgamma(static_cast<double>(i));
        double lg = sfuns::lngamma(static_cast<double>(i));
        std::cout << "Γ(" << i << ") = " << std::setw(20) << g
                  << " | lnΓ(" << i << ") = " << lg << '\n';
    }
    return 0;
}