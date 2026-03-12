#include "matrix.hpp"
#include <cassert>

namespace Eigenvalues{

matrix::matrix(int n_,int m_) : n(n_), m(m_), data(n_*m_){}

double& matrix::operator()(int i,int j){
    return data[i*m+j];
}

double matrix::operator()(int i,int j) const{
    return data[i*m+j];
}

int matrix::size1() const{return n;}
int matrix::size2() const{return m;}

void matrix::setid(){
    for(int i=0;i<n;i++)
    for(int j=0;j<m;j++)
        (*this)(i,j)=(i==j);
}

vector::vector(int n_) : n(n_), data(n_){}

double& vector::operator[](int i){return data[i];}
double vector::operator[](int i) const{return data[i];}

void matrix::print(std::string s) const{
    if(s!="") std::cout<<s<<"\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)
            std::cout<<(*this)(i,j)<<" ";
        std::cout<<"\n";
    }
}

void vector::print(std::string s) const{
    if(s!="") std::cout<<s<<"\n";
    for(int i=0;i<n;i++)
        std::cout<<data[i]<<"\n";
}

matrix matrix::transpose() const{

    matrix T(m,n);

    for(int i=0;i<n;i++)
    for(int j=0;j<m;j++)
        T(j,i)=(*this)(i,j);

    return T;
}

matrix operator*(const matrix& A,const matrix& B){

    assert(A.m==B.n);

    matrix C(A.n,B.m);

    for(int i=0;i<A.n;i++)
    for(int j=0;j<B.m;j++)
    for(int k=0;k<A.m;k++)
        C(i,j)+=A(i,k)*B(k,j);

    return C;
}

}