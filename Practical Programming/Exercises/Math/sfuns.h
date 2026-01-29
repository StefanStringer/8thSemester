#pragma once

#include <cmath>
#include <numbers>
#include <limits>

namespace sfuns {
//constants
inline constexpr double PI = std::numbers::pi;

double fgamma(double x);

double lngamma(double x);
}