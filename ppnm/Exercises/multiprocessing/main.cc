#include <iostream>
#include <cmath>
#include <thread>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <chrono>

struct datum {
    int start;
    int end;
    double sum;
};

void harm(datum& p) {
    double sum = 0.0;
    for (int i = p.start; i < p.end; ++i) {
        sum += 1.0 / static_cast<double>(i);
    }
    p.sum = sum;
}

int main(int argc, char** argv) {
    long long nterms = static_cast<long long>(1e9); // Use long long for large numbers
    int nthreads = 1;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-terms" && i + 1 < argc) {
            try {
                nterms = std::stoll(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid number for -terms\n";
                return EXIT_FAILURE;
            }
        } else if (arg == "-threads" && i + 1 < argc) {
            try {
                nthreads = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid number for -threads\n";
                return EXIT_FAILURE;
            }
        }
    }

    if (nthreads <= 0) {
        std::cerr << "Number of threads must be > 0\n";
        return EXIT_FAILURE;
    }
    if (nterms <= 0) {
        std::cerr << "Number of terms must be > 0\n";
        return EXIT_FAILURE;
    }

    std::vector<datum> data(nthreads);
    

    long long chunk = nterms / nthreads;
    long long remainder = nterms % nthreads;
    
    int current_start = 1;
    for (int i = 0; i < nthreads; ++i) {
        // Distribute remainder to the first 'remainder' threads
        long long current_chunk = chunk + (i < remainder ? 1 : 0);
        
        data[i].start = current_start;
        data[i].end = current_start + current_chunk; // Exclusive end
        data[i].sum = 0.0;
        
        current_start = data[i].end;
    }

    // Measure time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Launch threads
    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    for (int i = 0; i < nthreads; ++i) {
        threads.emplace_back(harm, std::ref(data[i]));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Sum up results
    double total = 0.0;
    for (const auto& d : data) {
        total += d.sum;
    }

    // Output results
    // Format: threads, time_seconds, user_time (simulated by wall time here)
    std::cout << nthreads << " " << elapsed.count() << " " << total << "\n";

    return EXIT_SUCCESS;
}