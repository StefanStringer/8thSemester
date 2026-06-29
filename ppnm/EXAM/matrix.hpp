#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace lineq {

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
    int n;
    std::vector<double> data;

    vector() : n(0) {}
    vector(int n_) : n(n_), data(n_, 0.0) {}
    
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector& operator=(const vector&) = default;
    vector& operator=(vector&&) noexcept = default;

    int size() const { return n; }

    double& operator[](int i) { 
        if(i < 0 || i >= n) throw std::out_of_range("Vector index out of range");
        return data[i]; 
    }
    const double& operator[](int i) const { 
        if(i < 0 || i >= n) throw std::out_of_range("Vector index out of range");
        return data[i]; 
    }

    void print(const std::string& name = "") const {
        if(!name.empty()) std::cout << name << "\n";
        for(int i=0; i<n; ++i)
            std::cout << std::setw(12) << data[i];
        std::cout << "\n\n";
    }
};
}
namespace lineq {

// Matrix-Vector multiplication is added here to support the declipping function
inline vector operator*(const matrix& A, const vector& v) {
    if(A.size2() != v.n) throw std::runtime_error("Matrix-vector dimensions mismatch");
    vector r(A.size1());
    for(int i=0; i<A.size1(); i++){
        double sum = 0.0;
        for(int j=0; j<A.size2(); j++){
            sum += A(i,j) * v[j];
        }
        r[i] = sum;
    }
    return r;
}

}