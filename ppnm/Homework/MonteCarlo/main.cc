#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <functional>
#include <iomanip>
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
    for(int i=0;i<dim;i++)
        V *= (b[i] - a[i]);

    //this is how er generate randome values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

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
    double variance = (sum2/N - mean*mean);
    double sigma = (variance > 0) ? sqrt(variance) : 0.0;

    return {mean*V, sigma*V/sqrt(N)};
}

void QuestionA() {
    cout << "========== Question A: Unit Circle Area ==========\n\n";
    
    // Function: Indicator for unit circle (x^2 + y^2 <= 1)
    auto f_circle = [](lineq::vector x) -> double {
        if (x[0] * x[0] + x[1] * x[1] <= 1.0) 
            return 1.0;
        else 
            return 0.0;
    };

    // Integration bounds: [-1, 1] x [-1, 1]
    lineq::vector a = {-1.0, -1.0};
    lineq::vector b = {1.0, 1.0};
    
    double exact = M_PI;

    cout << "Target: Area of unit circle\n";
    cout << "Exact value (PI) = " << fixed << setprecision(8) << exact << "\n\n";
    
    cout << " N          Result        Error    Actual Error\n";
    cout << "------------------------------------------------------\n";

    // Open file for plotting data (separate per question)
    ofstream data_file("data_A.txt");
    data_file << "# N Estimated_Error Actual_Error\n";

    // Run for N = 1000, 2000, 4000 ... 1,000,000
    for (int N = 1000; N <= 1000000; N *= 2) {
        auto [result, est_error] = plainmc(f_circle, a, b, N);
        double actual_error = fabs(result - exact);

        cout << setw(6) << N << "  "
             << setw(12) << fixed << setprecision(6) << result << "  "
             << setw(12) << est_error << "  "
             << setw(12) << actual_error << "\n";

        // Write to data file for gnuplot
        data_file << N << " " << est_error << " " << actual_error << "\n";
    }

    data_file.close();
    
    cout << "\n------------------------------------------------------\n";
    cout << "Data saved to 'data_A.txt'\n";
}


int main(){
    QuestionA();

    return 0;
}