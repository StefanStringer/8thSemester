#include<iostream>
#include<limits>
#include<iomanip>
#include<cmath>

bool approx(double a, double b,
            double acc = 1e-9,   // absolute tolerance
            double eps = 1e-9)   // relative tolerance
{
    double diff = std::abs(a - b);

    if (diff <= acc) return true;

    double max_ab = std::max(std::abs(a), std::abs(b));
    if (max_ab == 0.0)
        return true;
                                    
    return diff <= eps * max_ab;
}

int main() {
    //machine epsilon

    std::cout << "Part 1: machine epsilon\n" << std::endl;

    float       f=1.0;
    while((float)      (1.0+f) != 1.0){
        f/=2.0;
    } f*=2.0;

    double      d=1.0;
    
    while((double)     (1.0+d) != 1.0){
        d/=2.0;
    } d*=2.0;

    long double l=1.0;
    
    while((long double)(1.0+l) != 1.0){
        l/=2.0;
    } l*=2.0;

    //Here the simulated epsiolon is printed
    std::printf("      float eps=%g\n",f);
    std::printf("     double eps=%g\n",d);
    std::printf("long double eps=%Lg\n",l);

    //Here the machine epsilon from the library is printed
    std::cout << std::numeric_limits<float>::epsilon() << "\n";
    std::cout << std::numeric_limits<double>::epsilon() << "\n";
    std::cout << std::numeric_limits<long double>::epsilon() << "\n";

    // Here the calculated machine epislon is done
    std::cout << "Calculated epsilon double = " << std::pow(2,-52) << std::endl;
    std::cout << "Calculated epsilon float =  " << std::pow(2,-52) << std::endl;

    // float a = 1.0f + f + f; // should be 1+epsilon, that is, larger than 1, no?
    // float b = f + f + 1.0f; // should be the same mathematically, no?
    // std::printf("a=%g\n", a);
    // std::printf("b=%g\n", b);


    // Part 2 Non-commutativity of addition
    std::cout << "\nPart 2: non-commutativity of addition\n" << std::endl;
    double epsilon=std::pow(2,-52);
    double tiny=epsilon/2;
    double a=1+tiny+tiny;
    double b=tiny+tiny+1;
    std::cout << "a==b ? " << (a==b ? "true":"false") << "\n";
    std::cout << "a>1  ? " << (a>1  ? "true":"false") << "\n";
    std::cout << "b>1  ? " << (b>1  ? "true":"false") << "\n";

    #include<iomanip>
    std::cout << std::fixed << std::setprecision(17);
    std::cout << "       tiny=" << tiny << "\n";
    std::cout << "1+tiny+tiny=" << a << "\n";
    std::cout << "tiny+tiny+1=" << b << "\n";

    //Part 3 Comparing doubles: introduction
    std::cout << "\nPart 3: comparing doubles\n" << std::endl;
    
    double do1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
    double do2 = 8*0.1;

    std::cout << "d1==d2? " << (do1==do2 ? "true":"false") << "\n";
    
    #include<iomanip>
    std::cout << std::fixed << std::setprecision(17);
    std::cout << "d1=" << do1 << "\n";
    std::cout << "d2=" << do2 << "\n";

    double d1 = 0.123456789012345;
    double d2 = 0.123456789012346;   // differs only in the last digit

    std::cout << std::setprecision(17); // show full double precision

    std::cout << "d1 = " << d1 << '\n';
    std::cout << "d2 = " << d2 << '\n';

    bool same = approx(d1, d2);   // uses default tolerances (1e‑9)

    std::cout << "\nUsing approx(d1, d2) with default tolerances:\n";
    std::cout << (same ? "≈ (approximately equal)" : "≠ (different)") << '\n';

    double tighterAcc = 1e-12;
    double tighterEps = 1e-12;
    bool sameTight = approx(d1, d2, tighterAcc, tighterEps);
    std::cout << "\nWith tighter tolerances (acc = eps = 1e‑12):\n";
    std::cout << (sameTight ? "≈" : "≠") << '\n';

    double looserAcc = 1e-6;
    double looserEps = 1e-6;
    bool sameLoose = approx(d1, d2, looserAcc, looserEps);
    std::cout << "\nWith looser tolerances (acc = eps = 1e‑6):\n";
    std::cout << (sameLoose ? "≈" : "≠") << '\n';

    return 0;
}
