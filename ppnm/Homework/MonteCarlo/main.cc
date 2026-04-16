#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <functional>
#include "matrix.hpp"

using namespace std;
using namespace lineq;

// Monte Carlo integrator
pair<double,double> plainmc(
    function<double(lineq::vector)> f,
    lineq::vector a,
    lineq::vector b,
    int N){
    int dim = a.size();
    double V = 1;
    for(int i=0;i<dim;i++) V *= (b[i]-a[i]);

    //this is how er generate randome values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0,1.0);

    double sum=0, sum2=0;
    lineq::vector x(dim);

    for(int i=0;i<N;i++){
        for(int k=0;k<dim;k++)
            x[k] = a[k] + dist(gen)*(b[k]-a[k]);

        double fx = f(x);
        sum += fx;
        sum2 += fx*fx;
    }

    double mean = sum/N;
    double sigma = sqrt(sum2/N - mean*mean);

    return {mean*V, sigma*V/sqrt(N)};
}

int main(){
    ofstream data("data.txt");
    std::cout << "---------- Part A ----------\n\n";
    // --- Test: area of unit circle ---
    auto f_circle = [](lineq::vector x){
        if(x[0]*x[0] + x[1]*x[1] <= 1.0) return 1.0;
        else return 0.0;
    };

    lineq::vector a = {-1,-1};
    lineq::vector b = {1,1};

    double exact = M_PI;

    cout << "Monte Carlo integration (unit circle area)\n";
    cout << "Exact value = " << exact << "\n\n";

    cout << " N        result        est_error     actual_error\n";

    for(int N = 1000; N <= 1000000; N *= 2){
        auto [res, err] = plainmc(f_circle, a, b, N);
        double actual_error = fabs(res - exact);

        cout << N << "  "
             << res << "  "
             << err << "  "
             << actual_error << "\n";

        data << N << " " << err << " " << actual_error << "\n";
    }

    cout << "\nData written to data.txt\n";
    cout << "Plot will be generated as error.svg\n";

    return 0;
}