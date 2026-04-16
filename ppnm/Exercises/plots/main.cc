#include <iostream>
#include <cstdlib>
#include "functions.h"

// ------------------------ Plotting Function ------------------------
void plot_with_gnuplot(const std::string& datafile,
                       const std::string& outputfile,
                       const std::string& title,
                       bool gamma=false,
                       bool is_lngamma=false)
{
    std::string command = "gnuplot -e \"";
    command += "set terminal svg size 1000,700 dynamic font 'Arial,16';";
    command += "set output '" + outputfile + "';";
    command += "set border 3 lw 2;";
    command += "set grid lw 1 lc rgb '#bbbbbb';";
    command += "set title '" + title + "' font ',18';";
    command += "set xlabel 'x' font ',16';";
    command += "set ylabel 'y' font ',16';";
    command += "set xtics font ',14';";
    command += "set ytics font ',14';";
    command += "set tics scale 1.5;";
    command += "set mxtics 2;";
    command += "set mytics 2;";

    if(gamma && !is_lngamma) {
        command += "set logscale y;";
        command += "plot '" + datafile + "' index 0 using 1:2 with points pt 7 ps 2 lc rgb 'red' title 'Stirling',";
        command += " '' index 1 using 1:2 with lines lw 2 lc rgb 'blue' title 'Stirling smooth',";
        command += " '' index 1 using 1:3 with lines lw 2 lc rgb 'green' title 'std::tgamma'";
    } else {
        command += "plot '" + datafile + "' with lines lw 2 lc rgb 'blue' title '" + title + "',";
        command += " '" + datafile + "' every 10 using 1:2 with points pt 7 ps 2 lc rgb 'red' notitle";
    }

    command += "\"";

    int result = std::system(command.c_str());
    if(result != 0) std::cerr << "Error running gnuplot\n";
}




// ------------------------ Main ------------------------
int main()
{
    // Error function
    generate_data("erf.dat", erf_approx, -3.0, 3.0, 500);
    plot_with_gnuplot("erf.dat", "erf.svg", "Error Function");

    // Gamma function
    generate_gamma_data("gamma.dat", 0.1, 10.0, 1.0, 0.01);
    plot_with_gnuplot("gamma.dat", "gamma.svg", "Gamma Function", true);

    // Log-Gamma function
    generate_lngamma_data("lngamma.dat", 0.1, 50.0, 1.0, 0.1);
    plot_with_gnuplot("lngamma.dat", "lngamma.svg", "Log-Gamma Function", true, true);

    std::cerr << "All data and plots generated.\n";
    return 0;
}
