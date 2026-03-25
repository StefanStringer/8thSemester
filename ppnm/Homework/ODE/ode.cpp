#include "ode.hpp"
#include <cmath>
#include <algorithm> // for std::min

namespace ode {

/* =========================================
   RK stepper (Euler/Midpoint embedded)
========================================= */
std::pair<vector,vector> rkstep12(
    std::function<vector(double,const vector&)> f,
    double x,
    const vector& y,
    double h
){
    vector k0 = f(x, y);
    vector k1 = f(x + h/2, y + k0*(h/2));

    vector yh = y + k1*h;        // higher order estimate
    vector dy = (k1 - k0)*h;     // error estimate

    return {yh, dy};
}

/* =========================================
   Adaptive driver
========================================= */
std::pair<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double,const vector&)> f,
    double a,
    double b,
    const vector& y0,
    double h,
    double acc,
    double eps
){
    double x = a;
    vector y = y0;

    std::vector<double> xlist{ x };
    std::vector<vector> ylist{ y };

    while(x < b) {

        if(x + h > b) h = b - x; // last step

        auto step = rkstep12(f, x, y, h);

        vector yh = step.first;
        vector dy = step.second;

        double tol = (acc + eps*yh.norm())*std::sqrt(h/(b-a));
        double err = dy.norm();

        if(err <= tol) {
            x += h;
            y = yh;

            xlist.push_back(x);
            ylist.push_back(y);
        }

        // step size control
        if(err > 0)
            h *= std::min(std::pow(tol/err, 0.25)*0.95, 2.0);
        else
            h *= 2.0;
    }

    return { xlist, ylist };
}

}