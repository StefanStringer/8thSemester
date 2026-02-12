#include<iostream>
#include<cmath>

int main(int argc, char** argv)
{
    double base = std::stoi(argv[1]);
    double exp = std::stoi(argv[2]);
    double result = pow(base, exp);
    std::cout << base << "^" << exp << " = " << result << "\n";
    return 0;
}