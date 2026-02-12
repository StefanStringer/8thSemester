#include<cmath>
#include<limits>
#include<numbers> //howwever it is for c++20
#include "sfuns.h"

namespace sfuns {
    constexpr double PI = std::numbers::pi;
    constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

static double stirling_ln_gamma(double x)
{

    double term = 1.0 / (12.0 * x - 1.0 / x / 10.0);
    double ln_fg = x * std::log(x + term) - x
                 + std::log(2.0 * PI / x) / 2.0;
    return ln_fg;
}


double fgamma(double x)
{

    if (x < 0.0) {

        double sin_px = std::sin(PI * x);
        if (sin_px == 0.0)
            return std::numeric_limits<double>::quiet_NaN();

        return PI / (sin_px * fgamma(1.0 - x));
    }

    // if (x == 1.0)
    //     return 1.0;

    if (x < 9.0)
        return fgamma(x + 1.0) / x;

    return std::exp(stirling_ln_gamma(x));
}

double lngamma(double x)
{
    if (x <= 0.0)
        return std::numeric_limits<double>::quiet_NaN();

    // if (x == 1.0)
    //     return 0.0;

    if (x < 9.0)
        return lngamma(x + 1.0) - std::log(x);

    return stirling_ln_gamma(x);
}

}