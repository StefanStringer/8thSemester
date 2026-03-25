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

void QuestionA(){
    using vec = Eigenvalues::vector;

    /* =========================
       1. Harmonic oscillator
    ========================= */
    {
        auto f = [](double x, const vec& y){
            vec dydx(2);
            dydx[0] = y[1];
            dydx[1] = -y[0];
            return dydx;
        };

        vec y0(2);
        y0[0] = 1;
        y0[1] = 0;

        auto [xlist, ylist] = ode::driver(f, 0, 10, y0, 0.1, 1e-3, 1e-3);

        plotSVG("harmonic.svg", xlist, ylist, 800, 600);
    }

    /* =========================
       2. Damped oscillator
    ========================= */
    {
        double gamma = 0.5;

        auto f = [gamma](double x, const vec& y){
            vec dydx(2);
            dydx[0] = y[1];
            dydx[1] = -y[0] - gamma*y[1];
            return dydx;
        };

        vec y0(2);
        y0[0] = 1;
        y0[1] = 0;

        auto [xlist, ylist] = ode::driver(f, 0, 10, y0, 0.1, 1e-3, 1e-3);

        plotSVG("damped.svg", xlist, ylist, 800, 600);
    }

    /* =========================
       3. Exponential decay
    ========================= */
    {
        auto f = [](double x, const vec& y){
            vec dydx(1);
            dydx[0] = -y[0];
            return dydx;
        };

        vec y0(1);
        y0[0] = 1;

        auto [xlist, ylist] = ode::driver(f, 0, 5, y0, 0.1, 1e-3, 1e-3);

        plotSVG("decay.svg", xlist, ylist, 800, 600);
    }

    std::cout << "Question A plots generated:\n";
    std::cout << " - harmonic.svg\n";
    std::cout << " - damped.svg\n";
    std::cout << " - decay.svg\n";
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