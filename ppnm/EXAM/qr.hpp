#pragma once
#include "matrix.hpp"
#include <vector>
#include <functional>
#include <tuple>

namespace lineq {
//throw the funcs from hpp in here!!
void QR_decomp(matrix &A, matrix &R);

vector backsub(const matrix &R,const vector &b);
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