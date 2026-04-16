#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <functional>
#include "matrix.hpp"   // need for lineq
using namespace lineq; // taken lineq from prevous homework

//---------------------- RK12 ODE SOLVER ----------------------
// Single Runge-Kutta step with embedded error estimate
std::pair<lineq::vector, lineq::vector> rkstep12(
    std::function<lineq::vector(double, lineq::vector)> f,
    double x,
    lineq::vector y,
    double h
) {
    lineq::vector k0 = f(x, y);            // Euler step (first-order)
    lineq::vector k1 = f(x + h/2, y + k0*(h/2)); // Midpoint (second-order)
    lineq::vector yh = y + k1*h;           // Estimate y(x+h)
    lineq::vector dy = (k1 - k0)*h;        // Error estimate
    return {yh, dy};
}

// Adaptive-step ODE integrator
std::pair<std::vector<double>, std::vector<lineq::vector>> driver(
    std::function<lineq::vector(double, lineq::vector)> F,
    std::pair<double,double> interval,
    lineq::vector yinit,
    double h=0.125,
    double acc=1e-6,
    double eps=1e-6
) {
    auto [a,b] = interval;
    double x = a;
    lineq::vector y = yinit;

    std::vector<double> xlist = {x};
    std::vector<lineq::vector> ylist = {y};

    do {
        if (x >= b) return {xlist, ylist};
        if (x + h > b) h = b - x;

        auto [yh, dy] = rkstep12(F, x, y, h);
        double tol = (acc + eps*yh.norm())*std::sqrt(h/(b-a));
        double err = dy.norm();

        if (err <= tol) {
            x += h; y = yh;
            xlist.push_back(x);
            ylist.push_back(y);
        }

        if (err > 0) h *= std::min(std::pow(tol/err,0.25)*0.95,2.0);
        else h *= 2;

    } while (true);
}

//---------------------- QUESTION A: PENDULUM ----------------------
void QuestionA() {
    std::cout << "---- QUESTION A: Pendulum ----\n";

    double b = 0.25; // damping
    double c = 5.0;  // gravity

    auto F = [b,c](double t, vector y) -> vector {
        vector dydt(2);
        dydt[0] = y[1];                  // theta' = omega
        dydt[1] = -b*y[1] - c*std::sin(y[0]);
        return dydt;
    };

    vector y0(2);
    y0[0] = M_PI-0.1; y0[1] = 0.0;

    auto [ts, ys] = driver(F, {0,10}, y0, 0.01, 1e-6, 1e-6);

    std::ofstream f("Question_A.dat");
    for(size_t i=0;i<ts.size();i++)
        f << ts[i] << " " << ys[i][0] << " " << ys[i][1] << "\n";
    f.close();
}

//---------------------- QUESTION B: ORBITS ----------------------
void QuestionB() {
    std::cout << "---- QUESTION B: Orbits ----\n";

    // Orbit ODE system generator
    auto makeOrbit = [](double eps){
        return [eps](double phi, lineq::vector y) -> lineq::vector {
            lineq::vector dydt(2);
            dydt[0] = y[1];
            dydt[1] = 1 - y[0] + eps*y[0]*y[0];
            return dydt;
        };
    };

    std::vector<double> epss = {0.0, 0.0, 0.01};
    std::vector<double> uprimes = {0.0, -0.5, -0.5};

    std::ofstream f("Question_B.dat");
    for(size_t i=0;i<epss.size();i++){
        lineq::vector y0 = {1.0, uprimes[i]};
        auto [phis, us] = driver(makeOrbit(epss[i]), {0, 8*M_PI}, y0, 0.01, 1e-6, 1e-6);

        f << "# eps=" << epss[i] << " uprime=" << uprimes[i] << "\n";
        for(size_t j=0;j<phis.size();j++)
            f << phis[j] << " " << us[j][0] << " " << us[j][1] << "\n";
        f << "\n\n"; // separate blocks for gnuplot
    }
    f.close();
}

//---------------------- QUESTION C: THREE-BODY FIGURE-8 ----------------------
void QuestionC() {
    std::cout << "---- QUESTION C: Three-body Figure-8 ----\n";

    // 12-component vector: velocities + positions
    auto three_body_rhs = [](double t, lineq::vector z) -> lineq::vector {
        lineq::vector dz(12);

        // Extract positions
        double x1=z[6], y1=z[7];
        double x2=z[8], y2=z[9];
        double x3=z[10], y3=z[11];

        // Extract velocities
        double vx1=z[0], vy1=z[1];
        double vx2=z[2], vy2=z[3];
        double vx3=z[4], vy3=z[5];

        auto accel = [](double x1,double y1,double x2,double y2){
            double dx = x2-x1, dy = y2-y1;
            double r3 = std::pow(dx*dx+dy*dy,1.5);
            return lineq::vector{dx/r3, dy/r3};
        };
        // this is where we compute each accelerations
        lineq::vector a12 = accel(x1,y1,x2,y2);
        lineq::vector a13 = accel(x1,y1,x3,y3);
        lineq::vector a21 = accel(x2,y2,x1,y1);
        lineq::vector a23 = accel(x2,y2,x3,y3);
        lineq::vector a31 = accel(x3,y3,x1,y1);
        lineq::vector a32 = accel(x3,y3,x2,y2);

        // dx/dt = velocities
        dz[6]=vx1; dz[7]=vy1;
        dz[8]=vx2; dz[9]=vy2;
        dz[10]=vx3; dz[11]=vy3;

        // dv/dt = accelerations
        dz[0] = a12[0]+a13[0]; dz[1] = a12[1]+a13[1];
        dz[2] = a21[0]+a23[0]; dz[3] = a21[1]+a23[1];
        dz[4] = a31[0]+a32[0]; dz[5] = a31[1]+a32[1];

        return dz;
    };

    // Initial conditions from Wikipedia figure-8 solution given in question C
    lineq::vector z0(12);
    // velocities
    z0[0] = 0.347111; z0[1] = 0.0;
    z0[2] = -0.173556; z0[3] = 0.201532;
    z0[4] = -0.173556; z0[5] = -0.201532;
    // positions
    z0[6] = 0.970004; z0[7] = -0.243087;
    z0[8] = -0.970004; z0[9] = 0.243087;
    z0[10] = 0.0; z0[11] = 0.0;

    double t0 = 0.0;
    double tmax = 6.3259; // one period

    auto [ts, zs] = driver(three_body_rhs, {t0,tmax}, z0, 0.01, 1e-6, 1e-6);

    // Save positions for plotting
    std::ofstream f("Question_C.dat");
    for(size_t i=0;i<ts.size();i++){
        f << zs[i][6] << " " << zs[i][7] << " "
          << zs[i][8] << " " << zs[i][9] << " "
          << zs[i][10] << " " << zs[i][11] << "\n";
    }
    f.close();
}

//---------------------- MAIN FUNCTION ----------------------
int main() {
    QuestionA();
    QuestionB();
    QuestionC();
    return 0;
}