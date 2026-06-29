Here is the c++ code for problem 28 Least-Squares Signal Declipping.

It follows the algorithm described in the lecture notes for Ordinary Least Squares problem.
Section 1.4 expands upon other applications for least squares, where Signal smoothing
shows the basic calculations needed for the problem.

using equation x = y_tilde + Mz, one gets the reconstructed vector x, which is
a vector of elements within a given upper and lower bound for a signal/function.
The main part was going through the lecture notes, and making sure that i got the right calculations
and that i understood where they came from.
y_tilde: is the clipped signal with the samples replaced when over the threshold
M: is the matrix that places the unknown values into the positions that are clipped
z: is the unknown samples that have to be reconstructed

applying equation 50 to get the least squares solution helped make it simpler, as well
as making the third_diff function after what is seen in eq 51 to help declipping.

As I already had the OLS question as an exercise, I reused code from qr.cc for 
QR decomposition, so there is some 'legacy' code here.
additionally I had code from matrix.hpp for my vectors and matricies.

It is handled as two parts in my main.
SineQuestion() and complexQuestion().

SineQuestion() handles the first part, where a sine wave is created and has a min and max at [-0.8, 0.8].
there is a plot created named sine_declipping.svg showing the three original, clipped, and declipped signals.

Finally, complexQuestion() generates the fucntion given in the lecture notes at eq 46. This works the same, however the
min and max are [-2.0, 2.0].
From here it applies the same method and generates a plot of the same three functions at complex_declipping.svg.

Such that these can be plotted, .dat files are created with all the values generated too.

For self assesmnet, i would give 8/10. When looking at how the matrix D is written in the lecture notes, I only take the first row seen in notes (not interier rows of the matrix)
as its of the form [-1, 3, -3, 1]. Its an approximation, and not a full D. Although I have the right structure, it could be better.
Aditionally, i like how my complex function is from the lecture notes, but I could have picked something more interesting, or at least tested it on more rigerous functions
as I didnt really come up with this, i took the complex signal from the notes! (but i also like that i used that one, so i am split on it)
Ultimately I didnt write any conditions in my code to catch out whatever edge cases could appear and be safe against issues. I just wrote the basic conditions such that it
should work without major fail.