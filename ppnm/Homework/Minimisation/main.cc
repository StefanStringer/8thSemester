#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <iomanip>
#include "matrix.hpp"

using namespace Eigenvalues;

// def gradient(φ,x) :
//     φx = φ(x)
//     gφ = vector(len(x))
//     for i in range(len(x)) :
//         dxi = (1+abs(x[i]))*2**(-26) # δx~ε^(1/2)
//         x[i]+=dxi
//         gφ[i]=(φ(x)-φx)/dxi
//         x[i]-=dxi
//     return gφ

vector gradient(std::function<double(vector)> phi, vector x) {
    int n = x.size();
    vector g(n);
    for (int i = 0; i < n; i++) {
        double dxi = (1.0 + std::abs(x[i])) * std::pow(2.0, -26);
        vector x_plus = x;
        vector x_minus = x;
        x_plus[i] += dxi;
        x_minus[i] -= dxi;
        g[i] = (phi(x_plus) - phi(x_minus)) / (2.0 * dxi);
    }
    return g;
}

matrix hessian(std::function<double(vector)> phi, vector x) {
    int n = x.size();
    matrix H(n, n);
    
    for (int j = 0; j < n; j++) {
        double dxj = (1.0 + std::abs(x[j])) * std::pow(2.0, -10); // Changed from -13 to -10
        
        vector x_plus = x;
        vector x_minus = x;
        x_plus[j] += dxj;
        x_minus[j] -= dxj;
        
        vector g_plus = gradient(phi, x_plus);
        vector g_minus = gradient(phi, x_minus);
        
        for (int i = 0; i < n; i++) {
            H(i, j) = (g_plus[i] - g_minus[i]) / (2.0 * dxj);
        }
    }
    return H;
}
// def hessian(φ,x) :
//     H = matrix(len(x),len(x))
//     gφx = gradient(φ,x)
//     for j in range(len(x)) :
//         dxj=(1+abs(x[j]))*2**(-13) # δx~ε^(1/4), try also δx~ε^(1/3)
//         x[j]+=dxj
//         dgφ=gradient(φ,x)-gφx
//         for i in range(len(x)) : H[i,j]=dgφ[i]/dxj
//         x[j]-=dxj
// return H

