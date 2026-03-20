#include "spline.hpp"
#include <cmath>

namespace Spline {

//  Binary search
//  Finds interval i such that x[i] <= z < x[i+1]

int binsearch(const vector& x,double z){

    int i=0;
    int j=x.size()-1;

    while(j-i>1){

        int mid=(i+j)/2;

        if(z>x[mid]) i=mid;
        else j=mid;
    }

    return i;
}


// QUESTION A LINEAR PART
//  Linear interpolation

double linterp(const vector& x,const vector& y,double z){

    int i=binsearch(x,z);

    double dx=x[i+1]-x[i];// if(!(dx>0)) throw new Exception("uups...");
    double dy=y[i+1]-y[i];

    return y[i]+dy/dx*(z-x[i]);
}


//  Integral of linear spline (part b of question A)
double linterp_integral(const vector& x,const vector& y,double z){

    int i=binsearch(x,z);

    double sum=0;

    for(int k=0;k<i;k++){

        double dx=x[k+1]-x[k];
        double dy=y[k+1]-y[k];

        sum+=y[k]*dx+0.5*dy*dx;
    }

    double dx=z-x[i];
    double dy=y[i+1]-y[i];
    double slope=dy/(x[i+1]-x[i]);

    sum+=y[i]*dx+0.5*slope*dx*dx;

    return sum;
}

//Question B SPLINE PART quadratic spline interpolation with derivative and
// Quadratic spline constructor
qspline::qspline(const vector& xs,const vector& ys)
: x(xs),y(ys),b(xs.size()-1),c(xs.size()-1){

    int n=x.size();

    vector h(n-1),p(n-1);

    for(int i=0;i<n-1;i++){

        h[i]=x[i+1]-x[i];
        p[i]=(y[i+1]-y[i])/h[i];
    }

    c[0]=0;

    for(int i=0;i<n-2;i++)
        c[i+1]=(p[i+1]-p[i]-c[i]*h[i])/h[i+1];

    c[n-2]/=2;

    for(int i=n-3;i>=0;i--)
        c[i]=(p[i+1]-p[i]-c[i+1]*h[i+1])/h[i];

    for(int i=0;i<n-1;i++)
        b[i]=p[i]-c[i]*h[i];
}



//  Evaluate quadratic spline

double qspline::evaluate(double z){

    int i=binsearch(x,z);

    double dx=z-x[i];

    return y[i]+b[i]*dx+c[i]*dx*dx;
}



//  Quadratic spline derivative

double qspline::derivative(double z){

    int i=binsearch(x,z);

    double dx=z-x[i];

    return b[i]+2*c[i]*dx;
}



//  Quadratic spline integral

double qspline::integral(double z){

    int i=binsearch(x,z);

    double sum=0;

    for(int k=0;k<i;k++){

        double dx=x[k+1]-x[k];

        sum+=y[k]*dx
            +b[k]*dx*dx/2
            +c[k]*dx*dx*dx/3;
    }

    double dx=z-x[i];

    sum+=y[i]*dx
        +b[i]*dx*dx/2
        +c[i]*dx*dx*dx/3;

    return sum;
}

//Part C question, the cubic spline constructor, evaluation, derivative and integral are in the same format as the quadratic spline but with an extra coefficient d for the cubic term. The constructor is more complex as it requires solving a tridiagonal system of equations to find the c coefficients, which are then used to find the b and d coefficients.


cspline::cspline(const vector& xs,const vector& ys)
: x(xs),y(ys),b(xs.size()-1),c(xs.size()),d(xs.size()-1){

    int n = x.size();

    vector h(n-1), alpha(n-1);

    for(int i = 0; i < n-1; i++){
        h[i] = x[i+1] - x[i];
        alpha[i] = (y[i+1] - y[i]) / h[i];
    }

    vector D(n), Q(n - 1), B(n);

    D[0] = 2;
    Q[0] = 1;
    B[0] = 3 * alpha[0];

    for (int i = 1; i < n-1; i++){
        D[i] = 2 * h[i-1] / h[i] + 2;
        Q[i] = h[i-1] / h[i];
        B[i] = 3 * (alpha[i-1] + alpha[i] * h[i-1] / h[i]);
    }

    D[n - 1] = 2;
    B[n - 1] = 3 * alpha[n - 2];

    for (int i = 1; i < n; i++){
        double factor = Q[i - 1] / D[i-1];
        D[i] -= factor;
        B[i] -= factor * B[i - 1];
    }

    c[n - 1] = B[n - 1] / D[n - 1];

    for (int i = n - 2; i >= 0; i--){
        c[i] = (B[i] - Q[i] * c[i + 1]) / D[i];
    }

    for (int i = 0; i < n - 1; i++){
        b[i] = alpha[i] - h[i] * (2 * c[i] + c[i + 1]) / 3;
        d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
    }
}

//cubic spline eval

double cspline::evaluate(double z){

    int i=binsearch(x,z);

    double dx=z-x[i];

    return y[i]+b[i]*dx+c[i]*dx*dx+d[i]*dx*dx*dx;
}

//cubic spline derivative

double cspline::derivative(double z){

    int i=binsearch(x,z);

    double dx=z-x[i];

    return b[i]+2*c[i]*dx+3*d[i]*dx*dx;
}

//cubpic spline integral

double cspline::integral(double z){

    int i=binsearch(x,z);

    double sum=0;

    for(int k=0;k<i;k++){

        double dx=x[k+1]-x[k];

        sum+=y[k]*dx
            +b[k]*dx*dx/2
            +c[k]*dx*dx*dx/3
            +d[k]*dx*dx*dx*dx/4;
    }

    double dx=z-x[i];

    sum+=y[i]*dx
        +b[i]*dx*dx/2
        +c[i]*dx*dx*dx/3
        +d[i]*dx*dx*dx*dx/4;

    return sum;
}

}