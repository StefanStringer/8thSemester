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

    // Start at 1 to skip program name
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            try {
                numbers.push_back(std::stod(argv[i + 1]));
                i++; // Skip the next argument (the number)
            } catch (const std::exception& e) {
                std::cerr << "Error parsing number: " << argv[i+1] << "\n";
                return EXIT_FAILURE;
            }
        }
    }

    if (numbers.empty()) {
        std::cerr << "No numbers provided with -n flag.\n";
        return EXIT_FAILURE;
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
    bool has_input = false;

    while (std::cin >> x) {
        has_input = true;
        std::cout << x << " "
                  << std::sin(x) << " "
                  << std::cos(x) << std::endl;
    }

    if (!has_input) {
        std::cerr << "No input received on stdin.\n";
        return EXIT_FAILURE;
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
        } else if (arg == "--output" && i + 1 < argc) {
            outfile = argv[++i];
        }
    }

    if (infile.empty() || outfile.empty()) {
        std::cerr << "Usage: ./main --input <file> --output <file>\n";
        return EXIT_FAILURE;
    }

    std::ifstream input(infile);
    std::ofstream output(outfile);

    if (!input.is_open()) {
        std::cerr << "Error: Could not open input file: " << infile << "\n";
        return EXIT_FAILURE;
    }
    if (!output.is_open()) {
        std::cerr << "Error: Could not open output file: " << outfile << "\n";
        return EXIT_FAILURE;
    }

    double x;
    bool has_data = false;
    while (input >> x) {
        has_data = true;
        output << x << " "
               << std::sin(x) << " "
               << std::cos(x) << std::endl;
    }

    if (!has_data) {
        std::cerr << "Warning: Input file was empty or contained no valid numbers.\n";
    }

    input.close();
    output.close();

    return EXIT_SUCCESS;
}


// ======================================================
// MAIN DISPATCHER
// ======================================================
int main(int argc, char* argv[]) {
    // Explicitly mark parameters as potentially unused to avoid -Werror
    (void)argc;
    (void)argv;

#ifdef PART1
    return main_part1(argc, argv);
#elif defined(PART2)
    return main_part2();
#elif defined(PART3)
    return main_part3(argc, argv);
#else
    std::cerr << "Error: No mode selected. Compile with -DPART1, -DPART2, or -DPART3.\n";
    return EXIT_FAILURE;
#endif
}