//The QR Decomp thing whihc is from roots homework
struct QRResults {
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

QRResults qr_decomposition(matrix A) { //from previous roots homework
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

// def newton(phi, x, acc = 1e-3);
//     while True :                    // Newton iterations
//         g = gradient(phi,x)
//         if g.norm() < acc : break   // job done
//         H = hessian(phi,x)
//         for i in range(H.size1) : H[i,i]+=1e-6 // Levenberg regularization
//         dx = QRdecomposition(H).solve(-g)
//         lambda = 1 ;
//         while (lambda ≥ 1/1024);       // backtracking linesearch
//             if φ(x+lambda*dx) < φ(x) : break // good step
//             lambda /= 2
//         x=x+lambda*dx
//     return x

vector newton_minimise(std::function<double(vector)> phi, vector x, double acc = 1e-3, int max_iter = 2000) {
    int iter = 0;
    bool converged = false;
    double reg = 1e-6;

    while (iter < max_iter) {
        vector g = gradient(phi, x);
        double g_norm = g.norm();
        
        if (g_norm < acc) {
            converged = true;
            break;
        }

        matrix H = hessian(phi, x);

        int n = H.size1();
        for (int i = 0; i < n; i++) {
            H(i, i) += reg; 
        }

        QRResults qr = qr_decomposition(H);
        vector dx = qr.solve(g * -1.0);

        double lambda = 1.0;
        double phi_x = phi(x);
        int ls_fails = 0;
        
        while (lambda >= 1.0 / 1024.0) {
            vector x_new = x + dx * lambda;
            if (phi(x_new) < phi_x) {
                break;
            }
            lambda /= 2.0;
            ls_fails++;
        }

        if (ls_fails > 5) {
            reg *= 10.0;
            if (reg > 1.0) reg = 1.0;
        } else {
            if (reg > 1e-6) reg /=2.0;
        }
        x = x + dx * lambda;
        iter++;
    }

    // Print results in the requested format
    std::cout << "Converged: " << (converged ? "Yes" : "No (Max Iterations or Stuck)") << "\n";
    std::cout << "Iterations: " << iter << "\n";
    std::cout << "Final Gradient Norm: " << std::scientific << gradient(phi, x).norm() << std::defaultfloat << "\n";
    
    return x;
}

//functons to find minimums for:

double rosenbrock_scalar(vector x) {
    double xi = x[0];
    double yi = x[1];
    return std::pow(1.0 - xi, 2) + 100.0 * std::pow(yi - xi*xi, 2);
}

//now himmerlblaus function

double himmelblau_scalar(vector x) {
    double xi = x[0];
    double yi = x[1];
    return std::pow(xi*xi + yi - 11, 2) + std::pow(xi + yi*yi - 7, 2);
}


void QuestionA() {
    std::cout << "######### Question A: Newton minimisation #########\n\n";
    //First test on Rosenbrock function
    std::cout << "Testing on Rosenbrock function:\n";
    std::cout << "f(x,y) = (1-x)^2 + 100(y-x^2)^2\n";
    std::cout << "Expected minimum: (1.0, 1.0)\n\n";

    vector start({-1.0, 1.0});
    std::cout << "Start: (" << start[0] << ", " << start[1] << ")\n";

    vector min_rosen = newton_minimise(rosenbrock_scalar, start, 1e-6, 10000);

    std::cout << "Found minimum at: (" << min_rosen[0] << ", " << min_rosen[1] << ")\n";
    std::cout << "Function value at minimum: " << rosenbrock_scalar(min_rosen) << "\n\n";

    //second test with Himmelblaus function
    std::cout << "Testing on Himmelblau's function:\n";
    std::cout << "f(x,y) = (x^2+y-11)^2 + (x+y^2-7)^2\n";
    std::cout << "Has 4 minima. Testing different starts...\n\n";
    std::vector<std::pair< double, double>> himmel_starts = {
        {3.0, 2.0},  
        {-2.8, 3.1}, 
        {-3.8, -3.3}, 
        {3.6, -1.8}  
    };

    for (auto& p : himmel_starts) {
        vector start_vec({p.first, p.second});
        std::cout << "Starting at: (" << p.first << ", " << p.second << ")\n";
        
        vector min_himmel = newton_minimise(himmelblau_scalar, start_vec, 1e-8);
        
        std::cout << "Found minimum at: (" << min_himmel[0] << ", " << min_himmel[1] << ")\n";
        std::cout << "Function value at minimum: " << himmelblau_scalar(min_himmel) << "\n\n";
    }
}

void QuestionB() {
    std::cout << "######### Question B: Higgs Boson Fit #########\n\n";
    std::vector<double> energy, signal, error;
    double x, y, z;
    
    std::cout << "Reading data from stdin...\n";
    while (std::cin >> x >> y >> z) {
        energy.push_back(x);
        signal.push_back(y);
        error.push_back(z);
    }

    if (energy.empty()) {
        std::cerr << "Error: No data read from stdin. Usage: ./main < higgs.data.txt\n";
        return;
    }

    std::cout << "Loaded " << energy.size() << " data points.\n";

    // Lambda captures 'energy', 'signal', 'error' by reference automatically
    auto chi2_func = [&](vector params) -> double {
        double m = params[0];
        double gamma = params[1];
        double A = params[2];
        double chi2 = 0.0;
        
        for (size_t i = 0; i < energy.size(); ++i) {
            double E = energy[i];
            double sigma_obs = signal[i];
            double err = error[i];
            
            double denom = (E - m) * (E - m) + (gamma * gamma) / 4.0;
            if (denom == 0) return 1e20; 
            double sigma_model = A / denom;
            
            double residual = (sigma_model - sigma_obs) / err;
            chi2 += residual * residual;
        }
        return chi2;
    };

    vector start_params({125.0, 1.0, 5.0});
    
    std::cout << "Initial guess: m=125, Gamma=1.0, A=5.0\n";
    std::cout << "Running Newton Minimization...\n";

    vector best_params = newton_minimise(chi2_func, start_params, 1e-6);

    double best_m = best_params[0];
    double best_gamma = best_params[1];
    double best_A = best_params[2];
    double final_chi2 = chi2_func(best_params);

    std::cout << "\n--- Fit Results ---\n";
    std::cout << "Mass (m): " << std::fixed << std::setprecision(2) << best_m << " GeV\n";
    std::cout << "Width (Gamma): " << std::fixed << std::setprecision(4) << best_gamma << " GeV\n";
    std::cout << "Amplitude (A): " << std::fixed << std::setprecision(4) << best_A << "\n";
    std::cout << "Final Chi-squared: " << std::scientific << final_chi2 << std::defaultfloat << "\n";

    std::cout << "\nGenerating plot data...\n";
    
    std::ofstream plot_file("higgs_fit.dat");
    if (!plot_file) {
        std::cerr << "Error: Could not create higgs_fit.dat\n";
        return;
    }

    plot_file << "# E[GeV] Signal[units] Error[units] Fit[units]\n";

    for (size_t i = 0; i < energy.size(); ++i) {
        double E = energy[i];
        double sigma_obs = signal[i];
        double err = error[i];
        double sigma_model = best_A / ((E - best_m)*(E - best_m) + (best_gamma*best_gamma)/4.0);
        plot_file << E << " " << sigma_obs << " " << err << " " << sigma_model << "\n";
    }
    
    double min_e = energy.front() - 10;
    double max_e = energy.back() + 10;
    double step = 0.1;
    
    plot_file << "\n# Smooth Fit Curve\n";
    for (double e = min_e; e <= max_e; e += step) {
        double sigma_model = best_A / ((e - best_m)*(e - best_m) + (best_gamma*best_gamma)/4.0);
        plot_file << e << " " << sigma_model << "\n";
    }

    plot_file.close();
    std::cout << "Data saved to higgs_fit.dat\n";
}


//question C implementation. Previous functions for A are the forward ones, we do central now:
vector gradient_central(std::function<double(vector)> phi, vector x) {
    int n = x.size();
    vector g(n);
    double h_base = std::pow(2.0, -26);

    for (int i = 0; i < n; i++) {
        double h = (1.0 + std::abs(x[i])) * h_base;
        vector x_plus = x;
        vector x_minus = x;
        x_plus[i] += h;
        x_minus[i] -= h;
        g[i] = (phi(x_plus) - phi(x_minus)) / (2.0 * h);
    }
    return g;
}

matrix hessian_central(std::function<double(vector)> phi, vector x) {
    int n = x.size();
    matrix H(n, n);
    double h_base = std::pow(2.0, -17.0);

    for (int j = 0; j < n; j++) {
        double h = (1.0 + std::abs(x[j])) * h_base;
        vector x_plus = x;
        vector x_minus = x;
        x_plus[j] += h;
        x_minus[j] -= h;

        vector g_plus = gradient_central(phi, x_plus);
        vector g_minus = gradient_central(phi, x_minus);

        for (int i = 0; i < n; i++) {
            H(i, j) = (g_plus[i] - g_minus[i]) / (2.0 * h);
        }
    }
    return H;
}

vector newton_minimise_central(std::function<double(vector)> phi, vector x, double acc = 1e-3, int max_iter = 2000) {
    int iter = 0;
    bool converged = false;
    double reg = 1e-6;

    while (iter < max_iter) {
        vector g = gradient_central(phi, x);
        double g_norm = g.norm();
        
        if (g_norm < acc) {
            converged = true;
            break;
        }

        matrix H = hessian_central(phi, x);

        int n = H.size1();
        for (int i = 0; i < n; i++) {
            H(i, i) += reg; 
        }

        QRResults qr = qr_decomposition(H);
        vector dx = qr.solve(g * -1.0);

        double lambda = 1.0;
        double phi_x = phi(x);
        int ls_fails = 0;
        
        while (lambda >= 1.0 / 1024.0) {
            vector x_new = x + dx * lambda;
            if (phi(x_new) < phi_x) {
                break;
            }
            lambda /= 2.0;
            ls_fails++;
        }

        if (ls_fails > 5) {
            reg *= 10.0;
            if (reg > 1.0) reg = 1.0;
        } else {
            if (reg > 1e-6) reg /=2.0;
        }
        x = x + dx * lambda;
        iter++;
    }

    std::cout << "Converged: " << (converged ? "Yes" : "No (Max Iterations or Stuck)") << "\n";
    std::cout << "Iterations: " << iter << "\n";
    std::cout << "Final Gradient Norm: " << std::scientific << gradient_central(phi, x).norm() << std::defaultfloat << "\n";
    
    return x;
}

void QuestionC() {
    std::cout << "######### Question C: Central Difference Comparison #########\n\n";
    
    std::cout << "Comparing Forward Difference (Part A) vs Central Difference (Part C)\n";

    // Test Rosenbrock function first
    std::cout << "### Rosenbrock Function ###\n";
    vector start_rosen({-1.0, 1.0});
    
    std::cout << "Start: (" << start_rosen[0] << ", " << start_rosen[1] << ")\n\n";
    
    std::cout << "Forward Difference:\n";
    // We reuse the old function which uses forward/forward
    vector res_forward = newton_minimise(rosenbrock_scalar, start_rosen, 1e-6);
    std::cout << "   Result: (" << res_forward[0] << ", " << res_forward[1] << ")\n";
    std::cout << "   Value: " << rosenbrock_scalar(res_forward) << "\n\n";

    std::cout << "Central Difference:\n";
    vector res_central = newton_minimise_central(rosenbrock_scalar, start_rosen, 1e-6);
    std::cout << "   Result: (" << res_central[0] << ", " << res_central[1] << ")\n";
    std::cout << "   Value: " << rosenbrock_scalar(res_central) << "\n\n";

    // Test Himmelblau function after
    std::cout << "### Himmelblau Function ###\n";
    std::vector<std::pair<double, double>> starts = {
        {3.0, 2.0}, {-2.8, 3.1}, {-3.8, -3.3}, {3.6, -1.8}
    };

    for (auto& p : starts) {
        vector start_vec({p.first, p.second});
        std::cout << "Start: (" << p.first << ", " << p.second << ")\n";
        
        std::cout << "   Forward: ";
        vector res_f = newton_minimise(himmelblau_scalar, start_vec, 1e-8);
        std::cout << "(" << res_f[0] << ", " << res_f[1] << ") Val=" << himmelblau_scalar(res_f) << "\n";
        
        std::cout << "   Central: ";
        vector res_c = newton_minimise_central(himmelblau_scalar, start_vec, 1e-8);
        std::cout << "(" << res_c[0] << ", " << res_c[1] << ") Val=" << himmelblau_scalar(res_c) << "\n\n";
    }

}

int main() {

    std::ofstream outFile("Out.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create Out.txt\n";
        return 1;
    }
    std::streambuf* old = std::cout.rdbuf(outFile.rdbuf());

    std::cout << "######### Homework: Minimisation #########\n\n";
    
    std::cout << "######### Question A: #########\n\n";
    QuestionA();
    std::cout << "\n#################################################################\n\n";
    
    std::cout << "######### Question B: #########\n\n";
    QuestionB();
    std::cout << "\n#################################################################\n\n";
    
    std::cout << "######### Question C: #########\n\n";
    QuestionC();
    std::cout << "\n#################################################################\n\n";

    std::cout.rdbuf(old);
    outFile.close();
    
    std::cout << "Results written to Out.txt\n";
    return 0;
}