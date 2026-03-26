#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace lineq {

// ---------------------------
// Vector class
// ---------------------------
struct vector {
    std::vector<double> data;

    // Default constructor: empty vector
    vector() = default;

    // Constructor with size n, initialized to zero
    vector(int n) : data(n, 0) {}

    // Constructor from initializer list (for {1.0, 0.0} syntax)
    vector(std::initializer_list<double> list) : data(list) {}

    // Copy / move are fine
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector& operator=(const vector&) = default;
    vector& operator=(vector&&) noexcept = default;

    // Size of vector
    int size() const { return data.size(); }

    // Access elements
    double& operator[](int i) { return data[i]; }
    const double& operator[](int i) const { return data[i]; }

    // Euclidean norm
    double norm() const {
        double s = 0;
        for(double x : data) s += x*x;
        return std::sqrt(s);
    }

    // Dot product
    double dot(const vector& b) const {
        double s = 0;
        for(int i=0; i<size(); i++)
            s += data[i]*b[i];
        return s;
    }

    // Print vector
    void print(const std::string& name = "") const {
        if(!name.empty()) std::cout << name << "\n";
        for(int i=0; i<size(); ++i)
            std::cout << std::setw(12) << data[i];
        std::cout << "\n\n";
    }
};

// ---------------------------
// Vector arithmetic operators
// ---------------------------
inline vector operator+(vector a, const vector& b) { 
    for(int i=0;i<a.size();i++) a[i]+=b[i]; 
    return a; 
}
inline vector operator-(vector a, const vector& b) { 
    for(int i=0;i<a.size();i++) a[i]-=b[i]; 
    return a; 
}
inline vector operator-(vector a) { // unary minus
    for(int i=0;i<a.size();i++) a[i] = -a[i];
    return a;
}

// Scalar multiplication
inline vector operator*(const vector& a, double c){
    vector r = a;
    for(int i=0;i<r.size();i++) r[i]*=c;
    return r;
}
inline vector operator*(double c, const vector& a){
    vector r = a;
    for(int i=0;i<r.size();i++) r[i]*=c;
    return r;
}

// Scalar division
inline vector operator/(vector a, double c){
    for(int i=0;i<a.size();i++) a[i]/=c;
    return a;
}

// ---------------------------
// Matrix class
// ---------------------------
struct matrix {
    std::vector<vector> cols; // column-major storage

    matrix() = default;
    matrix(int n,int m) : cols(m, vector(n)) {}

    int size1() const { return cols.empty() ? 0 : cols[0].size(); } // rows
    int size2() const { return cols.size(); } // columns

    // Access elements
    double& operator()(int i,int j) { return cols[j][i]; }
    const double& operator()(int i,int j) const { return cols[j][i]; }

    vector& operator[](int j) { return cols[j]; }
    const vector& operator[](int j) const { return cols[j]; }

    // Transpose
    matrix transpose() const {
        matrix R(size2(), size1());
        for(int i=0;i<size1();i++)
            for(int j=0;j<size2();j++)
                R(j,i) = (*this)(i,j);
        return R;
    }

    // Identity matrix
    static matrix id(int n){
        matrix I(n,n);
        for(int i=0;i<n;i++) I(i,i) = 1;
        return I;
    }

    // Print matrix
    void print(const std::string& name = "") const {
        if(!name.empty()) std::cout << name << "\n";
        std::cout << std::fixed << std::setprecision(6);
        for(int i = 0; i < size1(); ++i){
            for(int j = 0; j < size2(); ++j)
                std::cout << std::setw(12) << (*this)(i,j);
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

// ---------------------------
// Matrix arithmetic operators
// ---------------------------
inline matrix operator*(const matrix& A, const matrix& B){
    if(A.size2()!=B.size1()) throw std::invalid_argument("size mismatch");
    matrix R(A.size1(),B.size2());
    for(int j=0;j<B.size2();j++)
        for(int k=0;k<A.size2();k++)
            for(int i=0;i<A.size1();i++)
                R(i,j)+=A(i,k)*B(k,j);
    return R;
}

// Matrix-vector multiplication
inline vector operator*(const matrix& A, const vector& v){
    vector r(A.size1());
    for(int j=0;j<A.size2();j++)
        for(int i=0;i<A.size1();i++)
            r[i]+=A(i,j)*v[j];
    return r;
}

// ---------------------------
// Approximate comparisons
// ---------------------------
inline bool approx(double a,double b,double eps=1e-8){
    return std::abs(a-b)<eps;
}

inline bool approx(const vector& a,const vector& b,double eps=1e-8){
    if(a.size()!=b.size()) return false;
    for(int i=0;i<a.size();i++)
        if(!approx(a[i],b[i],eps)) return false;
    return true;
}

inline bool approx(const matrix& A,const matrix& B,double eps=1e-8){
    if(A.size1()!=B.size1() || A.size2()!=B.size2()) return false;
    for(int i=0;i<A.size1();i++)
        for(int j=0;j<A.size2();j++)
            if(!approx(A(i,j),B(i,j),eps)) return false;
    return true;
}

}