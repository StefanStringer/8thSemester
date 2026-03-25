#include <iostream>
#include <cmath>
#include "matrix.hpp"
#include "ode.hpp"

using namespace Eigenvalues;
using namespace ode;

// ====================================
// Question A: Harmonic oscillator example
// ====================================
#include <fstream>  // for file output
#include <string>
#include <algorithm>

void plotSVG(const std::string &filename,
             const std::vector<double> &x,
             const std::vector<vector> &y,
             int width=800, int height=600)
{
    double xmin = *std::min_element(x.begin(), x.end());
    double xmax = *std::max_element(x.begin(), x.end());

    double ymin = y[0][0], ymax = y[0][0];
    for(const auto &v : y)
        for(double val : v){
            ymin = std::min(ymin, val);
            ymax = std::max(ymax, val);
        }

    std::ofstream file(filename);
    file << "<svg xmlns='http://www.w3.org/2000/svg' "
            "width='" << width << "' height='" << height << "'>\n";

    // margin
    int left = 50, right = 20, top = 20, bottom = 50;

    auto transformX = [&](double xx){
        return left + (xx-xmin)/(xmax-xmin)*(width-left-right);
    };
    auto transformY = [&](double yy){
        return height - bottom - (yy-ymin)/(ymax-ymin)*(height-top-bottom);
    };

    // axes
    file << "<line x1='" << left << "' y1='" << height-bottom
         << "' x2='" << width-right << "' y2='" << height-bottom
         << "' stroke='black'/>\n";
    file << "<line x1='" << left << "' y1='" << top
         << "' x2='" << left << "' y2='" << height-bottom
         << "' stroke='black'/>\n";

    // colors for curves
    std::vector<std::string> colors = {"red","blue","green","orange","purple"};

    for(size_t k=0;k<y[0].size();k++){
        file << "<polyline fill='none' stroke='" << colors[k%colors.size()]
             << "' stroke-width='2' points='";

        for(size_t i=0;i<x.size();i++){
            double xx = transformX(x[i]);
            double yy = transformY(y[i][k]);
            file << xx << "," << yy << " ";
        }
        file << "'/>\n";
    }

    file << "</svg>\n";
    file.close();
}

void QuestionA() {
    std::cout << "=== Question A: Harmonic Oscillator ===\n";

    vector y0(2);
    y0[0] = 1.0;
    y0[1] = 0.0;

    auto f = [](double t, const vector& y) -> vector {
        vector dy(2);
        dy[0] = y[1];
        dy[1] = -y[0];
        return dy;
    };

    double a = 0.0, b = 10.0, h = 0.1, acc = 1e-6, eps = 1e-6;

    auto [xlist, ylist] = driver(f, a, b, y0, h, acc, eps);

    plotSVG("QuestionA.svg", xlist, ylist);
    std::cout << "SVG plot saved to QuestionA.svg\n";
}

// ====================================
// Question B: Relativistic prevission of planetary orbit
// ====================================
void QuestionB() {
    std::cout << "=== Question B: Relativistic Precession of Planetary Orbit ===\n";

}

// ====================================
// Main function
// ====================================
int main() {
    QuestionA();
    QuestionB();
    return 0;
}