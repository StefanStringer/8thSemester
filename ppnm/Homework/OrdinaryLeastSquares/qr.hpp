#pragma once
#include "matrix.hpp"
#include <vector>
#include <functional>
#include <tuple>

namespace Eigenvalues {

// Least squares fit returning
// 1. best-fit coefficients
// 2. covariance matrix
std::tuple<vector,matrix> lsfit(
    std::vector<std::function<double(double)>> fs,
    vector x,
    vector y,
    vector dy
);

}