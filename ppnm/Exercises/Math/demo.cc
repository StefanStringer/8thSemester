#include <iostream>
#include <iomanip>
#include <numbers>

int main() {
    std::cout << std::setprecision(15);
    std::cout << "π  = " << std::numbers::pi   << '\n';
    std::cout << "e  = " << std::numbers::e    << '\n';
    std::cout << "√2 = " << std::numbers::sqrt2 << '\n';
    return 0;
}