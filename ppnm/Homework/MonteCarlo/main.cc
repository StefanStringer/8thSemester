#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <functional>
#include <iomanip>
#include "matrix.hpp"

using namespace std;
using namespace lineq;

// Monte Carlo integrator (super plain)
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

//Here is the Halton Sequence from doc method
double halton(int index, int base) {
    double result = 0.0;
    double fraction = 1.0/base;
    int i = index;

    while ( i > 0 ) {
        result += fraction * (i % base);
        i /= base;
        fraction /= base;
    }
    return result;
}
//monte carlo using halton sequence (Quasi random hence QR)
pair<double, double> plainmc_qr(
    function<double(lineq::vector)> f,
    lineq::vector a,
    lineq::vector b,
    int N,
    const std::vector<int>& bases) {
    
    int dim = a.size();
    double V = 1;
    for(int i = 0; i < dim; i++) V *= (b[i] - a[i]);

    double sum = 0, sum2 = 0;
    lineq::vector x(dim);

    // Generate N points
    for(int i = 1; i <= N; i++) {
        for(int k = 0; k < dim; k++) {
            // Map Halton points from [0,1] to [a[k], b[k]]
            double h_val = halton(i, bases[k]);
            x[k] = a[k] + h_val * (b[k] - a[k]);
        }

        double fx = f(x);
        sum += fx;
        sum2 += fx * fx;
    }

    double mean = sum / N;

    return {mean * V, 0.0}; 
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

void QuestionB() {
    cout << "========== Question B: Halton Sequence Integration ==========\n\n";
    
    // Function: Indicator for unit circle (x^2 + y^2 <= 1)
    auto f_circle_b = [](lineq::vector x) -> double {
        return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    };

    lineq::vector a = {0.0, 0.0, 0.0};
    lineq::vector b = {1.0, 1.0, 1.0};
    double exact_val = 1.0;

    cout <<"Exact value: " << exact_val << "\n\n";

    cout << " N          PRNG Result   PRNG EstErr   QR Result     QR ActErr\n";
    cout << "------------------------------------------------------------------\n";

    // Open file for plotting data (separate per question)
    ofstream data_b("data_B.txt");
    data_b << "# N PRNG_Result PRNG_EstErr QR_Result QR_ActErr\n";

    // Use first few prime numbers as bases for Halton sequence
    std::vector<int> bases1 = {2, 3, 5};
    std::vector<int> bases2 = {7, 11, 13};

    // Run for N = 1000, 2000, 4000 ... 1,000,000
    for (int N = 1000; N <= 1000000; N *= 2) {
        // 1. Pseudo-Random Run
        auto [res_pr, err_pr] = plainmc(f_circle_b, a, b, N);
        
        // 2. Quasi-Random Run (Sequence 1)
        auto [res_qr1, _] = plainmc_qr(f_circle_b, a, b, N, bases1);
        
        // 3. Quasi-Random Run (Sequence 2) - Used for error estimation
        auto [res_qr2, __] = plainmc_qr(f_circle_b, a, b, N, bases2);

        // Calculate Actual Errors (since we know the exact answer)
        double act_err_pr = fabs(res_pr - exact_val);
        double act_err_qr = fabs(res_qr1 - exact_val);
        
        // Estimate QR Error by comparing two sequences (as requested)
        double est_err_qr = fabs(res_qr1 - res_qr2);

        cout << setw(6) << N << "  "
             << setw(12) << fixed << setprecision(6) << res_pr << "  "
             << setw(12) << err_pr << "  "
             << setw(12) << res_qr1 << "  "
             << setw(12) << act_err_qr << "\n";

        // Store data for plotting: N, PRNG_Error, QR_Error
        data_b << N << " " << act_err_pr << " " << act_err_qr << "\n";
    }

    data_b.close();
    
    cout << "\n---------------------------------------------\n";
    cout << "Data saved to 'data_B.txt'\n";
}



int main(){
    cout << "====================================================\n";
    cout << "Monte Carlo Integration\n";
    cout << "====================================================\n\n";

    QuestionA();
    QuestionB();
    //QuestionC(); idk if ill get it done??

    cout << "====================================================\n";
    cout << "Finished :)\n";
    cout << "====================================================\n\n";

    return 0;
}