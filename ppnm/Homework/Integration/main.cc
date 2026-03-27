#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>
#include <vector>
#include <iomanip>
#include <string>

using Func = std::function<double(double)>;

// Global counter for function calls
int eval_count = 0;
Func wrap_counter(Func f) {
    return [f](double x) { eval_count++; return f(x); };
}

// Recursive adaptive open 4-point integrator
std::pair<double,double> integrate(Func f, double a, double b,
                                   double acc=1e-4, double eps=1e-4,
                                   double f2=std::nan("1"), double f3=std::nan("1"),
                                   int depth=0) {
    if(depth > 1000) return {0, 1e10}; // prevent infinite recursion

    double h = b-a;
    if(std::isnan(f2)) { f2 = f(a + 2*h/6); f3 = f(a + 4*h/6); }

    double f1 = f(a + h/6);
    double f4 = f(a + 5*h/6);

    double Q = (2*f1 + f2 + f3 + 2*f4)/6*h;
    double q = (f1 + f2 + f3 + f4)/4*h;

    double err = std::abs(Q - q);
    double tol = acc + eps*std::abs(Q);

    if(err <= tol) return {Q, err};

    double mid = (a+b)/2;
    auto left  = integrate(f, a, mid, acc/std::sqrt(2), eps, f1, f2, depth+1);
    auto right = integrate(f, mid, b, acc/std::sqrt(2), eps, f3, f4, depth+1);

    double total = left.first + right.first;
    double total_err = std::sqrt(left.second*left.second + right.second*right.second);
    return {total, total_err};
}

// Clenshaw–Curtis transformation + infinite interval support
std::pair<double,double> integrate_cc(Func f, double a, double b,
                                      double acc=1e-4, double eps=1e-4) {
    // Infinite limits
    if(a == -INFINITY && b == INFINITY) {
        auto g = [&](double t){ return f(std::tan(M_PI*(t-0.5))) * M_PI / std::pow(std::cos(M_PI*(t-0.5)),2); };
        return integrate(g, 0.0, 1.0, acc, eps);
    }
    if(a == -INFINITY && std::isfinite(b)) {
        auto g = [&](double t){ return f(b - (1-t)/t) / (t*t); };
        return integrate(g, 0.0, 1.0, acc, eps);
    }
    if(std::isfinite(a) && b == INFINITY) {
        auto g = [&](double t){ return f(a + (1-t)/t) / (t*t); };
        return integrate(g, 0.0, 1.0, acc, eps);
    }

    // Finite interval: apply Clenshaw–Curtis
    double mid  = (a+b)/2;
    double half = (b-a)/2;
    auto g = [&](double t){ return f(mid + half*std::cos(t)) * std::sin(t) * half; };
    return integrate(g, 0.0, M_PI, acc, eps);
}

// Error function using the adaptive integrator
double my_erf(double z, double acc=1e-6, double eps=1e-6) {
    if(z < 0) return -my_erf(-z, acc, eps);
    if(z <= 1) {
        auto f = [](double x){ return std::exp(-x*x); };
        auto [res,err] = integrate(f, 0, z, acc, eps);
        return 2/std::sqrt(M_PI)*res;
    } else {
        auto f = [z](double t){ double x = z + (1-t)/t; return std::exp(-x*x)/(t*t); };
        auto [res,err] = integrate(f, 0, 1, acc, eps);
        return 1 - 2/std::sqrt(M_PI)*res;
    }
}

// Simple linspace
std::vector<double> linspace(double start, double stop, int num) {
    std::vector<double> v(num);
    double step = (stop-start)/(num-1);
    for(int i=0;i<num;i++) v[i] = start + i*step;
    return v;
}

