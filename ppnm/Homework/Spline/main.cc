//my file imports from program
#include "spline.hpp"
//imports i need
#include <iostream>
#include <fstream>
#include <cmath>

//name space imports, using eigenvector from that homework
using namespace Spline;
using Eigenvalues::vector;


//Question A – Linear spline
void questionA(){

    std::cout<<"Question A: Linear spline\n\n";
    std::cout<<" Look at .svg files for plots pretaining to question C\n\n";
    std::cout<<" Includes a fig_linear.svg for answer to A.\n\n";

    int n=10;

    vector x(n),y(n);

    for(int i=0;i<n;i++){
        x[i]=i;
        y[i]=std::cos(x[i]);
    }

    std::ofstream file("linear.dat");

    for(double z=0;z<=9;z+=0.1){

        double s=linterp(x,y,z);
        double integ=linterp_integral(x,y,z);

        file<<z<<" "<<s<<" "<<integ<<"\n";
    }

    file.close();
}

//Question B – (Quadratic spline)
void questionB(){

    std::cout<<"Question B: Quadratic spline\n\n";
    std::cout<<" Look at .svg files for plots pretaining to question B\n\n";
    std::cout<<" Includes a fig_quadratic.svg for answer to B.\n\n";

    int n=10;

    vector x(n),y(n);

    for(int i=0;i<n;i++){

        x[i]=i;
        y[i]=std::sin(x[i]);
    }

    qspline S(x,y);

    std::ofstream file("quadratic.dat");

    for(double z=0;z<=9;z+=0.1){

        file<<z<<" "
            <<S.evaluate(z)<<" "
            <<S.derivative(z)<<" "
            <<S.integral(z)<<"\n";
    }

    file.close();
}
// Questrion C – (Cubic spline)

void questionC(){

    std::cout<<"Question C: Cubic spline\n\n";
    std::cout<<" Look at .svg files for plots pretaining to question C\n\n";
    std::cout<<" Includes a fig_cubic.svg and a fig_total.svg for comparison of all splines.\n\n";

    int n=10;

    vector x(n),y(n);

    for(int i=0;i<n;i++){
        x[i]=i;
        y[i]=std::sin(x[i]);
    }

    cspline S(x,y);

    std::ofstream file("cubic.dat");

    for(double z=0;z<=9;z+=0.1){

        file<<z<<" "
            <<S.evaluate(z)<<" "
            <<S.derivative(z)<<" "
            <<S.integral(z)<<"\n";
    }

    file.close();
}

//Main runs each question in turn, writing results to files for plotting with gnuplot or similar tools.
int main(){

    questionA();
    questionB();
    questionC();
}