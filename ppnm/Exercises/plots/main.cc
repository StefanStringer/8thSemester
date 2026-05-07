#include <iostream>
#include <cstdlib>
#include <string>
#include "functions.h"

// ------------------------ Plotting Function ------------------------
void plot_with_gnuplot(const std::string& datafile,
                       const std::string& outputfile,
                       const std::string& title,
                       bool is_gamma = false,
                       bool is_lngamma = false) {
    
    std::string safe_title = title;
    size_t pos = 0;
    while ((pos = safe_title.find("'", pos)) != std::string::npos) {
        safe_title.replace(pos, 1, "''");
        pos += 2;
    }

    std::string cmd = "gnuplot -e \"";
    cmd += "set terminal svg size 1000,700 dynamic font 'Arial,16';";
    cmd += "set output '" + outputfile + "';";
    cmd += "set border 3 lw 2;";
    cmd += "set grid lw 1 lc rgb '#bbbbbb';";
    cmd += "set title '" + safe_title + "' font ',18';";
    cmd += "set xlabel 'x' font ',16';";
    cmd += "set ylabel 'y' font ',16';";
    cmd += "set xtics font ',14';";
    cmd += "set ytics font ',14';";
    cmd += "set tics scale 1.5;";
    cmd += "set mxtics 2;";
    cmd += "set mytics 2;";

    if (is_gamma && !is_lngamma) {
  
        cmd += "set logscale y;";
        cmd += "plot '" + datafile + "' index 0 using 1:2 with points pt 7 ps 2 lc rgb 'red' title 'Stirling',";
        cmd += " '' index 1 using 1:2 with lines lw 2 lc rgb 'blue' title 'Stirling Smooth',";
        cmd += " '' index 1 using 1:3 with lines lw 2 lc rgb 'green' title 'std::tgamma'";
    } else {
        cmd += "plot '" + datafile + "' using 1:2 with lines lw 2 lc rgb 'blue' title '" + safe_title + "',";
        cmd += " '" + datafile + "' every 10 using 1:2 with points pt 7 ps 2 lc rgb 'red' notitle";
    }

    cmd += "\"";

    std::cout << "Running: " << cmd << "\n";
    int result = std::system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Error running gnuplot (exit code " << result << "). Is gnuplot installed?\n";
    } else {
        std::cout << "Generated: " << outputfile << "\n";
    }
}

int main() {
    std::cout << "Generating Error Function data...\n";
    generate_data("erf.dat", erf_approx, -3.0, 3.0, 500);
    plot_with_gnuplot("erf.dat", "erf.svg", "Error Function");

    std::cout << "Generating Gamma Function data...\n";
    // Coarse step 1.0, Fine step 0.01
    generate_gamma_data("gamma.dat", 0.1, 10.0, 1.0, 0.01);
    plot_with_gnuplot("gamma.dat", "gamma.svg", "Gamma Function", true, false);

    std::cout << "Generating Log-Gamma Function data...\n";
    // Coarse step 1.0, Fine step 0.1
    generate_lngamma_data("lngamma.dat", 0.1, 50.0, 1.0, 0.1);
    plot_with_gnuplot("lngamma.dat", "lngamma.svg", "Log-Gamma Function", true, true);

    std::cout << "All data and plots generated.\n";
    return 0;
}