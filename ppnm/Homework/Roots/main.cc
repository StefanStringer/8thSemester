#include "matrix.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <iomanip>

using namespace Eigenvalues;

//here is the jacobian
matrix jacobian(std::function<vector(vector)> f, vector x, vector fx = vector(0), vector dx = vector(0)) {
    int n = x.size();
    if (dx.size() == 0) {
        dx = vector(n);
        for (int i = 0; i < n; i++) {
            dx[i] = std::abs(x[i]) * std::pow(2.0, -26);
            if (dx[i] == 0) dx[i] = std::pow(2.0, -26);
        }
    }
    if (fx.size() == 0) fx = f(x);

    matrix J(n, n);
    for (int j = 0; j < n; j++) {
        x[j] += dx[j];
        vector df = f(x) - fx;
        for (int i = 0; i < n; i++) J(i, j) = df[i] / dx[j];
        x[j] -= dx[j];
    }
    return J;
}

//The QR Decomp thing
struct QRResult {
    matrix Q, R;
    vector solve(const vector &b) {
        vector y = Q.transpose() * b;
        vector x(b.size());
        for (int i = b.size() - 1; i >= 0; i--) {
            double sum = 0;
            for (int j = i + 1; j < b.size(); j++) sum += R(i, j) * x[j];
            if (std::abs(R(i, i)) < 1e-12) throw std::runtime_error("Single matrix in Q R");
            x[i] = (y[i] - sum) / R(i, i);
        }
        return x;
    }
};

QRResult qr_decomposition(matrix A) {
    int m = A.size1();
    int n = A.size2();
    matrix Q(m, m);
    Q.setid();
    matrix R = A;

    for (int k = 0; k < n; k++) {
        vector col(n - k);
        for (int i = 0; i < n; i++) col[i] = R(k + i, k);

        double norm_col = 0;
        for(double val : col.data) norm_col += val * val;
        norm_col = std::sqrt(norm_col);

        double sign = (col[0] >= 0) ? 1.0 : -1.0;
        double alpha = sign * norm_col;

        vector v(n - k);
        v[0] = col[0] + alpha;
        for (int i = 1; i < n - k; i++) v[i] = col[i];

        double v_norm_squared = 0;
        for(double val : v.data) v_norm_squared += val * val;

        if(v_norm_squared < 1e-12) continue;

        for (int i = k; i < m; i++) {
            double dot_product = 0;
            for (int j = k; j < n; j++) dot_product += v[j - k] * R(i, j);
            double factor = 2.0 * dot_product / v_norm_squared;
            for (int j = k; j < n; j++) R(i, j) -= factor * v[j - k];
        }

        for (int i = 0; i < m; i++) {
            double dot_product = 0;
            for (int j = k; j < m; j++) dot_product += v[j - k] * Q(i, j);
            double factor = 2.0 * dot_product / v_norm_squared;
            for (int j = k; j < m; j++) Q(i, j) -= factor * v[j - k];
        }
    }
    return {Q, R};
}
// nbeed newtons method
vector newton(std::function<vector(vector)> f, vector start, double acc = 1e-6, vector dx = vector(0)) {
    vector x = start;
    vector fx = f(x);
    double lambda_min = 1e-10;
    int max_iter = 100;
    int iter = 0;

    if (dx.size() == 0) {
        dx = vector(x.size());
        for (int i = 0; i < x.size(); i++) {
            dx[i] = std::abs(x[i]) * std::pow(2.0, -26);
            if (dx[i] == 0) dx[i] = std::pow(2.0, -26);
        }
    }

    while (iter < max_iter) {
        if (fx.norm() < acc) break;

        matrix J = jacobian(f, x, fx, dx);
        QRResult qr = qr_decomposition(J);
        vector delta = qr.solve(-fx);

        double step_norm = delta.norm();
        double delta_norm = dx.norm();
        if (step_norm < delta_norm * 1e-3) break;

        double lambda = 1.0;
        vector z;
        vector fz;

        while (true) {
            z = x + delta * lambda;
            fz = f(z);
            if (fz.norm() < (1.0 - lambda / 2.0) * fx.norm()) break;
            if (lambda < lambda_min) break;
            lambda /= 2.0;
        }
        x = z;
        fx = fz;
        iter++;
    }
    return x;
}