int main() {
    std::cout << std::fixed << std::setprecision(8);

    std::cout << "=== PART A: Basic Integrals & Error Function ===\n";
    // Functions
    auto fA = [](double x){ return std::sqrt(x); };
    auto fB = [](double x){ return 1/std::sqrt(x); };
    auto fC = [](double x){ return std::sqrt(1-x*x); };
    auto fD = [](double x){ return std::log(x)/std::sqrt(x); };
    double a=0, b=1;

    eval_count=0;
    auto [I_A,eA] = integrate(fA,a,b);
    std::cout << "sqrt(x) | Integral = " << I_A << " | EstErr = " << eA << "\n";

    eval_count=0;
    auto [I_B,eB] = integrate_cc(fB,a,b);
    std::cout << "1/sqrt(x) | Integral = " << I_B << " | EstErr = " << eB << "\n";

    eval_count=0;
    auto [I_C,eC] = integrate(fC,a,b);
    std::cout << "sqrt(1-x^2) | Integral = " << I_C << " | EstErr = " << eC << "\n";

    eval_count=0;
    auto [I_D,eD] = integrate_cc(fD,a,b);
    std::cout << "ln(x)/sqrt(x) | Integral = " << I_D << " | EstErr = " << eD << "\n";

    std::cout << "\nError function tests:\n";
    for(double z=-1.0; z<=2.0; z+=0.5) {
        double val = my_erf(z);
        std::cout << "my_erf(" << z << ") = " << val << "\n";
    }

    // Save data for plotting erf(z)
    std::ofstream f1("erf.svg.dat");
    auto zs = linspace(-3,3,200);
    for(auto z : zs) f1 << z << " " << my_erf(z) << "\n";
    f1.close();

    // Save accuracy vs acc for erf(1)
    std::ofstream f2("erf_acc.svg.dat");
    double acc = 0.1;
    double exact = 0.84270079294971486934;
    for(int i=0;i<7;i++){
        double val = my_erf(1.0,0.0,acc);
        f2 << acc << " " << val << " " << std::abs(val-exact) << "\n";
        acc /= 10;
    }
    f2.close();

    std::cout << "\n=== PART B: Clenshaw-Curtis & Infinite Limits ===\n";

    eval_count=0;
    auto [CC_A,eCC_A] = integrate_cc(fA,a,b);
    std::cout << "sqrt(x) | CC = " << CC_A << " | Plain = " << I_A << "\n";

    eval_count=0;
    auto [CC_B,eCC_B] = integrate_cc(fB,a,b);
    std::cout << "1/sqrt(x) | CC = " << CC_B << " | Plain = " << I_B << "\n";

    eval_count=0;
    int ncalls=0;
    auto fcount = [&ncalls](double x){ ncalls++; return x*x; };
    auto [I_f, e_f] = integrate_cc(fcount,0,1);
    std::cout << "z^2 integral 0..1 = " << I_f << " | Function calls = " << ncalls << " | Scipy ~21\n";

    ncalls=0;
    auto g = [&ncalls](double x){ ncalls++; return std::exp(-x*x); };
    auto [I_g, e_g] = integrate_cc(g,-INFINITY,INFINITY);
    std::cout << "exp(-x^2) -inf..inf = " << I_g << " | Function calls = " << ncalls << " | Scipy ~277\n";

    std::cout << "\n=== PART C: Difficult Integrals ===\n";
    auto h1 = [](double x){ return std::sin(x)/std::exp(-x*x); };
    auto [I_h1,e_h1] = integrate_cc(h1,0,M_PI);
    std::cout << "int_0^pi sin(x)/exp(-x^2) = " << I_h1 << " | EstErr = " << e_h1 << "\n";

    auto h2 = [](double x){ return x/std::exp(x); };
    auto [I_h2,e_h2] = integrate_cc(h2,0,INFINITY);
    std::cout << "int_0^inf x/exp(x) = " << I_h2 << " | EstErr = " << e_h2 << "\n";

    std::cout << "\nDone. Data files: erf.svg.dat, erf_acc.svg.dat\n";

    return 0;
}