#include "matrix.hpp"
#include <iomanip>
#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////
// RANDOM SYMMETRIC MATRIX
//////////////////////////////////////////////////////////////

Eigenvalues::matrix make_random_symmetric(int n){

    std::mt19937 rng(1234);
    std::uniform_real_distribution<double> dist(-1.0,1.0);

    Eigenvalues::matrix A(n,n);

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            A(i,j)=dist(rng);
        }
    }

    for(int i=0;i<n;i++)
    for(int j=0;j<i;j++){
        double val=0.5*(A(i,j)+A(j,i));
        A(i,j)=val;
        A(j,i)=val;
    }

    return A;
}

//////////////////////////////////////////////////////////////
// JACOBI SOLVER PART 1
//////////////////////////////////////////////////////////////

class JacobiSolver{

public:

static void timesJ(Eigenvalues::matrix& A,int p,int q,double theta){

    double c=cos(theta);
    double s=sin(theta);

    for(int i=0;i<A.size1();i++){

        double aip=A(i,p);
        double aiq=A(i,q);

        A(i,p)=c*aip-s*aiq;
        A(i,q)=s*aip+c*aiq;
    }
}

static void Jtimes(Eigenvalues::matrix& A,int p,int q,double theta){

    double c=cos(theta);
    double s=sin(theta);

    for(int j=0;j<A.size2();j++){

        double apj=A(p,j);
        double aqj=A(q,j);

        A(p,j)=c*apj+s*aqj;
        A(q,j)=-s*apj+c*aqj;
    }
}

static pair<Eigenvalues::vector,Eigenvalues::matrix> diagonalize(Eigenvalues::matrix A){

    int n=A.size1();

    Eigenvalues::matrix V(n,n);
    V.setid();

    bool changed;
    int sweeps=0;

    do{

        changed=false;

        for(int p=0;p<n-1;p++)
        for(int q=p+1;q<n;q++){

            double app=A(p,p);
            double aqq=A(q,q);
            double apq=A(p,q);

            double theta=0.5*atan2(2*apq,aqq-app);

            double c=cos(theta);
            double s=sin(theta);

            double new_app=c*c*app-2*s*c*apq+s*s*aqq;
            double new_aqq=s*s*app+2*s*c*apq+c*c*aqq;

            if(new_app!=app || new_aqq!=aqq){

                changed=true;

                timesJ(A,p,q,theta);
                Jtimes(A,p,q,-theta);
                timesJ(V,p,q,theta);
            }
        }

        sweeps++;

    }while(changed);

    cout<<"Jacobi rotation sweeps used in calc: "<<sweeps<<"\n";

    Eigenvalues::vector eigenvalues(n);

    for(int i=0;i<n;i++)
        eigenvalues[i]=A(i,i);

    return {eigenvalues,V};
}

};

//////////////////////////////////////////////////////////////
// HYDROGEN HAMILTONIAN PART 2
//////////////////////////////////////////////////////////////

Eigenvalues::matrix hydrogen_matrix(double rmax,double dr){

    int n=(int)(rmax/dr)-1;

    Eigenvalues::matrix H(n,n);

    Eigenvalues::vector r(n);

    for(int i=0;i<n;i++)
        r[i]=dr*(i+1);

    double k=-0.5/(dr*dr);

    for(int i=0;i<n-1;i++){

        H(i,i)  = -2*k;
        H(i,i+1)=  k;
        H(i+1,i)=  k;
    }

    H(n-1,n-1)=-2*k;

    for(int i=0;i<n;i++)
        H(i,i)+=-1/r[i];

    return H;
}

//////////////////////////////////////////////////////////////
// TIMING FUNCTION MY GOD THIS TOOK AGES TO FIGURE OUT
//////////////////////////////////////////////////////////////

double time_jacobi(int n){

    auto A=make_random_symmetric(n);

    auto start=chrono::high_resolution_clock::now();

    JacobiSolver::diagonalize(A);

    auto end=chrono::high_resolution_clock::now();

    chrono::duration<double> dt=end-start;

    return dt.count();
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////

using namespace std;

int main(int argc, char** argv) {

    cout << "\n=============================\n";
    cout << "PART 1: JACOBI TEST\n";
    cout << "=============================\n\n";

    int n = 4;
    auto A = make_random_symmetric(n);

    cout << "Random symmetric matrix A:\n";
    A.print("");

    auto [w, V] = JacobiSolver::diagonalize(A);

    cout << "Eigenvalues:\n";
    w.print("");

    cout << "Eigenvectors V:\n";
    V.print("");

    Eigenvalues::matrix D(n,n);
    for(int i=0; i<n; i++)
        D(i,i) = w[i];
    cout << "Diagonal matrix D:\n";
    D.print("");

    auto A_reconstructed = V*D*V.transpose();
    cout << "Check V*D*V^T:\n";
    A_reconstructed.print("");

    auto VTV = V.transpose()*V;
    cout << "Check V^T * V:\n";
    VTV.print("");

    // -------------------------------
    // PART 2: HYDROGEN ATOM
    // -------------------------------

    cout << "\n=============================\n";
    cout << "PART 2: HYDROGEN ATOM\n";
    cout << "=============================\n\n";

    double rmax = 10.0;
    double dr   = 0.3;
    string wf_file = "";
    string diag_file = "";

    // parse command-line args
    for(int i=1; i<argc; i++){
        string arg = argv[i];
        if(arg == "-wf" && i+1 < argc)
            wf_file = argv[++i];
        if(arg == "-diagf" && i+1 < argc)
            diag_file = argv[++i];
        if(arg == "-rmax" && i+1 < argc)
            rmax = atof(argv[++i]);
        if(arg == "-dr" && i+1 < argc)
            dr   = atof(argv[++i]);
    }

    int npoints = int(rmax/dr) - 1;
    Eigenvalues::vector r(npoints);
    for(int i=0; i<npoints; i++) r[i] = dr*(i+1);

    auto H = hydrogen_matrix(rmax, dr);
    cout << "Hamiltonian built.\n";

    auto [E, Vec] = JacobiSolver::diagonalize(H);

    cout << "\nLowest energies:\n";
    int nprint = min(5, npoints);
    for(int i=0; i<nprint; i++)
        cout << "E[" << i << "] = " << E[i] << "\n";

    cout << "Exact ground state = -0.5\n";

    // save wavefunctions if requested
    if(wf_file != ""){
        ofstream wf(wf_file);
        for(int i=0; i<npoints; i++){
            wf << r[i];
            for(int k=0; k<3; k++){ // save first 3 wavefunctions
                wf << " " << Vec(i,k)/sqrt(dr); // normalized
            }
            wf << "\n";
        }
        wf.close();
    }

    // -------------------------------
    // PART 3: SCALING TEST
    // -------------------------------

    cout << "\n=============================\n";
    cout << "PART 3: SCALING TEST\n";
    cout << "=============================\n\n";

    cout << "N    Time(s)\n";

    vector<int> sizes = {20,40,60,80,100,120,140,160,180,200};
    vector<double> times(sizes.size());

    for(size_t i=0;i<sizes.size();i++){
        times[i] = time_jacobi(sizes[i]);
        cout << sizes[i] << "   " << times[i] << "\n";
    }

    // save diag times if requested
    if(diag_file != ""){
        ofstream df(diag_file);
        for(size_t i=0;i<sizes.size();i++)
            df << sizes[i] << " " << times[i] << "\n";
        df.close();
    }

    cout << "\nExpected scaling ~ N^3\n";
    cout << "\nProgram finished.\n";
    cout << "\n Images printed out can be found in the current folder.\n";

    return 0;
}