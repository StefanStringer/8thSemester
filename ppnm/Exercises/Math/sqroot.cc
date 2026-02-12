#include<iostream>
#include<cmath>

int main(int arg, char** argv)
{
    int n = std::stoi(argv[1]);
    double result = std::sqrt(n);  
    std::cout << "sqrt(" << n << ") = " << result << "\n";
    return 0;
}