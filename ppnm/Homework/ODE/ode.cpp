#include "ode.hpp"
#include <cmath>
#include <algorithm> // for std::min

namespace ode {

/* =========================================
   RK stepper (Euler/Midpoint embedded)
========================================= */
std::pair<vector,vector> rkstep12( //pair used instead of tuple like in his example
    std::function<vector(double,const vector&)> f, /* the f from dy/dx=f(x,y) */
    double x, /* the current value of the variable */
    const vector& y, /* the current value y(x) of the sought function */
    double h /* the step to be taken */
){
    vector k0 = f(x, y); /* embedded lower order formula (Euler) */
    vector k1 = f(x + h/2, y + k0*(h/2));  /* higher order formula (midpoint) */

    vector yh = y + k1*h;        /* y(x+h) estimate */
    vector dy = (k1 - k0)*h;    /* error estimate */

    return {yh, dy}; //changed from the std::make_tuple method 
}

/* =========================================
   Adaptive driver
========================================= */
std::pair<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double,const vector&)> f, /* the f from dy/dx=f(x,y) */
    double a, /* the initial value of the variable */
    double b, /* the final value of the variable */
    const vector& y0,/* y(initial-point) */
    double h, /* initial step-size */ //removed default value as it causes errors
    double acc, /* absolute accuracy goal */
    double eps /* relative accuracy goal */
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