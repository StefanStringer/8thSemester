#include "functions.h"
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

// ------------------------ Error Function ------------------------
double erf_approx(double x){
    if(x < 0) return -erf_approx(-x);
    std::vector<double> a {0.254829592,-0.284496736,1.421413741,
                           -1.453152027,1.061405429};
    double t = 1.0 / (1.0 + 0.3275911 * x);
    double sum = t*(a[0] + t*(a[1] + t*(a[2] + t*(a[3] + t*a[4]))));
    return 1.0 - sum * std::exp(-x*x);
}

// ------------------------ Gamma Function ------------------------
double sgamma(double x){
    if(x < 0) return PI / std::sin(PI*x) / sgamma(1-x);
    if(x < 9) return sgamma(x+1) / x;

    double lnsgamma = std::log(2*PI)/2 + (x-0.5)*std::log(x) - x
                     + (1.0/12)/x - (1.0/360)/(x*x*x) + (1.0/1260)/(x*x*x*x*x);
    return std::exp(lnsgamma);
}

double lngamma(double x){
    constexpr double PI = 3.14159265358979323846;
    if(x <= 0) return NAN;
    if(x < 9) return lngamma(x+1) - std::log(x);
    return x * std::log(x + 1/(12*x - 1/(10*x))) - x + std::log(2*PI/x)/2;
}


// ------------------------ Generic Data Generator ------------------------
void generate_data(const std::string& filename,
                   std::function<double(double)> f,
                   double xmin, double xmax,
                   int N)
{
    std::ofstream file(filename);
    double dx = (xmax - xmin) / (N-1);

    for(int i=0; i<N; ++i){
        double x = xmin + i*dx;
        file << x << " " << f(x) << "\n";
    }
}

// ------------------------ Gamma Data Generator (coarse+fine) ------------------------
void generate_gamma_data(const std::string& filename,
                         double xmin, double xmax,
                         double dx, double dx2)
{
    std::ofstream file(filename);
    file << std::scientific;

    // Coarse points
    for(double x = xmin; x <= xmax; x += dx){
        file << x << " " << sgamma(x) << " " << std::tgamma(x) << "\n";
    }
    file << "\n\n";

    // Fine points
    for(double x = xmin; x <= xmax; x += dx2){
        file << x << " " << sgamma(x) << " " << std::tgamma(x) << "\n";
    }
}

void generate_lngamma_data(const std::string& filename,
                           double xmin, double xmax,
                           double dx, double dx2)
{
    std::ofstream file(filename);
    file << std::scientific;

    for(double x=xmin; x<=xmax; x+=dx){
        file << x << " " << lngamma(x) << " " << std::lgamma(x) << "\n";
    }
    file << "\n\n";

    for(double x=xmin; x<=xmax; x+=dx2){
        file << x << " " << lngamma(x) << " " << std::lgamma(x) << "\n";
    }
}

