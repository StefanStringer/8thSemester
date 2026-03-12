#pragma once
#include <vector>
#include <iostream>
#include <string>

namespace Eigenvalues{

struct matrix{
    int n,m;
    std::vector<double> data;

    matrix(int n_,int m_);

    double& operator()(int i,int j);
    double operator()(int i,int j) const;

    int size1() const;
    int size2() const;

    void setid();

    void print(std::string s="") const;
    matrix transpose() const;
};

struct vector{
    int n;
    std::vector<double> data;

    vector(int n_);

    double& operator[](int i);
    double operator[](int i) const;

    void print(std::string s="") const;
};

matrix operator*(const matrix& A,const matrix& B);

}