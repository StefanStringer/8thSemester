#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace Eigenvalues {

struct vector {
    int n;
    std::vector<double> data;

    vector() : n(0), data() {}
    vector(int n_) : n(n_), data(n_, 0.0) {}
    vector(std::initializer_list<double> init) : n(init.size()), data(init) {}

    double& operator[](int i) { return data[i]; }
    double operator[](int i) const { return data[i]; }

    int size() const { return n; }

    void print(const std::string &msg = "") const {
        if (msg != "") std::cout << msg << "\n";
        for (int i = 0; i < n; i++)
            std::cout << std::setw(12) << data[i] << "\n";
    }

    double norm() const {
        double sum = 0;
        for (int i = 0; i < n; i++) sum += data[i] * data[i];
        return std::sqrt(sum);
    }

    vector operator+(const vector &other) const {
        if (n != other.n) throw std::runtime_error("Vector size mismatch");
        vector res(n);
        for (int i = 0; i < n; i++) res.data[i] = data[i] + other.data[i];
        return res;
    }

    vector operator-(const vector &other) const {
        if (n != other.n) throw std::runtime_error("Vector size mismatch");
        vector res(n);
        for (int i = 0; i < n; i++) res.data[i] = data[i] - other.data[i];
        return res;
    }

    vector operator*(double scalar) const {
        vector res(n);
        for (int i = 0; i < n; i++) res.data[i] = data[i] * scalar;
        return res;
    }

    vector operator-() const {
        vector res(n);
        for (int i = 0; i < n; i++) res.data[i] = -data[i];
        return res;
    }
};

struct matrix {
    int n, m;
    std::vector<double> data;

    matrix(int n_, int m_) : n(n_), m(m_), data(n_ * m_, 0.0) {}

    double& operator()(int i, int j) { return data[i * m + j]; }
    double operator()(int i, int j) const { return data[i * m + j]; }

    int size1() const { return n; }
    int size2() const { return m; }

    void setid() {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                (*this)(i, j) = (i == j) ? 1.0 : 0.0;
    }

    void print(const std::string &msg = "") const {
        if (msg != "") std::cout << msg << "\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                std::cout << std::setw(12) << (*this)(i, j);
            std::cout << "\n";
        }
    }

    matrix transpose() const {
        matrix T(m, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                T(j, i) = (*this)(i, j);
        return T;
    }

    matrix operator*(const matrix &B) const {
        if (m != B.n) throw std::runtime_error("Matrix dimensions mismatch");
        matrix C(n, B.m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < B.m; j++) {
                double sum = 0;
                for (int k = 0; k < m; k++) sum += (*this)(i, k) * B(k, j);
                C(i, j) = sum;
            }
        return C;
    }

    vector operator*(const vector &v) const {
        if (m != v.n) throw std::runtime_error("Matrix-vector dimensions mismatch");
        vector res(n);
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int k = 0; k < m; k++) sum += (*this)(i, k) * v[k];
            res[i] = sum;
        }
        return res;
    }
};

} // namespace Eigenvalues kept from that homework exercise