//We first do the rosenbrock function gradient for question A using updated matrix.hpp

vector rosenbrock_gradient(vector x) {
    double xi = x[0];
    double yi = x[1];

    double term = yi - xi*xi;

    double grad_x = -2.0 * (1.0 - xi) - 400.0 * xi * term;
    double grad_y = 200.0 * term;
    
    vector grad(2);
    grad[0] = grad_x;
    grad[1] = grad_y;
    return grad;
}

//Here add for Question B
//ODE solver for the schrod eq

void solve_scrodinger(double E, double r_min, double r_max,
    std::vector<double>& r_values, std::vector<double>& f_values,
    double acc = 1e-8, double eps = 1e-10) {
    int n = 1000;
    double dr = (r_max - r_min) / n;
    r_values.resize(n + 1);
    f_values.resize(n + 1);

    for (int i = 0; i <= n; i++) {
        r_values[i] = r_min + i * dr;
        // f_values[i] = 0.0; // Initialize f_values to zero, will be updated in the loop
    }

    f_values[0] = r_min - r_min * r_min; // Initial condition f(r_min) = r_min - r_min^2

    double f_prime = 1.0 - 2.0 * r_min; // f'(r_min) = 1 - 2*r_min
    f_values[1] = f_values[0] + f_prime * dr; // Use the derivative to set the second value

    for (int i = 1; i < n; i++) {
        double r = r_values[i];
        double r_prev = r_values[i - 1];
        double r_next = r_values[i + 1];

        double k2 = 2.0 * (1.0 / r + E);
        double k2_prev = 2.0 * (1.0 / r_prev + E);
        double k2_next = 2.0 * (1.0 / r_next + E);

        // Numerov's method
        f_values[i + 1] = (2.0 * (1.0 - (5.0 * dr * dr * k2 / 12.0)) * f_values[i] - (1.0 + (dr * dr * k2_prev / 12.0)) * f_values[i - 1]) / (1.0 + (dr * dr * k2_next / 12.0));
        
        if (std::abs(f_values[i + 1]) > acc) {
            std::cerr << "Warning: Solution diverging at r = " << r << "\n";
            break;
        }
    }
}

//shooting function? the M(E) = f_E(r_max)
double shooting_function(double E, double r_min, double r_max, double acc = 1e-8, double eps = 1e-10) {
    std::vector<double> r_values, f_values;
    solve_scrodinger(E, r_min, r_max, r_values, f_values, acc, eps);
    return f_values.back(); // Return f_E(r_max)
}

// Bisection method for finding root of M(E) = 0
double find_ground_state_energy(double r_min, double r_max, 
                                double E_low = -1.0, double E_high = 0.0,
                                double tol = 1e-8) {
    double M_low = shooting_function(E_low, r_min, r_max);
    double M_high = shooting_function(E_high, r_min, r_max);
    
    if (M_low * M_high > 0) {
        throw std::runtime_error("Root not bracketed! Try different energy bounds.");
    }
    
    int max_iter = 100;
    for (int iter = 0; iter < max_iter; iter++) {
        double E_mid = (E_low + E_high) / 2.0;
        double M_mid = shooting_function(E_mid, r_min, r_max);
        
        if (std::abs(M_mid) < tol || (E_high - E_low) < tol) {
            return E_mid;
        }
        
        if (M_low * M_mid < 0) {
            E_high = E_mid;
            M_high = M_mid;
        } else {
            E_low = E_mid;
            M_low = M_mid;
        }
    }
    
    throw std::runtime_error("Bisection did not converge?");
}

//placing the optimised newton version under here, so the old one remains
inline double calc_phi(const vector &fx) {
    return 0.5 * fx.norm() * fx.norm();
}


