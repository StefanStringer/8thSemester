//packages I want to import
#include<vector> 
#include<random>
#include<iostream>
//files i want to include from
#include"matrix.hpp"

struct QR{
    std::pair<lineq::matrix, lineq::matrix>  decomp(const lineq::matrix& A) {

        lineq::matrix Q(A); // copy A into Q
        lineq::matrix R(A.size2(), A.size2());

        int m = A.size2();

        for (int i = 0; i < m; ++i) {

            lineq::vector v = A[i];
            for (int j = 0; j < i; ++j) {
                double r = v.dot(Q[j]);
                R(j, i) = r;
                v = v - r * Q[j];
            }

            double norm_v = v.norm();
            R[i][i] = norm_v;
            Q[i] = v / norm_v;
        }
        return {Q, R};
    }

    lineq::vector back_sub(const lineq::matrix& R, const lineq::vector& y) {
        lineq::vector x(R.size2());
        for (int i = R.size2() - 1; i >= 0; i--) {

            double sum = 0;
            for (int j = i + 1; j < R.size2(); j++) {
                sum += R(i, j) * x[j];
            }

            x[i] = (y[i] - sum) / R(i, i);
        }
        return x;
    }   

        // Here wa solve the linear system Ax=b using the QR decomposition of A. We first compute y = Q^T b, and then solve Rx=y using back substitution.
    lineq::vector solve(const lineq::matrix& Q, const lineq::matrix& R, const lineq::vector& b) {
        lineq::vector y(Q.size2());
        for (int i = 0; i < Q.size2(); i++) {
            y[i] = Q[i].dot(b);
        }
        return back_sub(R, y);
    }

    double det(const lineq::matrix& R) {
        double prod = 1;
        for (int i = 0; i < R.size1(); i++) {
            prod *= R[i][i];
        }
        return prod;

    }

    lineq::matrix inverse(const lineq::matrix& Q, const lineq::matrix& R) {
        lineq::matrix R_inv(R);
        for (int i = 0; i < R.size2(); i++) {
            lineq::vector ek(R.size1());
            ek[i] = 1;
            lineq::vector ck = back_sub(R, ek);
            R_inv[i] = ck;
        }
        lineq::matrix A_inv = R_inv * Q.transpose();
        return A_inv;
    }

    
};