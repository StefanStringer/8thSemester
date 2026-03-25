#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace Eigenvalues {

struct matrix{
    int n,m;
    std::vector<double> data;

    matrix(int n_, int m_): n(n_), m(m_), data(n_*m_,0.0) {}

    double& operator()(int i,int j){ return data[i*m+j]; }
    double operator()(int i,int j) const { return data[i*m+j]; }

    int size1() const { return n; }
    int size2() const { return m; }

    void setid(){
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                (*this)(i,j) = (i==j)?1.0:0.0;
    }

    void print(const std::string &msg="") const {
        if(msg!="") std::cout << msg << "\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++)
                std::cout << std::setw(12) << (*this)(i,j);
            std::cout << "\n";
        }
    }

    matrix transpose() const {
        matrix T(m,n);
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                T(j,i)=(*this)(i,j);
        return T;
    }

    matrix operator*(const matrix &B) const {
        if(m != B.n) throw std::runtime_error("Matrix dimensions mismatch");
        matrix C(n, B.m);
        for(int i=0;i<n;i++)
            for(int j=0;j<B.m;j++){
                double sum=0;
                for(int k=0;k<m;k++) sum += (*this)(i,k)*B(k,j);
                C(i,j)=sum;
            }
        return C;
    }
};

struct vector {
    std::vector<double> data;

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    vector() = default;
    vector(size_t n) : data(n) {}

    size_t size() const { return data.size(); }
    double& operator[](size_t i) { return data[i]; }
    const double& operator[](size_t i) const { return data[i]; }

    // Addition
    vector operator+(const vector& other) const {
        vector res(size());
        for(size_t i=0;i<size();i++) res[i] = data[i] + other[i];
        return res;
    }

    // Subtraction
    vector operator-(const vector& other) const {
        vector res(size());
        for(size_t i=0;i<size();i++) res[i] = data[i] - other[i];
        return res;
    }

    // Scalar multiplication
    vector operator*(double s) const {
        vector res(size());
        for(size_t i=0;i<size();i++) res[i] = data[i]*s;
        return res;
    }

    // Scalar multiplication (scalar first)
    friend vector operator*(double s, const vector& v) { return v*s; }

    // Euclidean norm
    double norm() const {
        double sum=0;
        for(double x:data) sum+=x*x;
        return std::sqrt(sum);
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const vector& v) {
        os << "[";
        for(size_t i=0;i<v.size();i++){
            os << v[i];
            if(i<v.size()-1) os << ", ";
        }
        os << "]";
        return os;
    }
};
}