vector newton_optimised(std::function<vector(vector)> f, vector start, double acc = 1e-6, vector dx = vector(0)) {
    vector x = start;
    vector fx = f(x);
    
    int n = x.size();
    // Allocate Jacobian ONCE outside the loop
    matrix J(n, n); 
    
    double lambda_min = 1e-10;
    int max_iter = 100;
    int iter = 0;

    // Initialize dx if not provided
    if (dx.size() == 0) {
        dx = vector(n);
        for (int i = 0; i < n; i++) {
            dx[i] = std::abs(x[i]) * std::pow(2.0, -26);
            if (dx[i] == 0) dx[i] = std::pow(2.0, -26);
        }
    }

    double phi_0 = calc_phi(fx);
    double dphi_0 = -2.0 * phi_0; // Derivative of phi at lambda=0

    while (iter < max_iter) {
        if (fx.norm() < acc) break;

        // 1. Compute Jacobian (reuses allocated memory)
        J = jacobian(f, x, fx, dx);

        // 2. Solve J * delta = -fx
        QRResult qr = qr_decomposition(J);
        vector delta = qr.solve(-fx);

        // Check for convergence based on step size
        if (delta.norm() < dx.norm() * 1e-3) break;

        // 3. Quadratic Interpolation Line Search
        double lambda = 1.0;
        vector z;
        vector fz;

        while (true) {
            z = x + delta * lambda;
            fz = f(z);
            double phi_new = calc_phi(fz);

            // Armijo condition
            if (fz.norm() < (1.0 - lambda / 2.0) * fx.norm()) {
                break; // Step accepted
            }

            if (lambda < lambda_min) {
                break; // Accept bad step if too small
            }

            // Quadratic Interpolation
            double c = (phi_new - phi_0 - dphi_0 * lambda) / (lambda * lambda);
            double lambda_next;

            if (std::abs(c) < 1e-15 || c <= 0) {
                lambda_next = lambda / 2.0; // Fallback to halving
            } else {
                lambda_next = -dphi_0 / (2.0 * c);
            }

            // Safety: Ensure step size decreases
            if (lambda_next >= lambda) {
                lambda_next = lambda / 2.0;
            }

            lambda = lambda_next;
        }

        x = z;
        fx = fz;
        iter++;
    }
    return x;
}

void QuestionA() {
    std::cout << "######### Question A: Newton's Method on Rosenbrock Function #########\n\n";
    
//debug part checker
    std::cout << " debug with 1d equation (x^2 - 4 = 0) \n";
    vector start_1d({1.0});
    vector root_1d = newton([](vector x) { return vector({x[0]*x[0] - 4}); }, start_1d, 1e-8);
    std::cout << "starting at: (" << start_1d[0] << ")\n";
    std::cout << "Found root at: (" << root_1d[0] << ")\n";
    std::cout << "Expected root: (2.0)\n";
    std::cout << "Function value at root: " << (root_1d[0]*root_1d[0] - 4) << "\n\n";

// rosenbrocks valley function
    std::cout << "Finding roots of the gradient of the Rosenbrock function:\n";
    std::cout << "f(x,y) = (1-x)^2 + 100*(y-x^2)^2\n";

    std::vector<std::pair<double, double>> starts = {
        {-1.2, 1.0}, 
        {0.0, 0.0},  
        {2.0, 2.0}   
    };

    for (auto& p : starts) {
        vector start_vec({p.first, p.second});
        std::cout << "Starting at: (" << p.first << ", " << p.second << ")\n";
        
        vector root = newton(rosenbrock_gradient, start_vec, 1e-8);
        
        std::cout << "Found root at: (" << root[0] << ", " << root[1] << ")\n";
        std::cout << "Gradient norm at root: " << rosenbrock_gradient(root).norm() << "\n";
        
        // Calculate function value
        double val = std::pow(1.0 - root[0], 2) + 100.0 * std::pow(root[1] - root[0]*root[0], 2);
        std::cout << "Function value f(x,y): " << val << "\n\n";
    }

// Himmelblaus function part
    std::cout << "Finding roots of the gradient of the Himmelblau's function:\n";
    std::cout << "f(x,y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2\n";

    std::vector<std::pair<double, double>> himmel_starts = {
        {3.0, 2.0},  
        {-2.805118, 3.131312}, 
        {-3.779310, -3.283186}, 
        {3.584428, -1.848126}  
    };
    
    for (auto& p : himmel_starts) {
        vector start_vec({p.first, p.second});
        std::cout << "Starting at: (" << p.first << ", " << p.second << ")\n";
        
        vector root = newton([](vector x) {
            double xi = x[0];
            double yi = x[1];
            double grad_x = 4.0 * (xi*xi + yi - 11) * xi + 2.0 * (xi + yi*yi - 7);
            double grad_y = 2.0 * (xi*xi + yi - 11) + 4.0 * (xi + yi*yi - 7) * yi;
            return vector({grad_x, grad_y});
        }, start_vec, 1e-8);
        
        std::cout << "Found root at: (" << root[0] << ", " << root[1] << ")\n";
        std::cout << "Gradient norm at root: " << rosenbrock_gradient(root).norm() << "\n";
        
        // Calculate function value
        double val = std::pow(root[0]*root[0] + root[1] - 11, 2) + std::pow(root[0] + root[1]*root[1] - 7, 2);
        std::cout << "Function value f(x,y): " << val << "\n\n";
    }
}

