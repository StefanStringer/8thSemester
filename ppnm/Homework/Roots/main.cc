#include "matrix.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>

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
    
    std::cout.rdbuf(old);
    outFile.close();
    
    std::cout << "Results written to Out.txt\n";
    return 0;
}