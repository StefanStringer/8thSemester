#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace lineq {

struct vector {
    std::vector<double> data;

    vector() = default;
    vector(int n) : data(n,0) {}

    int size() const { return data.size(); }

    double& operator[](int i){ return data[i]; }
    const double& operator[](int i) const { return data[i]; }

    double norm() const {
        double s=0;
        for(double x: data) s+=x*x;
        return std::sqrt(s);
    }

    double dot(const vector& b) const {
        double s=0;
        for(int i=0;i<size();i++)
            s+=data[i]*b[i];
        return s;
    }

	void print(const std::string& name = "") const
{
    if(!name.empty())
        std::cout << name << "\n";

    for(int i = 0; i < size(); ++i)
        std::cout << std::setw(12) << data[i];

    std::cout << "\n\n";
}
};

inline vector operator+(vector a,const vector& b){ 
    for(int i=0;i<a.size();i++) a[i]+=b[i]; 
    return a; 
}
inline vector operator-(vector a,const vector& b){ 
    for(int i=0;i<a.size();i++) a[i]-=b[i]; 
    return a; 
}
inline vector operator*(double c, vector a){ 
    for(int i=0;i<a.size();i++) a[i]*=c; 
    return a; 
}
inline vector operator/(vector a,double c){ 
    for(int i=0;i<a.size();i++) a[i]/=c; 
    return a; 
}

struct matrix {
    std::vector<vector> cols;

    matrix() = default;
    matrix(int n,int m) : cols(m,vector(n)) {}

    int size1() const { return cols.empty()?0:cols[0].size(); }
    int size2() const { return cols.size(); }

    double& operator()(int i,int j){ return cols[j][i]; }
    const double& operator()(int i,int j) const { return cols[j][i]; }

    vector& operator[](int j){ return cols[j]; }
    const vector& operator[](int j) const { return cols[j]; }

    matrix transpose() const{
        matrix R(size2(),size1());
        for(int i=0;i<size1();i++)
            for(int j=0;j<size2();j++)
                R(j,i)=(*this)(i,j);
        return R;
    }

    static matrix id(int n){
        matrix I(n,n);
        for(int i=0;i<n;i++) I(i,i)=1;
        return I;
    }

	void print(const std::string& name = "") const
{
    if(!name.empty())
        std::cout << name << "\n";

    std::cout << std::fixed << std::setprecision(6);

    for(int i = 0; i < size1(); ++i)
    {
        for(int j = 0; j < size2(); ++j)
            std::cout << std::setw(12) << (*this)(i,j);
        std::cout << "\n";
    }

    std::cout << "\n";
}
};

inline matrix operator*(const matrix& A,const matrix& B){
    if(A.size2()!=B.size1()) throw std::invalid_argument("size mismatch");
    matrix R(A.size1(),B.size2());
    for(int j=0;j<B.size2();j++)
        for(int k=0;k<A.size2();k++)
            for(int i=0;i<A.size1();i++)
                R(i,j)+=A(i,k)*B(k,j);
    return R;
}

inline vector operator*(const matrix& A,const vector& v){
    vector r(A.size1());
    for(int j=0;j<A.size2();j++)
        for(int i=0;i<A.size1();i++)
            r[i]+=A(i,j)*v[j];
    return r;
}

inline bool approx(double a,double b,double eps=1e-8){
    return std::abs(a-b)<eps;
}

inline bool approx(const matrix& A,const matrix& B,double eps=1e-8){
    if(A.size1()!=B.size1() || A.size2()!=B.size2()) return false;
    for(int i=0;i<A.size1();i++)
        for(int j=0;j<A.size2();j++)
            if(!approx(A(i,j),B(i,j),eps)) return false;
    return true;
}

inline bool approx(const vector& a,const vector& b,double eps=1e-8){
    if(a.size()!=b.size()) return false;
    for(int i=0;i<a.size();i++)
        if(!approx(a[i],b[i],eps)) return false;
    return true;
}

}