void QuestionB() {
    std::cout << "######### Question B: Hydrogen Atom Bound States #########\n\n";
    
    double r_min = 1e-6;
    double r_max = 8.0;
    double acc = 1e-8;
    double eps = 1e-10;

    std::cout << "Finding ground state energy:\n";
    std::cout << "r_min = " << r_min << ", r_max = " << r_max << "\n";
    
    try {
        double E0 = find_ground_state_energy(r_min, r_max);

        std::cout << "Estimated ground state energy E0: " << E0 << "\n";
        std::cout << "Expected ground state energy for hydrogen atom: -0.5\n";

        std::vector<double> r_values, f_values;
        solve_scrodinger(E0, r_min, r_max, r_values, f_values, acc, eps);

        //Get the wavefunctions
        std::cout << "Wavefunction values at selected r:\n";
        for (size_t i = 0; i < r_values.size(); i += r_values.size() / 10) {
            std::cout << "r = " << r_values[i] << ", f(r) = " << f_values[i] << "\n";
        }

        //convergence check
        std::cout << "Checking convergence of the solution:\n";
        

        // Vary r_max
        std::cout << "\nEffect of r_max:\n";
        std::cout << std::setw(10) << "r_max" << std::setw(15) << "E0" 
                  << std::setw(15) << "Error\n";
        for (double rm : {4.0, 6.0, 8.0, 10.0, 12.0}) {
            double E_test = find_ground_state_energy(r_min, rm, -1.0, 0.0, 1e-8);
            std::cout << std::setw(10) << rm 
                      << std::setw(15) << std::setprecision(8) << E_test
                      << std::setw(15) << std::abs(E_test - (-0.5)) << "\n";
        }
        
        // Vary r_min
        std::cout << "\nEffect of r_min:\n";
        std::cout << std::setw(10) << "r_min" << std::setw(15) << "E0" 
                  << std::setw(15) << "Error\n";
        for (double rm : {1e-2, 1e-4, 1e-6, 1e-8}) {
            double E_test = find_ground_state_energy(rm, r_max, -1.0, 0.0, 1e-8);
            std::cout << std::setw(10) << rm 
                      << std::setw(15) << std::setprecision(8) << E_test
                      << std::setw(15) << std::abs(E_test - (-0.5)) << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error in Question B: " << e.what() << "\n";
    }
    
}

void QuestionC() {
    std::cout << "######### Question C: Optimised Newton with Quadratic Line Search #########\n\n";
    
    // Test on Rosenbrock again
    std::cout << "Testing optimised Newton on Rosenbrock:\n";
    vector start({-1.2, 1.0});
    
    vector root = newton_optimised(rosenbrock_gradient, start, 1e-8);
    
    std::cout << "Found root at: (" << root[0] << ", " << root[1] << ")\n";
    std::cout << "Gradient norm: " << rosenbrock_gradient(root).norm() << "\n";
    std::cout << "Function value: " << (std::pow(1.0 - root[0], 2) + 100.0 * std::pow(root[1] - root[0]*root[0], 2)) << "\n";
    
}

int main() {
    std::ofstream outFile("Out.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create Out.txt\n";
        return 1;
    }
    std::streambuf* old = std::cout.rdbuf(outFile.rdbuf());
    
    std::cout << " ################################################################# \n";
    std::cout << "                            ROOTS                           \n";
    
    std::cout << " Question A: Newton's Method on Rosenbrock Function \n";
    QuestionA();

    std::cout << " Question B: ODE Solver for the Schrödinger Equation \n";
    QuestionB();
    
    std::cout << " Question C: Optimized Newton with Quadratic Line Search \n";
    QuestionC();
    std::cout << " ################################################################# \n";
    

    std::cout.rdbuf(old);
    outFile.close();
    
    std::cout << "Results written to Out.txt\n";
    return 0;
}