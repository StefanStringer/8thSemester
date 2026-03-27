import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate


# PLOT 1: erf convergence

data = np.loadtxt("erf_data.txt")

acc = data[:, 0]
err = data[:, 1]

plt.figure()
plt.loglog(acc, err, 'o-', label="Your integrator")

plt.loglog(acc, acc, '--', label="~O(acc) reference")

plt.xlabel("Accuracy goal (acc)")
plt.ylabel("Absolute error")
plt.title("Convergence of erf(1)")
plt.grid(True, which="both")
plt.legend()

plt.savefig("erf_convergence.svg")
plt.close()


# PLOT 2: Comparison with NumPy

def f1(x):
    return 1/np.sqrt(x)

def f2(x):
    return np.log(x)/np.sqrt(x)

#remember we use quad to intergrate from computational physics
# res1, err1 = integrate.quad(f1, 0, 1, limit=100)
# res2, err2 = integrate.quad(f2, 0, 1, limit=100)

# labels = []
# calls = []

# with open("compare_data.txt") as f:
#     next(f)
#     for line in f:
#         name, val = line.split()
#         labels.append(name)
#         calls.append(float(val))

# labels.append("numpy_quad")
# calls.append(100)

# # bar plot ??? Like what else should i plot it as?
# plt.figure()
# plt.bar(labels, calls)

# plt.ylabel("Function evaluations")
# plt.title("Comparison of integration efficiency")
# plt.xticks(rotation=30)

# plt.savefig("comparison.svg")
# plt.close()

print("Plots saved: erf_convergence.svg")#, comparison.svg")