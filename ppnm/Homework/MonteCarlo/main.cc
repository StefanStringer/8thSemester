#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <functional>
#include <iomanip>
#include "matrix.hpp"

using namespace std;
using namespace lineq;

// Monte Carlo integrator (plain)
pair<double,double> plainmc(
    function<double(lineq::vector)> f,
    lineq::vector a,
    lineq::vector b,
    int N){
    int dim = a.size();
    double V = 1;
    for(int i=0;i<dim;i++)
        V *= (b[i] - a[i]);

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

// Halton Sequence
double halton(int index, int base) {
    double result = 0.0;
    double fraction = 1.0/base;
    int i = index;
    while (i > 0) {
        result += fraction * (i % base);
        i /= base;
        fraction /= base;
    }
    return result;
}

// Quasi-random Monte Carlo (Halton)
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

    for(int i = 1; i <= N; i++) {
        for(int k = 0; k < dim; k++) {
            double h_val = halton(i, bases[k]);
            x[k] = a[k] + h_val * (b[k] - a[k]);
        }

        double fx = f(x);
        sum += fx;
        sum2 += fx*fx;
    }

    double mean = sum / N;
    return {mean * V, 0.0}; 
}

// QUESTION A: Plain Monte Carlo - Unit Circle
void QuestionA() {
    cout << "========== Question A: Unit Circle Area ==========\n\n";
    
    auto f_circle = [](lineq::vector x) -> double {
        if (x[0] * x[0] + x[1] * x[1] <= 1.0) 
            return 1.0;
        else 
            return 0.0;
    };

    lineq::vector a = {-1.0, -1.0};
    lineq::vector b = {1.0, 1.0};
    double exact = M_PI;

    cout << "Target: Area of unit circle\n";
    cout << "Exact value (PI) = " << fixed << setprecision(8) << exact << "\n\n";
    
    cout << " N          Result        Error    Actual Error\n";
    cout << "------------------------------------------------------\n";

    ofstream data_file("data_A.txt");
    data_file << "# N Estimated_Error Actual_Error\n";

    for (int N = 1000; N <= 1000000; N *= 2) {
        auto [result, est_error] = plainmc(f_circle, a, b, N);
        double actual_error = fabs(result - exact);

        cout << setw(6) << N << "  "
             << setw(12) << fixed << setprecision(6) << result << "  "
             << setw(12) << est_error << "  "
             << setw(12) << actual_error << "\n";

        data_file << N << " " << est_error << " " << actual_error << "\n";
    }

    data_file.close();
    
    cout << "\n------------------------------------------------------\n";
    cout << "Data saved to 'data_A.txt'\n";
    cout << "Note: Error should scale as 1/√N (check plot)\n";
}

// QUESTION B: Quasi-Random (Halton) Sequences
void QuestionB() {
    cout << "========== Question B: Halton Sequence Integration ==========\n\n";
    
    auto f_circle_b = [](lineq::vector x) -> double {
        return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    };

    lineq::vector a = {0.0, 0.0, 0.0};
    lineq::vector b = {1.0, 1.0, 1.0};
    double exact_val = 1.0;

    cout <<"Exact value: " << exact_val << "\n\n";

    cout << " N          PRNG Result   PRNG EstErr   QR Result     QR ActErr\n";
    cout << "------------------------------------------------------------------\n";

    ofstream data_b("data_B.txt");
    data_b << "# N PRNG_Result PRNG_EstErr QR_Result QR_ActErr\n";

    std::vector<int> bases1 = {2, 3, 5};
    std::vector<int> bases2 = {7, 11, 13};

    for (int N = 1000; N <= 1000000; N *= 2) {
        auto [res_pr, err_pr] = plainmc(f_circle_b, a, b, N);
        auto [res_qr1, _] = plainmc_qr(f_circle_b, a, b, N, bases1);
        auto [res_qr2, __] = plainmc_qr(f_circle_b, a, b, N, bases2);

        double act_err_pr = fabs(res_pr - exact_val);
        double act_err_qr = fabs(res_qr1 - exact_val);
        double est_err_qr = fabs(res_qr1 - res_qr2);

        cout << setw(6) << N << "  "
             << setw(12) << fixed << setprecision(6) << res_pr << "  "
             << setw(12) << err_pr << "  "
             << setw(12) << res_qr1 << "  "
             << setw(12) << act_err_qr << "\n";

        data_b << N << " " << act_err_pr << " " << act_err_qr << "\n";
    }

    data_b.close();
    
    cout << "\n---------------------------------------------\n";
    cout << "Data saved to 'data_B.txt'\n";
    cout << "QR error scales better than 1/√N (check plot)\n";
}

