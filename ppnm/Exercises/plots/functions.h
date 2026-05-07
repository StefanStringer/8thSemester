#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <functional>
#include <cmath>

// Error function approximation
double erf_approx(double x);

// Gamma function (Stirling + recursion)
double sgamma(double x);

// Log-Gamma function
double lngamma(double x);

// Generic data generator
void generate_data(const std::string& filename,
                   std::function<double(double)> f,
                   double xmin, double xmax,
                   int N);

// Gamma data generator (coarse + fine for plotting)
void generate_gamma_data(const std::string& filename,
                         double xmin, double xmax,
                         double dx_coarse, double dx_fine);

// Log-Gamma data generator
void generate_lngamma_data(const std::string& filename,
                           double xmin, double xmax,
                           double dx_coarse, double dx_fine);

#endif