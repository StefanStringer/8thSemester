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

struct vector{
    int n;
    std::vector<double> data;

    vector(int n_): n(n_), data(n_,0.0) {}

    double& operator[](int i){ return data[i]; }
    double operator[](int i) const { return data[i]; }

    int size() const { return n; }

    void print(const std::string &msg="") const {
        if(msg!="") std::cout << msg << "\n";
        for(int i=0;i<n;i++)
            std::cout << std::setw(12) << data[i] << "\n";
    }
};

}