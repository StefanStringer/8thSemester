// ------------------------------------------------------------
// main.cpp
// ------------------------------------------------------------
#include "vec.h"
#include <iostream>

int main() {
    // ----- Constructors -------------------------------------------------
    vec a{1.0, 2.0, 3.0};          // parameterized
    vec b;                         // default (0,0,0)
    vec c = a;                     // copy construction
    vec d = std::move(b);          // move construction (b becomes unspecified)

    // ----- Assignment ---------------------------------------------------
    b = a;                         // copy assignment
    d = std::move(c);              // move assignment

    // ----- Arithmetic ---------------------------------------------------
    vec e = a + b;                 // binary +
    vec f = e - a;                 // binary -
    vec g = f * 2.5;               // scalar multiplication (right side)
    vec h = 0.5 * g;               // scalar multiplication (left side)
    vec i = h / 2.0;               // division by scalar
    i += a;                        // compound +=
    i -= b;                        // compound -=
    i *= 3.0;                      // compound *=
    i /= 3.0;                      // compound /=

    // ----- Unary minus --------------------------------------------------
    vec j = -i;                    // unary -

    // ----- Stream output ------------------------------------------------
    std::cout << "Vector j (stream output): " << j << '\n';

    // ----- Debug print --------------------------------------------------
    j.print("j");                  // prints with optional prefix

    // ----- Approximate equality -----------------------------------------
    vec k{1.000001, 2.000001, 3.000001};
    bool same = approx(j, k, 1e-5, 1e-5);
    std::cout << "j and k are approximately equal? "
              << (same ? "yes" : "no") << '\n';

    // ----- Show that everything works together -------------------------
    std::cout << "\n--- Summary of all vectors ---\n";
    std::cout << "a = " << a << "\n"
              << "b = " << b << "\n"
              << "d = " << d << "\n"
              << "e = " << e << "\n"
              << "f = " << f << "\n"
              << "g = " << g << "\n"
              << "h = " << h << "\n"
              << "i = " << i << "\n"
              << "j = " << j << "\n";

    return 0;
}