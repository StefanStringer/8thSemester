#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>

// ======================================================
// PART 1
// Command line usage:
// ./main -n 1 -n 2 -n 3
// ======================================================

int main_part1(int argc, char* argv[]) {
    std::vector<double> numbers;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            numbers.push_back(std::stod(argv[++i]));
        }
    }

    for (double n : numbers) {
        std::cout << n << " "
                  << std::sin(n) << " "
                  << std::cos(n) << std::endl;
    }

    return EXIT_SUCCESS;
}


// ======================================================
// PART 2
// Standard input usage:
// echo 1 2 3 | ./main
// ======================================================

int main_part2() {
    double x;

    while (std::cin >> x) {
        std::cout << x << " "
                  << std::sin(x) << " "
                  << std::cos(x) << std::endl;
    }

    return EXIT_SUCCESS;
}


// ======================================================
// PART 3
// File usage:
// ./main --input infile.txt --output outfile.txt
// ======================================================

int main_part3(int argc, char* argv[]) {
    std::string infile = "";
    std::string outfile = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input" && i + 1 < argc) {
            infile = argv[++i];
        }

        if (arg == "--output" && i + 1 < argc) {
            outfile = argv[++i];
        }
    }

    std::ifstream input(infile);
    std::ofstream output(outfile);

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files.\n";
        return EXIT_FAILURE;
    }

    double x;
    while (input >> x) {
        output << x << " "
               << std::sin(x) << " "
               << std::cos(x) << std::endl;
    }

    return EXIT_SUCCESS;
}


// ======================================================
// MAIN DISPATCHER
// (Selected via -DPART1 / -DPART2 / -DPART3)
// ======================================================

int main(int argc, char* argv[]) {

#ifdef PART1
    return main_part1(argc, argv);
#endif

#ifdef PART2
    return main_part2();
#endif

#ifdef PART3
    return main_part3(argc, argv);
#endif

    std::cerr << "No part selected during compilation.\n";
    return EXIT_FAILURE;
}
