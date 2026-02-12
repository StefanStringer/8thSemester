
#include <iostream>
#include <iomanip>
#include "sfuns.h"
#include <numbers>
#include <complex>

using complex=std::complex<double>;
constexpr double  π = 3.14159265358979324;
constexpr double  E = 2.71828182845904523;
constexpr complex I = complex(0,1);

int main()
{   
    std::cout << "Part One\n"; 
    std::cout << std::setprecision(15);
    std::cout << "Ln(I) = " << std::log(I) << " Precise value: 1.5707963267948966i" << "\n";
    std::cout << " I^I =" << std::pow(I, I) << " Precise value: 0.20787957635076193" << "\n";
    std::cout << "e^(I) = " << std::exp(I) << " Precise value: (0.5403023058681398, 0.8414709848078965)" << "\n"; 
    std::cout << "π ^ (I) =" << std::pow(π, I) << " Precise value: (0.41329211610159433, 0.9105984992126147)" << "\n";
    std::cout << "sqrt(2) = " << std::sqrt(2.0) << " Precise value: 1.4142135623730951" << "\n";
    std::cout << "21/5   = " << 21.0 / 5.0 << "Precise value: 4.2" <<'\n';
    std::cout << "2 ^(1/5) = " << std::pow(2.0, 1.0/5.0) << " Precise value: 1.148698354997035" << "\n";
    std::cout << "e^π    = " << std::pow(E, π) << " Precise value: 23.140692632779267" << "\n";
    std::cout << "π·e    = " << std::pow(π, E) << " Precise value: 22.45915771836104" << "\n";

    std::cout << "\nPart Two\n";
    std::cout << "Gamma(x)  (Stirling approx)   |   lnGamma(x)\n";
    std::cout << "-------------------------------------------------\n";

    for (int i = 1; i <= 10 + 1; i++) {
        double g  = sfuns::fgamma(static_cast<double>(i));
        double lg = sfuns::lngamma(static_cast<double>(i));
        std::cout << "Γ(" << i << ") = " << std::setw(20) << g
                  << " | lnΓ(" << i << ") = " << lg << '\n';
    }
    

    std::cout << "Actual lnGamma values for comparison " << '\n';
    std::cout << "loggamma(1) = " << "0.000000" << std::endl;
    std::cout << "loggamma(2) = " << "0.000000" << std::endl;
    std::cout << "loggamma(3) = " << "0.693147" << std::endl;
    std::cout << "loggamma(4) = " << "1.791759" << std::endl;
    std::cout << "loggamma(5) = " << "3.178053" << std::endl;
    std::cout << "loggamma(6) = " << "4.787491" << std::endl;
    std::cout << "loggamma(7) = " << "6.579251" << std::endl;
    std::cout << "loggamma(8) = " << "8.525161" << std::endl;
    std::cout << "loggamma(9) = " << "10.60460" << std::endl;
    std::cout << "loggamma(10) = " << "12.80182" << std::endl;
    return 0;
}