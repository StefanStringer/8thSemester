#pragma once
#include "matrix.hpp"
#include <vector>
#include <functional>

namespace ode {

using Eigenvalues::vector;

// RK stepper (Euler/Midpoint embedded)
std::pair<vector,vector> rkstep12(
    std::function<vector(double,const vector&)> f,
    double x,
    const vector& y,
    double h
);

// Adaptive driver
std::pair<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double,const vector&)> f,
    double a,
    double b,
    const vector& y,
    double h,
    double acc,
    double eps
);

}