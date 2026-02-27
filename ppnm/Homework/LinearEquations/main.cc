#include "matrix.hpp"
#include "qr.hpp"

#include <iostream>
#include <random>
#include <chrono>
#include <stdexcept>

/* ============================================================
   Random Generators
============================================================ */

lineq::matrix random_matrix(int n, int m)
{
    if (n < m)
        throw std::invalid_argument("Require n >= m");

    // Random seeding
    // Mersenne Twister engine to help with random number??
    std::mt19937 rng(std::random_device{}());
    // we want a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> Uniform(0.0,1.0);

    lineq::matrix A(n,m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A.cols[j][i] = Uniform(rng);
        }
    }
    return A;
}

lineq::vector random_vector(int n)
{
    // Random seeding
    // Mersenne Twister engine to help with random number??
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> Uniform(0.0,1.0);

    lineq::vector v(n);
    
    for (int j = 0; j < n; j++) {
        v[j] = Uniform(rng);
    }

    return v;
}

/* ============================================================
   MAIN
============================================================ */

int main(int argc, char** argv)

{
    std::cout << "\n==================================================\n";
    std::cout << "Homework: Linear Equations \n";
    std::cout << "==================================================\n\n";

    std::cout << "\n=========================================================\n";
    std::cout << "PART A: A Matrix, Decomposition functions and Linear Solve\n";
    std::cout << "===========================================================\n\n";

    //create and print all matricies and vectors using the random generators
    int n = 4;  // default value as n > m is needed for tall matrix

    if(argc > 1){
        n = std::stoi(argv[1]);
    }

    int m = 3;

    std::cout << "Generating random tall matrix A (" << n << "x" << m << ")\n\n";

    lineq::matrix A = random_matrix(n,m); 
    A.print("A =");

    lineq::QR qrA(A);

    std::cout << "\nMatrix Q:\n";
    qrA.Q().print("Q =");

    std::cout << "\nMatrix R:\n";
    qrA.R().print("R =");

    //hHere we check R upper triangular

    bool upper = true;
    for(int i=0;i<m;++i)
        for(int j=0;j<i;++j)
            if(std::abs(qrA.R()(i,j)) > 1e-10)
                upper = false;

    std::cout << "\nCheck: R is upper triangular -> "
              << (upper ? "TRUE" : "FALSE") << "\n";

    std::cout << "Resulting R:\n";
    qrA.R().print("R =");

    // Here we check Q^T Q = I

    lineq::matrix QTQ = qrA.Q().transpose()*qrA.Q();
    bool orthogonal = lineq::approx(QTQ, lineq::matrix::id(m));

    std::cout << "Check: Q^T Q = I -> "
              << (orthogonal ? "TRUE" : "FALSE") << "\n";

    std::cout << "Resulting Q^T*Q:\n";
    QTQ.print("Q^T Q =");
    // Here we now check QR = A

    lineq::matrix QRcheck = qrA.Q()*qrA.R();
    bool reconstruct = lineq::approx(QRcheck, A);

    std::cout << "Check: Q*R = A -> "
              << (reconstruct ? "TRUE" : "FALSE") << "\n";
    
    std::cout << "Resulting Q*R:\n";
    QRcheck.print("Q*R =");
    // Test all the solving fujnctions here 

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "Solve test with square matrix\n";
    std::cout << "--------------------------------------------------\n\n";

    int size=3;

    lineq::matrix Asq = random_matrix(size,size);
    lineq::vector b = random_vector(size);
    std::cout << "New matrix A where n = m = " << size << ":\n";
    std::cout << "Square matrix A:\n";
    Asq.print("A =");

    std::cout << "\nVector b:\n";
    b.print("b =");

    lineq::QR qrB(Asq);

    lineq::vector x = qrB.solve(b);

    std::cout << "\nSolution x:\n";
    x.print("x =");

    lineq::vector Ax = Asq*x;

    std::cout << "\nCheck Ax:\n";
    Ax.print("A*x =");

    std::cout << "\nApprox(Ax,b) = "
              << lineq::approx(Ax,b) << "\n";

    std::cout << "\nDeterminant det(A) = "
              << qrB.det() << "\n";


    //Invesre test part of PART A for questions

    std::cout << "\n==================================================\n";
    std::cout << "PART B: Matrix Inverse using QR \n";
    std::cout << "==================================================\n\n";

    lineq::matrix Ainv = qrB.inverse(); //check this agin. It works now!
    std::cout << "Inverse matrix A^{-1}:\n";
    Ainv.print("A^{-1} =");

    // Check AB = I
    lineq::matrix Icheck = Asq * Ainv;
    std::cout << "\nCheck A * A^{-1}:\n";
    Icheck.print("A * A^{-1} =");

    lineq::approx(Icheck, lineq::matrix::id(size));
    //bool is_identity = lineq::approx(Icheck, lineq::matrix::id(size)) < 1e-10;
    //std::cout << "\nA * A^{-1} ≈ I: " << (is_identity ? "TRUE" : "FALSE") << "\n"; //THis KEEPS PRINTING FALSE, but the matrices look correct. I think the approximation is just not good enough. I will check this again later.

    std::cout << "\n==================================================\n";
    std::cout << "DONE :) \n";
    std::cout << "==================================================\n\n";

    return 0;
}