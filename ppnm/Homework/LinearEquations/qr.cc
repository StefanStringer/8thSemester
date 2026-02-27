#include "qr.hpp"
#include <stdexcept>

namespace lineq {

// Constructor: performs modified Gram-Schmidt
QR::QR(const matrix& A)
{
    //int n = A.size1();   // rows
    int m = A.size2();   // columns

    Q_ = A;              // copy A into Q
    R_ = matrix(m, m);   // R is m x m

    for(int i = 0; i < m; i++){

        double norm = Q_[i].norm();

        if(norm == 0)
            throw std::runtime_error("Matrix is rank deficient");

        R_(i,i) = norm;
        Q_[i] = Q_[i] / norm;

        for(int j = i+1; j < m; j++){
            double dot = Q_[i].dot(Q_[j]);
            R_(i,j) = dot;
            Q_[j] = Q_[j] - dot * Q_[i];
        }
    }
}


// Solve QRx = b
vector QR::solve(const vector& b) const
{
    int m = R_.size2();

    // y = Q^T b
    vector y(m);
    for(int i = 0; i < m; i++)
        y[i] = Q_[i].dot(b);

    // Back substitution Rx = y
    vector x(m);

    for(int i = m-1; i >= 0; i--){
        double sum = 0;
        for(int j = i+1; j < m; j++)
            sum += R_(i,j) * x[j];

        x[i] = (y[i] - sum) / R_(i,i);
    }

    return x;
}


// Determinant = product of diagonal of R
double QR::det() const
{
    double d = 1.0;

    for(int i = 0; i < R_.size2(); i++)
        d *= R_(i,i);

    return d;
}

matrix QR::inverse() const
{
    int n = R_.size2();
    matrix Rinv(n,n);

    for(int k=0;k<n;++k)
    {
        vector e(n);
        e[k]=1.0;

        vector c(n);

        for(int i=n-1;i>=0;--i)
        {
            double sum=0;
            for(int j=i+1;j<n;++j)
                sum+=R_(i,j)*c[j];

            c[i]=(e[i]-sum)/R_(i,i);
        }

        Rinv[k]=c;
    }

    return Rinv * Q_.transpose();
}

}