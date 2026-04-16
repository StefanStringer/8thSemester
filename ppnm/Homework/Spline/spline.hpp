// i still dont know why but its here
#pragma once
//my file imports from program
#include "matrix.hpp"

namespace Spline {

using Eigenvalues::vector;
// PART A
/*---------------------------------------------------
  Binary search
---------------------------------------------------*/
int binsearch(const vector& x,double z);

/*---------------------------------------------------
  Linear spline
---------------------------------------------------*/
double linterp(const vector& x,const vector& y,double z);

double linterp_integral(const vector& x,const vector& y,double z);

//Question B SPLINE PART quadratic spline interpolation with derivative and anti-derivative
/*---------------------------------------------------
  Quadratic spline class
---------------------------------------------------*/
struct qspline{

    vector x,y,b,c;

    qspline(const vector& xs,const vector& ys);

    double evaluate(double z);
    double derivative(double z);
    double integral(double z);
};
//Question C SPLINE PART cubic spline interpolation with derivative and anti-derivative
/*---------------------------------------------------
  Cubic spline class
---------------------------------------------------*/
struct cspline{

    vector x,y,b,c,d;

    cspline(const vector& xs,const vector& ys);

    double evaluate(double z);
    double derivative(double z);
    double integral(double z);
};

}