#include "functions.h"
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <limits>
#include <iomanip> //this was needed for the precision thing rihft?

constexpr double PI = 3.14159265358979323846;

// ------------------------ Error Function ------------------------
double erf_approx(double x) {
    if (x < 0) return -erf_approx(-x);
    std::vector<double> a{0.254829592, -0.284496736, 1.421413741, -1.453152027, 1.061405429};
    double t = 1.0 / (1.0 + 0.3275911 * x);
    double sum = t * (a[0] + t * (a[1] + t * (a[2] + t * (a[3] + t * a[4]))));
    return 1.0 - sum * std::exp(-x * x);
}

// ------------------------ Gamma Function ------------------------
double sgamma(double x) {
    if (x < 0) return PI / std::sin(PI * x) / sgamma(1.0 - x);
    if (x < 9.0) return sgamma(x + 1.0) / x;

    double lnsgamma = std::log(2.0 * PI) / 2.0 + (x - 0.5) * std::log(x) - x
                     + (1.0 / 12.0) / x - (1.0 / 360.0) / (x * x * x) 
                     + (1.0 / 1260.0) / (x * x * x * x * x);
    return std::exp(lnsgamma);
}

// ------------------------ Log-Gamma Function ------------------------
double lngamma(double x) {
    if (x <= 0.0) return std::numeric_limits<double>::quiet_NaN();
    if (x < 9.0) return lngamma(x + 1.0) - std::log(x);
    
    // Stirling approximation for log-gamma
    return x * std::log(x + 1.0 / (12.0 * x - 1.0 / (10.0 * x))) - x 
           + std::log(2.0 * PI / x) / 2.0;
}

// ------------------------ Generic Data Generator ------------------------
void generate_data(const std::string& filename,
                   std::function<double(double)> f,
                   double xmin, double xmax,
                   int N) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening " << filename << "\n";
        return;
    }
    
    double dx = (xmax - xmin) / (N - 1);
    for (int i = 0; i < N; ++i) {
        double x = xmin + i * dx;
        file << x << " " << f(x) << "\n";
    }
}

// ------------------------ Gamma Data Generator ------------------------
void generate_gamma_data(const std::string& filename,
                         double xmin, double xmax,
                         double dx_coarse, double dx_fine) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening " << filename << "\n";
        return;
    }
    
    file << std::scientific << std::setprecision(10);

    for (double x = xmin; x <= xmax; x += dx_coarse) {
        file << x << " " << sgamma(x) << " " << std::tgamma(x) << "\n";
    }
    
    file << "\n\n";

    for (double x = xmin; x <= xmax; x += dx_fine) {
        file << x << " " << sgamma(x) << " " << std::tgamma(x) << "\n";
    }
}

// ------------------------ Log-Gamma Data Generator ------------------------
void generate_lngamma_data(const std::string& filename,
                           double xmin, double xmax,
                           double dx_coarse, double dx_fine) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening " << filename << "\n";
        return;
    }
    
    file << std::scientific << std::setprecision(10);

    for (double x = xmin; x <= xmax; x += dx_coarse) {
        file << x << " " << lngamma(x) << " " << std::lgamma(x) << "\n";
    }
    
    file << "\n\n";

    for (double x = xmin; x <= xmax; x += dx_fine) {
        file << x << " " << lngamma(x) << " " << std::lgamma(x) << "\n";
    }
}