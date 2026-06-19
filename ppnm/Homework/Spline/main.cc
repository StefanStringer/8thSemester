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
    
    int n=10;
    vector x(n), y(n);

    // Generate sin(x) data
    for(int i=0;i<n;i++){
        x[i]=i;
        y[i]=std::sin(x[i]);
    }

    qspline S(x,y);

    std::ofstream file("quadratic.dat");
    for(double z=0;z<=9;z+=0.1){
        file<<z<<" " <<S.evaluate(z)<<" " <<S.derivative(z)<<" " <<S.integral(z)<<"\n";
    }
    file.close();

    // --- DEBUG TEST CASES WITH VERIFICATION ---
    std::cout << "Verification Test Cases:\n";
    std::cout << "--------------------------\n";
    
    // Test 1: yi = 1 (Constant -> f(x)=1, f'(x)=0)
    {
        vector x1(5), y1(5);
        for(int i=0;i<5;i++){x1[i]=i+1;y1[i]=1;}
        qspline S1(x1,y1);
        
        double err_b = std::abs(S1.b[0] - 0.0);
        double err_c = std::abs(S1.c[0] - 0.0);
        
        std::cout << "Test yi=1 (Expected b=0, c=0):\n";
        std::cout << "  Calculated: b=" << S1.b[0] << ", c=" << S1.c[0] << "\n";
        std::cout << "  Error: " << err_b << " (b), " << err_c << " (c)\n";
        std::cout << (err_b < 1e-8 && err_c < 1e-8 ? "  PASS\n\n" : "  FAIL\n\n");
    }
    
    // Test 2: yi = xi (Linear -> f(x)=x, f'(x)=1)
    {
        vector x2(5), y2(5);
        for(int i=0;i<5;i++){x2[i]=i+1;y2[i]=i+1;}
        qspline S2(x2,y2);
        
        double err_b = std::abs(S2.b[0] - 1.0);
        double err_c = std::abs(S2.c[0] - 0.0);
        
        std::cout << "Test yi=xi (Expected b=1, c=0):\n";
        std::cout << "  Calculated: b=" << S2.b[0] << ", c=" << S2.c[0] << "\n";
        std::cout << "  Error: " << err_b << " (b), " << err_c << " (c)\n";
        std::cout << (err_b < 1e-6 && err_c < 1e-8 ? "  PASS\n\n" : "  FAIL\n\n");
    }
    
    // Test 3: yi = xi^2 (Quadratic -> f(x)=x^2, f'(x)=2x)
    {
        vector x3(5), y3(5);
        for(int i=0;i<5;i++){x3[i]=i+1;y3[i]=(i+1)*(i+1);}
        qspline S3(x3,y3);
        
        double err = 0;
        for(int k=0; k<5; k++) {
            err += std::abs(S3.evaluate(x3[k]) - y3[k]);
        }
        
        std::cout << "Test yi=xi^2 (Reconstruction Error at nodes):\n";
        std::cout << "  Sum of absolute errors at nodes: " << err << "\n";
        std::cout << (err < 1e-8 ? "  PASS\n\n" : "  FAIL\n\n");
    }

    std::cout << "Plot 'fig_quadratic.svg' shows the spline fit.\n";
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