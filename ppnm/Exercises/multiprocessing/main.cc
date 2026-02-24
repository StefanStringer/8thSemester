#include <iostream>
#include <cmath>
#include <thread>
#include <string>
#include <vector>

struct datum {
    int start, end;
    double sum;
};

void harm(datum& p) {
    int start = p.start;
    int end = p.end;
    double sum = 0;
    for (int i = start; i < end; i++) {
        sum += 1.0 / i;
    }
    p.sum = sum;
}

int main(int argc, char** argv) {
    int nterms = static_cast<int>(1e9); // default: 1e9 terms
    int nthreads = 1;                   // default: 1 thread

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-terms" && i + 1 < argc) {
            nterms = static_cast<int>(std::stoll(argv[++i]));
        }
        if (arg == "-threads" && i + 1 < argc) {
            nthreads = std::stoi(argv[++i]);
        }
    }

    std::cout << "terms: " << nterms << "\n";
    std::cout << "threads: " << nthreads << "\n";

    // Prepare data objects for each thread
    std::vector<datum> data(nthreads);
    for (int i = 0; i < nthreads; ++i) {
        data[i].start = 1 + (nterms / nthreads) * i;
        data[i].end   = 1 + (nterms / nthreads) * (i + 1);
    }
    // Fix last thread's endpoint
    data[nthreads - 1].end = nterms + 1;

    // Launch threads
    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    for (int i = 0; i < nthreads; ++i) {
        threads.emplace_back(harm, std::ref(data[i]));
    }

    // Join threads
    for (std::thread &t : threads) {
        t.join();
    }

    // Sum up results
    double total = 0;
    for (datum &d : data) {
        total += d.sum;
    }

    std::cout << "total sum = " << total << std::endl;

    return 0;
}
