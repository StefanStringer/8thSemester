import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("erf.svg.dat")
z = data[:,0]
erf_vals = data[:,1]

plt.figure(figsize=(6,4))
plt.plot(z, erf_vals, label="my_erf(z)", color="blue")
plt.xlabel("z")
plt.ylabel("erf(z)")
plt.title("Error function erf(z)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("erf.svg")
plt.close()

acc_data = np.loadtxt("erf_acc.svg.dat")
acc = acc_data[:,0]
vals = acc_data[:,1]
abs_err = acc_data[:,2]

plt.figure(figsize=(6,4))
plt.loglog(acc, abs_err, marker='o', linestyle='-', color="red")
plt.xlabel("Requested accuracy (acc)")
plt.ylabel("Absolute error |my_erf(1)-exact|")
plt.title("Convergence of erf(1) with decreasing acc")
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.savefig("erf_acc.svg")
plt.close()

print("Plots saved: erf.svg, erf_acc.svg")