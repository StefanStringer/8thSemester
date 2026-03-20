//My imports
#include "matrix.hpp"
#include "qr.hpp"
//imports needed
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <cmath>
#include <tuple>
//the namespace copied again from eigenvalue homewrok
using namespace Eigenvalues;

/*------------------------------------------------------------
  Experimental data from Rutherford & Soddy (1902)
------------------------------------------------------------*/

const int n = 9;

double tdata[n]  ={1, 2, 3, 4, 6, 9, 10, 13, 15};
double ydata[n]  ={117, 100, 88, 72, 53, 29.5, 25.2, 15.2, 11.1};
double dydata[n] ={6, 5, 4, 4, 4, 3, 3, 2, 2};


/*============================================================
  QUESTION A
  Ordinary least squares fit (no uncertainties reported)
============================================================*/

void questionA(){

    std::cout << "=============================\n";
    std::cout << "Question A: Least Squares Fit\n";
    std::cout << "=============================\n\n";

    vector t(n),y(n),dy(n);

    // transform data for logarithmic fit
    // ln(y) = ln(a) - lambda * t
    for(int i=0;i<n;i++){

        t[i]=tdata[i];
        y[i]=std::log(ydata[i]);

        // uncertainty of ln(y)
        dy[i]=dydata[i]/ydata[i];
    }

    // basis functions for linear fit
    auto fs=std::vector<std::function<double(double)>>{
        [](double x){return 1.0;},
        [](double x){return x;}
    };

    // perform fit
    auto [c,Cov] = lsfit(fs,t,y,dy);

    double ln_a=c[0];
    double lambda=-c[1];
    double a=std::exp(ln_a);

    double T_half=std::log(2)/lambda;

    std::cout << "Experimental decay model:\n";
    std::cout << "y(t) = a * exp(-lambda * t)\n\n";

    std::cout << "Fitted parameters:\n";
    std::cout << "a      = " << a << "\n";
    std::cout << "lambda = " << lambda << "\n";
    std::cout << "Half-life = " << T_half << " days\n\n";

    // theoretical modern value
    std::cout << "Theoretical value is 3.66 days\n\n";
}


/*============================================================
  QUESTION B
  Fit including covariance matrix and uncertainties
============================================================*/

void questionB(){

    std::cout << "===========================================\n";
    std::cout << "Question B: Fit with Uncertainties\n";
    std::cout << "===========================================\n\n";

    vector t(n),y(n),dy(n);

    for(int i=0;i<n;i++){

        t[i]=tdata[i];
        y[i]=std::log(ydata[i]);
        dy[i]=dydata[i]/ydata[i];
    }

    auto fs=std::vector<std::function<double(double)>>{
        [](double x){return 1.0;},
        [](double x){return x;}
    };

    auto [c,Cov] = lsfit(fs,t,y,dy);

    double ln_a=c[0];
    double lambda=-c[1];

    double a=std::exp(ln_a);

    // coefficient uncertainties
    double d_ln_a=std::sqrt(Cov(0,0));
    double d_lambda=std::sqrt(Cov(1,1));

    double T_half=std::log(2)/lambda;

    // propagate uncertainty of half-life
    double dT_half=(std::log(2)/(lambda*lambda))*d_lambda;

    std::cout << "Fit coefficients:\n";
    std::cout << "ln(a)   = " << ln_a << " +/- " << d_ln_a << "\n";
    std::cout << "lambda  = " << lambda << " +/- " << d_lambda << "\n\n";

    std::cout << "Converted parameter:\n";
    std::cout << "a = " << a << "\n\n";

    std::cout << "Half-life = " << T_half
            << " +/- " << dT_half
            << " days\n\n";
}


/*============================================================
  QUESTION C
  Generate curves for uncertainty envelope
============================================================*/

void questionC(){

    std::cout << "===========================================\n";
    std::cout << "Question C: Plot uncertainty variations\n";
    std::cout << "===========================================\n\n";

    vector t(n),y(n),dy(n);

    for(int i=0;i<n;i++){

        t[i]=tdata[i];
        y[i]=std::log(ydata[i]);
        dy[i]=dydata[i]/ydata[i];
    }

    auto fs=std::vector<std::function<double(double)>>{
        [](double x){return 1.0;},
        [](double x){return x;}
    };

    auto [c,Cov] = lsfit(fs,t,y,dy);

    double ln_a=c[0];
    double lambda=-c[1];

    double d_ln_a=std::sqrt(Cov(0,0));
    double d_lambda=std::sqrt(Cov(1,1));

    double a=std::exp(ln_a);

    /*------------------------------------------
      Save experimental data for plotting
    ------------------------------------------*/

    std::ofstream data("fit_data.txt");

    for(int i=0;i<n;i++)
        data << tdata[i] << " "
             << ydata[i] << " "
             << dydata[i] << "\n";

    data.close();

    /*------------------------------------------
      Save fitted curves
    ------------------------------------------*/

    std::ofstream fit("fit_curve.txt");

    for(double x=0;x<=16;x+=0.1){

        double yfit=a*std::exp(-lambda*x);

        double y_plus=
            std::exp(ln_a+d_ln_a)*
            std::exp(-(lambda-d_lambda)*x);

        double y_minus=
            std::exp(ln_a-d_ln_a)*
            std::exp(-(lambda+d_lambda)*x);

        fit << x << " "
            << yfit << " "
            << y_plus << " "
            << y_minus << "\n";
    }

    fit.close();

    std::cout << "Data files written:\n";
    std::cout << "fit_data.txt\n";
    std::cout << "fit_curve.txt\n\n";
}


// was nicer to just write the main lie k this
int main(){

    questionA();
    questionB();
    questionC();

}