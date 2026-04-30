#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <iomanip>
#include "matrix.hpp"

using namespace Eigenvalues;

void QuestionA();


void QuestionB();


void QuestionC();

int main() {

    std::ofstream outFile("Out.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create Out.txt\n";
        return 1;
    }
    std::streambuf* old = std::cout.rdbuf(outFile.rdbuf());

    std::cout << "######### Homework: Minimisation #########\n\n";
    
    std::cout << "######### Question A: #########\n\n";
    QuestionA();
    std::cout << "\n#################################################################\n\n";
    
    std::cout << "######### Question B: #########\n\n";
    QuestionB();
    std::cout << "\n#################################################################\n\n";
    
    std::cout << "######### Question C: #########\n\n";
    QuestionC();
    std::cout << "\n#################################################################\n\n";

    std::cout.rdbuf(old);
    outFile.close();
    
    std::cout << "Results written to Out.txt\n";
    return 0;
}