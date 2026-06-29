#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>

//my classes from previous homework
#include "matrix.hpp"
#include "qr.hpp"

using namespace lineq; //from the matrix.hpp file
using namespace std;

//new function for the exam question that takes the third diff of a vector and returns a matrix.
matrix third_diff(int N) {
    if (N <= 3) return matrix(0, 0);

    matrix D(N-3, N);
    for(int i=0; i<N-3; i++) {
        D(i, i) = -1.0;
        D(i, i+1) = 3.0;
        D(i, i+2) = -3.0;
        D(i, i+3) = 1.0;
    }
    return D; //D matrix is returned
}
//declip functgion that takes vector, min and max values as specifeid. Then gives the declipped vector
lineq::vector declip(const lineq::vector &y, double y_min, double y_max) {
    int N = y.size();
    
    std::vector<int> clipped_idx;
    lineq::vector y_tilde(N);
    
    for(int i=0; i<N; i++) {
        if(y[i] == y_max || y[i] == y_min) {
            clipped_idx.push_back(i);
            y_tilde[i] = 0.0;
        } else {
            y_tilde[i] = y[i];
        }
    }
    int n = clipped_idx.size();
    if(n == 0) return y; //no clipping

    lineq::matrix M(N, n);
    for(int j=0; j<n; j++) {
        M(clipped_idx[j], j) = 1.0;
    }
    lineq::matrix D = third_diff(N);
    //here the function ||D * (y_clipped + M * z)||^2 is minimised from notes, which is  solving the least squares problem D * M * z = -D * y_clipped
    lineq::matrix A = D * M;
    lineq::vector Dy = D * y_tilde;
    lineq::vector b_vec(D.size1());
    for(int k = 0; k< D.size1(); k++) b_vec[k] = -Dy[k];

    lineq::matrix A_copy(A);
    lineq::matrix R(n, n);

    QR_decomp(A_copy, R);

    lineq::vector Qtb(n);
    for(int l=0; l<n; l++) {
        double sum = 0;
        for(int m=0; m<A.size1(); m++) {
            sum += A_copy(m, l) * b_vec[m];
        }
        Qtb[l] = sum;
    }

        lineq::vector z = backsub(R, Qtb);

        lineq::vector x = y_tilde;
        for(int p=0; p<n; p++) {
            x[clipped_idx[p]] = z[p];
        }
        return x;
    }


///Here is the function that handles the sine wave part of the question

void SineQuestion() {
    std::cout << "################# Sine wave declipping ################\n\n" << std::endl;
    int N = 200;
    lineq::vector y_original(N);
    lineq::vector y_tilde(N);

    ///values specified for sine part of question
    double y_min = -0.8;
    double y_max = 0.8;

    //conditions here such that it clips at max a min points specidifed for sine wave
    for(int i=0; i<N; i++) {
        double t = 2.0 * M_PI * i / N;
        y_original[i] = sin(t);
        if(y_original[i] >= y_max) {
            y_tilde[i] = y_max;
        } else if (y_original[i] <= y_min) {
            y_tilde[i] = y_min;
        } else {
            y_tilde[i] = y_original[i];
        }
    }

    lineq::vector y_declipped = declip(y_tilde, y_min, y_max);

    std::cout << "Signal length is " << N << std::endl;
    std::cout << "Clip limits are [" << y_min << ", " << y_max << "]" << std::endl;

    int num_clipped = 0;
    for(int i =0; i<N; i++) if (y_tilde[i] == y_min || y_tilde[i] == y_max) num_clipped++;
    std::cout << "Number of clipped points: " << num_clipped << std::endl;

    ofstream output_file("sine_declipping.dat");
    output_file << "# t y_original y_tilde y_declipped\n";
    for(int i=0; i<N; i++) {
        double t = 2.0 * M_PI * i / N;
        output_file << t << " " << y_original[i] << " " << y_tilde[i] << " " << y_declipped[i] << "\n";
    }
    output_file.close();

    double error_sum = 0.0;
    for(int i=0; i<N; i++) {
        error_sum += pow(y_declipped[i] - y_original[i], 2);
    }
    std::cout << "Sum of squared errors: " << error_sum << std::endl;
    double rms = sqrt(error_sum / N);
    std::cout << "RMS error: " << rms << std::endl;
    std::cout << "Output written to sine_declipping.dat" << std::endl;

}

void complexQuestion() {

    std::cout << "################# Complex signal declipping ################\n\n" << std::endl;

    int N = 200;
    // lineq::vector y_my_function(N);
    lineq::vector y_lecture_notes(N);
    lineq::vector y_tilde(N);

    //differen limits
    double y_min = -2.0;
    double y_max = 2.0;

    for(int i = 0; i<N; i++) {
        //Function taken from lecture notes (eq 46)
        double arg = 4.0 * M_PI * i / (N - 1);
        y_lecture_notes[i] = 2.0 *sin(0.9 * arg) - 1.0 * sin(2.1 * arg) + 0.5 * sin(3.1 * arg);
        
        if(y_lecture_notes[i] > y_max) {
            y_tilde[i] = y_max;
        } else if (y_lecture_notes[i] < y_min) {
            y_tilde[i] = y_min;
        } else {    
            y_tilde[i] = y_lecture_notes[i];
        }
    
    }

    lineq::vector y_declipped = declip(y_tilde, y_min, y_max);

    std::cout << "Signal length is " << N << std::endl;
    std::cout << "Clip limits are [" << y_min << ", " << y_max << "]" << std::endl;

    int num_clipped = 0;
    for(int i =0; i<N; i++) if (y_tilde[i] == y_min || y_tilde[i] == y_max) num_clipped++;
    std::cout << "Number of clipped points: " << num_clipped << std::endl;

    ofstream output_file("complex_declipping.dat");
    output_file << "# t y_lecture_notes y_tilde y_declipped\n";
    for(int i=0; i<N; i++) {
        double t = 2.0 * M_PI * i / N;
        output_file << t << " " << y_lecture_notes[i] << " " << y_tilde[i] << " " << y_declipped[i] << "\n";
    }
    output_file.close();

    double error_sum = 0.0;
    for(int i=0; i<N; i++) {
        error_sum += pow(y_declipped[i] - y_lecture_notes[i], 2);
    }
    std::cout << "Sum of squared errors: " << error_sum << std::endl;
    double rms = sqrt(error_sum / N);
    std::cout << "RMS error: " << rms << std::endl;
    std::cout << "Output written to complex_declipping.dat" << std::endl;

}

int main() {
    SineQuestion();
    complexQuestion();
    return 0;
}