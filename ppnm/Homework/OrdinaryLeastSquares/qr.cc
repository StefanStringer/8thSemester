#include "qr.hpp"
#include <cmath>

namespace Eigenvalues {

/*-------------------------------------------------------------
  QR decomposition using Modified Gram-Schmidt
  A becomes Q, R is upper triangular
-------------------------------------------------------------*/
void QR_decomp(matrix &A, matrix &R){

    int m=A.size1();
    int n=A.size2();

    for(int i=0;i<n;i++){

        double norm=0;

        for(int k=0;k<m;k++)
            norm+=A(k,i)*A(k,i);

        norm=std::sqrt(norm);

        R(i,i)=norm;

        for(int k=0;k<m;k++)
            A(k,i)/=norm;

        for(int j=i+1;j<n;j++){

            double dot=0;

            for(int k=0;k<m;k++)
                dot+=A(k,i)*A(k,j);

            R(i,j)=dot;

            for(int k=0;k<m;k++)
                A(k,j)-=A(k,i)*dot;
        }
    }
}

/*-------------------------------------------------------------
  Back substitution for triangular systems
-------------------------------------------------------------*/
vector backsub(const matrix &R,const vector &b){

    int n=R.size1();
    vector x(n);

    for(int i=n-1;i>=0;i--){

        double sum=b[i];

        for(int j=i+1;j<n;j++)
            sum-=R(i,j)*x[j];

        x[i]=sum/R(i,i);
    }

    return x;
}

/*-------------------------------------------------------------
  Compute inverse of an upper triangular matrix
-------------------------------------------------------------*/
matrix inverse_upper(const matrix &R){

    int n=R.size1();
    matrix Rinv(n,n);

    for(int i=n-1;i>=0;i--){

        Rinv(i,i)=1/R(i,i);

        for(int j=i-1;j>=0;j--){

            double sum=0;

            for(int k=j+1;k<=i;k++)
                sum+=R(j,k)*Rinv(k,i);

            Rinv(j,i)=-sum/R(j,j);
        }
    }

    return Rinv;
}

/*-------------------------------------------------------------
  Least squares fit
-------------------------------------------------------------*/
std::tuple<vector,matrix> lsfit(
    std::vector<std::function<double(double)>> fs,
    vector x,
    vector y,
    vector dy
){

    int n=x.size();
    int m=fs.size();

    // Design matrix
    matrix A(n,m);

    // Right-hand side
    vector b(n);

    // Fill weighted system
    for(int i=0;i<n;i++){

        b[i]=y[i]/dy[i];

        for(int k=0;k<m;k++)
            A(i,k)=fs[k](x[i])/dy[i];
    }

    matrix R(m,m);

    QR_decomp(A,R);

    // compute Q^T b
    vector Qtb(m);

    for(int i=0;i<m;i++){

        double sum=0;

        for(int k=0;k<n;k++)
            sum+=A(k,i)*b[k];

        Qtb[i]=sum;
    }

    // best fit coefficients
    vector c=backsub(R,Qtb);

    /*---------------------------------------------
      Compute covariance matrix
      C = (R^T R)^-1 = R^{-1}(R^{-1})^T
    ---------------------------------------------*/

    matrix Rinv=inverse_upper(R);

    matrix Cov(m,m);

    for(int i=0;i<m;i++)
        for(int j=0;j<m;j++){

            double sum=0;

            for(int k=0;k<m;k++)
                sum+=Rinv(i,k)*Rinv(j,k);

            Cov(i,j)=sum;
        }

    return {c,Cov};
}

}