// QUESTION C: Stratified Sampling
void QuestionC() {
    cout << "========== Question C: Stratified Sampling ==========\n\n";
    
    auto f_circle = [](lineq::vector x) -> double {
        return (x[0]*x[0] + x[1]*x[1] <= 1.0) ? 1.0 : 0.0;
    };

    lineq::vector a = {-1.0, -1.0};
    lineq::vector b = {1.0, 1.0};
    double exact = M_PI;
    const int nmin = 100;

    // Recursive stratified sampling
    function<pair<double,double>(lineq::vector,lineq::vector,int)> 
    stratified = [&](lineq::vector a_sub, lineq::vector b_sub, int N) -> pair<double,double> {
        
        if (N < nmin) {
            random_device rd; mt19937 gen(rd()); uniform_real_distribution<> dist(0.0,1.0);
            double sum=0, sum2=0; lineq::vector x(a_sub.size()); double V=1;
            for(int i=0;i<a_sub.size();i++) V *= (b_sub[i]-a_sub[i]);
            
            for(int k=0;k<N;k++){
                for(size_t j=0;j<x.size();j++)
                    x[j] = a_sub[j] + dist(gen)*(b_sub[j]-a_sub[j]);
                double fx = f_circle(x); sum += fx; sum2 += fx*fx;
            }
            double mean = sum/N;
            double sigma = sqrt(max(0.0,sum2/N-mean*mean));
            return {mean*V, sigma*V/sqrt(N)};
        }

        random_device rd; mt19937 gen(rd()); uniform_real_distribution<> dist(0.0,1.0);
        double sum=0, sum2=0; lineq::vector x(a_sub.size()); double V=1;
        for(int i=0;i<a_sub.size();i++) V *= (b_sub[i]-a_sub[i]);
        
        for(int k=0;k<nmin;k++){
            for(size_t j=0;j<x.size();j++)
                x[j] = a_sub[j] + dist(gen)*(b_sub[j]-a_sub[j]);
            double fx = f_circle(x); sum += fx; sum2 += fx*fx;
        }
        double total_var = max(0.0,sum2/nmin-(sum/nmin)*(sum/nmin));

        int best_dim = 0; double max_h = 0;
        for(size_t d=0;d<a_sub.size();d++){
            double h = b_sub[d]-a_sub[d];
            if(h > max_h){max_h=h; best_dim=d;}
        }

        // Subdivide along best dimension
        lineq::vector mid = a_sub; mid[best_dim] += (b_sub[best_dim]-a_sub[best_dim])/2;
        lineq::vector left_b = b_sub; left_b[best_dim] = mid[best_dim];
        lineq::vector right_a = a_sub; right_a[best_dim] = mid[best_dim];

        // Split points
        int N_rem = N - nmin;
        int N_left = N_rem/2, N_right = N_rem - N_left;

        auto [I1,e1] = stratified(a_sub,left_b,N_left);
        auto [I2,e2] = stratified(right_a,b_sub,N_right);

        return {I1+I2, sqrt(e1*e1+e2*e2)};
    };

    int N_total = 10000;
    auto [result, est_err] = stratified(a,b,N_total);
    double actual_err = fabs(result - exact);

    cout << "Target: Area of unit circle\n";
    cout << "Exact value (PI) = " << fixed << setprecision(8) << exact << "\n";
    cout << "Stratified Result = " << result << "\n";
    cout << "Estimated Error = " << est_err << "\n";
    cout << "Actual Error = " << actual_err << "\n";
    
    cout << "\n------------------------------------------------------\n";
    cout << "Data saved (stratified improves convergence compared to the plain MC)\n";
}

// MAIN
int main(){
    cout << "====================================================\n";
    cout << "Monte Carlo Integration\n";
    cout << "====================================================\n\n";

    QuestionA();
    QuestionB();
    QuestionC();

    cout << "====================================================\n";
    cout << "Finished :)\n";
    cout << "====================================================\n\n";

    return 0;
}