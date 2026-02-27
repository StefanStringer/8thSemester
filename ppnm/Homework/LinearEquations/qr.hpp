#pragma once
#include "matrix.hpp"

namespace lineq {

class QR {
private:
    matrix Q_;
    matrix R_;

public:
    // Constructor performs QR decomposition
    QR(const matrix& A);

    // Solve Ax = b using stored Q and R
    vector solve(const vector& b) const;

    // Determinant of A (same as det(R))
    double det() const;

    matrix inverse() const;

    // Getters (needed for testing in main)
    const matrix& Q() const { return Q_; }
    const matrix& R() const { return R_; }
};

}